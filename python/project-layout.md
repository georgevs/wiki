# Python project layout

### Source tree
```
data/
  ...
  __confidential/ ...
  
src/
  package1/ module1.py, ...
  package2/ module2.py, ...
  app1/
    package1/ module11.py, ...
    package3/ module3.py, ...
    app1.py
```

### Run app
```
PYTHONPATH+=./src python3 src/app1/app1.py data/__confidential/data.txt
```

### App
```python
import argparse
from package1.module1 import fn1
from package1.module11 import fn11
from package2.module2 import fn2
from package3.module3 import fn3

def main(file_path):
  print(digest_pdf(file_path))

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('file_path', type=str, help='file path')
    args = parser.parse_args()
    main(**vars(args))
```

### App module11
```python
from package1.module1 import fn1
from package2.module2 import fn2
from package3.module3 import fn3

fn11 = ...
```
