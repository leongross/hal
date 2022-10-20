# Dockerfile

Build the docker image from this local Dockerfile
```sh
$ docker build -t hal:gui .
```

Run the `./run.sh` script. This script will mount an X11 socket from the host system to the docker container.
This script requiers that you run Xorg.

```sh
$ ./run.sh
```
