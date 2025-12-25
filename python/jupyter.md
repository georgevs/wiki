# Setup Jupiter
### Install docker
```bash
sudo apt install docker.io docker-buildx
```
### Build Jupyter docker image
```bash
DOCKER_BUILDKIT=1 \
PASSWD=$(read -s -p 'Password:' PASSWD ; echo "$USER:$PASSWD") \
docker image build --no-cache --secret id=PASSWD --tag dev-jupyter - << EOF
  FROM ubuntu
  RUN --mount=type=secret,id=PASSWD \
    apt-get update && apt-get install -y sudo tmux vim curl python3 python3-pip && \
    useradd -m -s /bin/bash -G sudo $USER && \
    cat /run/secrets/PASSWD | chpasswd
    USER $USER
    ENV PATH "\$PATH:/home/$USER/.local/bin"
    RUN pip3 install notebook
EOF
```
### Create docker network
```bash
docker network create \
  -d bridge \
  --subnet 172.20.0.0/16 \
  --gateway 172.20.0.1 \
  bridge-dev
```
### Run Jupyter container
```bash
docker container run -it \
  --name dev-jupyter \
  --network bridge-dev \
  --ip 172.20.0.210 \
  --volume "/home/$USER/ws/DEV:/home/$USER/ws/DEV" \
  --volume "/home/$USER/ws/NOTES/wiki:/home/$USER/ws/NOTES/wiki" \
  --rm -d dev-jupyter
```
### Start Jupyter server
```bash
docker container exec -it --user $USER dev-jupyter \
  jupyter notebook --ip 0.0.0.0 --port 8888 --no-browser
```
