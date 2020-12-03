# Homemade CyberTruck

![CyberTruck Scene 1](https://github.com/SeymurD/homemade-cybertruck/blob/master/media/scenic_cybertruck_1.png)

### Introduction

The inspiration to build my own small replica of the Cybertruck was fueled by the shear want of trying to create something myself from the ground up and test myself to make something fun and exciting. Having worked on robots for a while in University project teams, I wanted to invest some money into my own maker gear such as a 3D printer, soldering stations, oscilloscope and all the small electronics to make this machine come to life.

The following will be a log of how my work on this progresses on starting from the model crafted in Fusion 360. As of now, I have probably spent around 1 1/2 months sizing, sketching and modelling the truck to be as close in proportion and stance as the real world beast. The greatest challenge in designing the shell of this vehicle is working both with the restrcition on my 3D printer in terms of accuracy, build volume, and part complexity such that I don't have overhangs present in every part. The chassis of was built from aluminium U-Channel beam for structural rigidity and stretches 24" in length. The length is actually closer to 28" when taking into the account the 3D printed rear end. The images below capture the overall design and placement of the interals using some of the rendering tools provided in Fusion360.

| | |
|:-------------------------:|:-------------------------:|
|<img width="1604" height="250" alt="cybertruck scene 1" src="https://github.com/SeymurD/homemade-cybertruck/blob/master/media/scenic_cybertruck_1.png">  Three-Quarter Side View |  <img width="1604" height="250" alt="cybertruck scene 2" src="https://github.com/SeymurD/homemade-cybertruck/blob/master/media/scenic_cybertruck_2.png"> Elevated Side View|
|<img width="1604" height="250" alt="cybertruck scene 3" src="https://github.com/SeymurD/homemade-cybertruck/blob/master/media/scenic_cybertruck_3.png"> Front View |  <img width="1604" height="250" alt="cybertruck scene 4" src="https://github.com/SeymurD/homemade-cybertruck/blob/master/media/scenic_cybertruck_4.png"> Rear View|

From the Front View image, you may be wondering what some of the little holes are within the front bumper. The smaller holes on the silver portion are angled at 40-degrees and contain RaspberryPi Cam V2 with the base lens and serve as additional sources of imaging data that I can sync with the main FLIR BlackFlyS camera mounted high in the center of the vehicle. Additionally an Intel RealSense depth camera will be mounted near the dash space of the vehicle to provide depth information and once again will be synced with the main camera such that an image from the main camera can have a corresponding depth parameter.

On the lower black front bumper are 6 large holes that correspond to 3 ultrasonic sensors that most electronic hobbyists are familiar with: the HC-SR04 ultrasonic sensor!

The sensors are fitted snug inside the front section by having created a small pocket for each sensor which can be easily accessed in the wheel-well of the vehicle. A solution for keeping the internal wiring clean was also provided by having 3D printed channels run seamlessly within the body of the vehicle and taking advantage of the fact that the core of the vehicle is an aluminium channel.

| | |
|:-------------------------:|:-------------------------:|
|<img width="1604" height="250" alt="cybertruck picam 1" src="https://github.com/SeymurD/homemade-cybertruck/blob/master/media/sectionview_picam.JPG">  Cross-Sectional View |  <img width="1604" height="250" alt="cybertruck picam 2" src="https://github.com/SeymurD/homemade-cybertruck/blob/master/media/sectionview_picam2.JPG"> Skeleton/Wireframe View|
