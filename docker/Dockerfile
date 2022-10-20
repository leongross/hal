FROM ubuntu:20.04

RUN echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections
RUN  apt-get update &&  apt-get install -y build-essential verilator \
        lsb-release git cmake pkgconf libboost-all-dev qtbase5-dev \
        libpython3-dev ccache autoconf autotools-dev libsodium-dev \
        libqt5svg5-dev libqt5svg5* ninja-build lcov gcovr python3-sphinx \
        doxygen python3-sphinx-rtd-theme python3-jedi python3-pip \
        pybind11-dev python3-pybind11 rapidjson-dev libspdlog-dev libz3-dev libreadline-dev \
        graphviz libomp-dev libsuitesparse-dev \
        libigraph-dev
RUN useradd -ms /bin/bash hal
WORKDIR /home/hal
USER hal
RUN git clone https://github.com/emsec/hal.git
WORKDIR /home/hal/hal
RUN mkdir build
WORKDIR /home/hal/hal/build
RUN cmake ..
RUN make -j $(nproc)
CMD ./bin/hal -g
