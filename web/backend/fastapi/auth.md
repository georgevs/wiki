# API security with OAuth2

## Fetch dependencies
```bash
pip3 install \
  fastapi \
  passlib \
  pydantic \
  python-jose[cryptography] \
  python-multipart \
  uvicorn

sudo apt install jq
```

## Install live certificates
```bash
openssl enc -aes-128-cbc -pbkdf2 -salt -d -in ~/ws-archive/certs.tar.gz.enc | tar xzv
```

## Manage secrets

### Create secrets
```bash
mkdir -p secrets
cat - > secrets/.env << EOF
TOKEN_SECRET_KEY=$(openssl rand -hex 32)
TOKEN_ALGORITHM=HS256
EOF
```
### Push secrets:
```bash
tar czv secrets | openssl enc -aes-128-cbc -pbkdf2 -salt -out ~/ws-archive/secrets.fastapi.tar.gz.enc
```
### Fetch secrets:
```bash
openssl enc -d -aes-128-cbc -pbkdf2 -salt -in ~/ws-archive/secrets.fastapi.tar.gz.enc | tar xzv
```

## Run against local server
### Start server
```bash
env $(cat secrets/.env | xargs) uvicorn auth.main:app --host 172.20.0.220 --port 3443 --ssl-certfile ./certs/cert.pem --ssl-keyfile ./certs/cert-key-nopassword.pem
```
### Test authenticate
```bash
curl -s https://local.spamfro.site:3443/auth/token \
  -X POST \
  -H 'Content-Type: application/x-www-form-urlencoded' \
  -d 'grant_type=password&username=alice%40acme.com&password=Abc123!&client_id=&client_secret=' \
| tee secrets/credentials.json
```
### Test authenticated user
```bash
curl https://local.spamfro.site:3443/users/me \
  -H "Authorization: Bearer $(jq -r .access_token secrets/credentials.json)"
```
### Test protected resource access
```bash
curl https://local.spamfro.site:3443/items \
  -H "Authorization: Bearer $(jq -r .access_token secrets/credentials.json)"
```

## Import dependencies
```python
from dataclasses import dataclass
from datetime import datetime, timedelta, timezone
from fastapi import APIRouter, Depends, FastAPI, HTTPException, status
from fastapi.security import OAuth2PasswordRequestForm, OAuth2PasswordBearer
from jose import jwt
from passlib.context import CryptContext
from pydantic import BaseModel
from typing import Annotated, Iterator, List
from os import environ
```

## Instantiate routes and service
```python
app = FastAPI()
app.include_router(auth_router)
app.include_router(users_router)
app.include_router(items_router)
```

## Authenticate
Acquire bearer JWT acces token in an OAuth2 password flow.

### Authenticate user route
```python
class Token(BaseModel):
  access_token: str
  token_type: str = 'Bearer'

auth_router = APIRouter(prefix='/auth', tags=['auth'])

@auth_router.post('/token')
async def authenticate_user(form_data: Annotated[OAuth2PasswordRequestForm, Depends()]) -> Token:
  account = find_account_by_email(email=form_data.username)
  if not account or not verify_password(form_data.password, account.hashed_password):
    raise HTTPException(status_code=status.HTTP_401_UNAUTHORIZED, detail='User account not found or password mismatch')
  if not account.active:
    raise HTTPException(status_code=status.HTTP_401_UNAUTHORIZED, detail='User account deactivated')

  token_data=TokenData(email=account.email, expires=(datetime.utcnow() + timedelta(minutes=30)))
  return Token(access_token=create_access_token(token_data))
```

### Create JWT access token
```python
@dataclass
class TokenData:
  email: str
  expires: datetime

token_secret_key = environ['TOKEN_SECRET_KEY']
token_algorithm = environ['TOKEN_ALGORITHM']

def create_access_token(token_data: TokenData) -> str:
  claims = dict(sub=token_data.email, exp=int(token_data.expires.timestamp()))
  return jwt.encode(claims, key=token_secret_key, algorithm=token_algorithm)
```

### Password hashing
```python
bcrypt_context = CryptContext(schemes=["bcrypt"], deprecated="auto")

def hash_password(plaintext_password: str) -> str:
  return bcrypt_context.hash(plaintext_password)

def verify_password(plaintext_password: str, hashed_password: str) -> bool:
  return bcrypt_context.verify(plaintext_password, hashed_password)
```

### Fake accounts database
```python
@dataclass
class DbAccount:
  email: str
  hashed_password: str
  active: bool

def find_account_by_email(email: str) -> DbAccount | None:
  return next((it for it in fetch_accounts() if it.email == email), None)

def fetch_accounts() -> Iterator[DbAccount]:
  yield DbAccount(email='alice@acme.com', hashed_password=hash_password('Abc123!'), active=True)
  yield DbAccount(email='bob@acme.com', hashed_password=hash_password('Bcd234@'), active=True)
  yield DbAccount(email='dave@acme.com', hashed_password=hash_password('Cde345#'), active=False)
```

## Access protected resource
Guard resouce access by verifying the bearer JWT access token in request's HTTP `Authorization` header

### Authenticated user route
```python
class User(BaseModel):
  email: str

users_router = APIRouter(prefix='/users', tags=['users'])

oauth2_scheme = OAuth2PasswordBearer(tokenUrl='auth/token')

async def get_current_user_account(access_token: Annotated[str, Depends(oauth2_scheme)]) -> DbAccount:
  token_data = parse_access_token(access_token)
  account = find_account_by_email(email=token_data.email) if token_data else None
  if not account:
    raise HTTPException(status_code=status.HTTP_401_UNAUTHORIZED, detail='Invalid access token')
  return account

@users_router.get('/me')
async def get_current_user(account: Annotated[DbAccount, Depends(get_current_user_account)]) -> User:
  return User(email=account.email)
```

### Protected resource access route
```python
class Item(BaseModel):
  id: int
  description: str

items_router = APIRouter(prefix='/items', tags=['items'])

@items_router.get('')
@items_router.get('/', include_in_schema=False)
async def read_items(account: Annotated[DbAccount, Depends(get_current_user_account)]) -> List[Item]:
  return map(lambda it: Item(id=it.id, description=it.description), fetch_items())


@items_router.get('/{id}')
async def read_items(id: int, account: Annotated[DbAccount, Depends(get_current_user_account)]) -> Item:
  item = find_item_by_id(id)
  if not item:
    raise HTTPException(status_code=status.HTTP_404_NOT_FOUND, detail='Item not found')
  return Item(id=item.id, description=item.description)
```

### Parse JWT access token
```python
def parse_access_token(access_token: str) -> TokenData | None:
  try:
    claims = jwt.decode(access_token, key=token_secret_key, algorithms=[token_algorithm])
    return TokenData(email=claims.get('sub'), expires=datetime.utcfromtimestamp(claims.get('exp')))
  except:
    return None
```

### Fake items database
```python
@dataclass
class DbItem:
  id: int
  description: str

def find_item_by_id(id: int) -> DbItem | None:
  return next((it for it in fetch_items() if it.id == id), None)

def fetch_items() -> Iterator[DbItem]:
  yield DbItem(id=1, description='item 1 description...')
  yield DbItem(id=2, description='item 2 description...')
  yield DbItem(id=3, description='item 3 description...')
```
