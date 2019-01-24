# docker build -t gams .
# docker run -ti -v $(pwd)/:/home/sim gams /bin/bash

FROM ubuntu:bionic

RUN apt-get update
RUN apt-get -y install build-essential
RUN apt-get -y install git-core
RUN apt-get -y install autoconf
RUN apt-get -y install sudo
RUN apt-get -y install wget
RUN apt-get -y install rsync

RUN adduser --disabled-password --gecos '' sim
RUN adduser sim sudo
RUN echo '%sudo ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers

USER sim
WORKDIR /home/sim

ENV HOME=/home/sim

ENV MPC_ROOT=$HOME/gams/MPC
ENV EIGEN_ROOT=$HOME/gams/eigen
ENV CAPNP_ROOT=$HOME/gams/capnproto
ENV GAMS_ROOT=$HOME/gams
ENV VREP_ROOT=
ENV CORES=4
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MADARA_ROOT/lib:$GAMS_ROOT/lib:$VREP_ROOT:$CAPNP_ROOT/c++/.libs
ENV PATH=$PATH:$MPC_ROOT:$VREP_ROOT:$CAPNP_ROOT/c++

# RUN git clone -b master --single-branch --depth 1 https://github.com/jredmondson/gams $GAMS_ROOT
# RUN git clone -b minimal --single-branch --depth 1 https://github.com/VertexStudio/AirSim.git

CMD /bin/bash

# RUN $GAMS_ROOT/scripts/linux/base_build.sh prereqs clang madara gams
