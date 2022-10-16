#!/usr/bin/env bash

set -x
docker run -e DISPLAY \
    -v /tmp/.X11-unix:/tmp/.X11-unix:ro \
    -v "$HOME/.Xauthority:/root/.Xauthority:rw" \
    --user="$(id --user):$(id --group)" \
    --net=host \
    hal:custom
