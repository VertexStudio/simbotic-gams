FROM ubuntu:bionic

RUN apt-get update
RUN apt-get -y install build-essential
RUN apt-get -y install clang-5.0 clang++-5.0
RUN apt-get -y install git-core
RUN apt-get -y install autoconf
RUN apt-get -y install sudo
RUN apt-get -y install wget
RUN apt-get -y install rsync
RUN apt-get -y install libboost-all-dev
RUN apt-get -y install xterm
RUN apt-get -y install unzip
RUN apt-get -y install lsb-release
RUN apt-get -y install cmake
RUN apt-get -y install gdb

RUN useradd sim -u 1001 && echo "sim:sim" | chpasswd && adduser sim sudo
RUN echo '%sudo ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers

ENV SIM_ROOT=/sim

USER sim
WORKDIR $SIM_ROOT

ENV AIRSIM_ROOT=$SIM_ROOT/AirSim
ENV MPC_ROOT=$SIM_ROOT/gams/MPC
ENV EIGEN_ROOT=$SIM_ROOT/gams/eigen
ENV CAPNP_ROOT=$SIM_ROOT/gams/capnproto
ENV MADARA_ROOT=$SIM_ROOT/gams/madara
ENV GAMS_ROOT=$SIM_ROOT/gams
ENV VREP_ROOT=
ENV CORES=4
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MADARA_ROOT/lib:$GAMS_ROOT/lib:$VREP_ROOT:$CAPNP_ROOT/c++/.libs
ENV PATH=$PATH:$MPC_ROOT:$VREP_ROOT:$CAPNP_ROOT/c++

CMD /bin/bash
