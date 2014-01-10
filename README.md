ECE 110 - FALL 2012
FINAL PROJECT
=================

Bottom-up design and implementation of a navigation circuitry using Arduino to guide a motorized car around a complex tape-lined track.

Description
=================
Uses a modified tape-avoiding scheme. The first sensor bar has two main (inner) sensors and two outer sensors to handle special types of turns. The second sensor bar has a color sensor. 

On a corner turn, the outer sensors detect and calibrate the wide swing of the vehicle. On zig-zag turns, the approach is almost identical. On splits (forks), the two inner sensors detect the fork and the color sensor reports what kind of split it is (gray/white).

To stop the car, the program waits until it sees the appropriate values from all sensors at once, as it will be over a large white block.

This flowchart details the order of execution starting from the navigation sensors:
![alt tag](http://i.imgur.com/etxEUjK.png)

Usage
=================
To run, ensure the arduino configuration matches the one shown below:
![alt tag](http://i.imgur.com/733JukY.png)

And the sensor configuration is as follows:

![alt tag](http://i.imgur.com/bwRUAT0.png)

Open tape_avoiding.ino in the Arduino IDE and hit 'Verify/Compile'

