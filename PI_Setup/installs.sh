#!/bin/bash

#this script installs all external packages that were used
#the used PI was a 3+ model running Ubiquity ROSberryPi
#on Ubuntu 20.04

#run chmod +x installs.sh to make this script executable

sudo apt-get update
sudo apt-get upgrade

##THIS PACKAGE DOES NOT SEEM TO WORK ON ARM PROCESSORS
#install ardrone-autonomy ros package form source
#  cd ..
#  git clone https://github.com/AutonomyLab/ardrone_autonomy.git
#  cd ..
#  rosdep install --from-paths src -i
#  catkin_make
#package description can be found at https://ardrone-autonomy.readthedocs.io/en/latest/index.html