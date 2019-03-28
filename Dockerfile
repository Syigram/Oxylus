# FROM ubuntu:bionic
FROM debian:stretch
LABEL maintainer="ale2793@hotmail.com"

USER root

RUN apt-get update
RUN apt-get install -y build-essential g++ gcc make git cmake pkg-config
RUN apt-get install -y libopencv-dev python-opencv
RUN apt-get install -y mpich
RUN apt-get install -y libboost1.62-all-dev
RUN apt-get install -y openmpi-bin


RUN useradd -ms /bin/bash agonzalez
#RUN echo "agonzalez ALL=(ALL:ALL) ALL" >> /etc/sudoers

#USER agonzalez

WORKDIR /home/agonzalez/oxylus
COPY . /home/agonzalez/oxylus

RUN cmake .
RUN make


USER agonzalez

#CMD ["./bin/oxylus"]
CMD ["mpirun -n 4 ./bin/oxylus"]

