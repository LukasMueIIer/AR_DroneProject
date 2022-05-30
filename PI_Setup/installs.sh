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

#wiringPi is deprecated
#install wiringPi http://wiringpi.com
#sudo apt-get install wiringpi


#install pigpio libarry for gpio access 
# cd ..
# wget https://github.com/joan2937/pigpio/archive/master.zip
# unzip master.zip
# cd pigpio-master
# make
# sudo make install

#install wiring pi fork 
cd ..
git clone https://github.com/WiringPi/WiringPi.git
cd WiringPi
chmod +x build 
./build 
