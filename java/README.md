# Java

## References
[Oracle: Java 17](https://docs.oracle.com/en/java/javase/17/docs/api/java.base/java/util/Collection.html)  
[Jenkov: Java tutorial](https://jenkov.com/tutorials/java/index.html)  
[Java playground](https://dev.java/playground)  
[Maven central](https://mvnrepository.com/)  

## Java Collections
<img src="./images/java-collections.drawio.svg" />

## Java IO
<img src="./images/java-io.drawio.svg" />

## Dependencies discovery
Search [Spring framework](https://docs.spring.io/spring-framework/docs/current/javadoc-api) documentation for the target class.  Google the target class package related to `mvncentral`.  The versioned links in the [Maven repository](https://mvnrepository.com/artifact/org.springframework) will have code snippet to use with Maven, Gradle, etc.


## Setup Java environment with Docker

### Install docker
```bash
sudo apt install docker.io docker-buildx
```

### Create a dev network
```bash
docker network create \
  -d bridge \
  --subnet 172.20.0.0/16 \
  --gateway 172.20.0.1 \
  bridge-dev
```

### Create a dev Java image
```bash
DOCKER_BUILDKIT=1 \
PASSWD=$(read -s -p 'Password:' PASSWD ; echo "$USER:$PASSWD") \
docker image build --no-cache --force-rm --secret id=PASSWD --tag dev-java - << EOF
  FROM ubuntu
  SHELL ["/bin/bash", "-c"]
  RUN --mount=type=secret,id=PASSWD \
    apt-get update && \
    apt-get install -y sudo ssh tmux vim curl less openjdk-17-jdk-headless && \
    useradd -m -s /bin/bash -G sudo $USER && \
    cat /run/secrets/PASSWD | chpasswd
  USER $USER
EOF
```

### Run a (persistent) dev Java container
```bash
docker container run -it \
  --name dev-java \
  --network bridge-dev \
  --ip 172.20.0.212 \
  --volume "/home/$USER/ws/DEV:/home/$USER/ws/DEV" \
  --volume "/home/$USER/ws/NOTES/wiki:/home/$USER/ws/NOTES/wiki" \
  -d dev-java
```

### Connect VScode directly to the dev Java container
Optionally, VScode can be remote connected to the docker dev Java container directly, and the `Java Extension Pack` by Microsoft can be loaded for more immersive coding and debugging experience.

This requires a SSH server to be launched inside the dev Java container.
```
docker container exec -it dev-java bash -c 'sudo service ssh start'
```

The docker dev Java container can then be remote connected from VScode with the `Remote ssh connect to host` command, and the `ssh 172.20.0.212` connect string. 
