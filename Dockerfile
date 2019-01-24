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

ENV SIMBOTIC_GAMS=1
ENV AIRSIM_ROOT=$HOME/AirSim
ENV MPC_ROOT=$HOME/gams/MPC
ENV EIGEN_ROOT=$HOME/gams/eigen
ENV CAPNP_ROOT=$HOME/gams/capnproto
ENV MADARA_ROOT=$HOME/gams/madara
ENV GAMS_ROOT=$HOME/gams
ENV VREP_ROOT=
ENV CORES=4
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MADARA_ROOT/lib:$GAMS_ROOT/lib:$VREP_ROOT:$CAPNP_ROOT/c++/.libs
ENV PATH=$PATH:$MPC_ROOT:$VREP_ROOT:$CAPNP_ROOT/c++

CMD /bin/bash
