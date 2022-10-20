#!/usr/bin/env bash

HAL_X_SOCKET=/tmp/.X11-unix/
HAL_UID=$(id --user)
HAL_GID=$(id --group)
HAL_CONTAINER="hal:gui"

if ! xset q >/dev/null;then
    echo "No Xserver is running on your machine. Exit."
    exit
fi

if [[ ! -d "$HAL_X_SOCKET" ]];then
    echo "Could not find X socket to mount. Exit."
    exit
fi

docker run -e DISPLAY \
    -v "$HAL_X_SOCKET":"$HAL_X_SOCKET" \
    -v "$HOME/.Xauthority:/root/.Xauthority:rw" \
    --user="$HAL_UID":"$HAL_GID" \
    --net=host \
    "$HAL_CONTAINER"
