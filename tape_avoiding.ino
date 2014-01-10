#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// INPUTS - pins 2-5
#define LS1 2  // Left sensor 1 (1st bar, outer)
#define LS2 3  // Left sensor 2 (1st bar, inner)
#define RS1 4  // Right sensor 1 (1st bar, outer)
#define RS2 5  // Right sensor 2 (1st bar, inner)
#define CS A0 // Color sensor (2nd bar)

// OUTPUTS - pins 6-11

#define analog_out_left 10 // port to set left motor speed through CA module
#define analog_out_right 11 // port to set right motor speed through CA module

// arrangement of sensor[] array : [LS1, LS2, RS2, RS1] (like the sensor bar as viewed from above)
int sensor[4] = {0, 0, 0, 0}; 

int car_speed = 130; // out of 255
boolean stop = false;

void setup() {

  Serial.begin(9600); // set up debugging console

  int i; // pin reference

  for(i=2; i < 6; i++) // iterate from pins 2 - 5
  {
    pinMode(i, INPUT); // sets the digital pin as input
  }
  for(i=6; i < 12; i++)
  {
    pinMode(i, OUTPUT); // sets the digital pin as output
  }

}

// method to turn on/off right motor
void right_motor(int i) {

  if(i==0) {
    analogWrite(analog_out_right, 0);
  }

  else if(i==1) {
    analogWrite(analog_out_right, car_speed); 
  }

}

// method to turn on/off left motor
void left_motor(int i) {

  if(i==0) {
    analogWrite(analog_out_left, 0);
  }

  else if(i==1) {
    analogWrite(analog_out_left, car_speed); 
  }

}

void getSensorValues() {
  sensor[0] = digitalRead(LS1);
  sensor[1] = digitalRead(LS2);
  sensor[2] = digitalRead(RS2);
  sensor[3] = digitalRead(RS1);
  //split_sensor = digitalRead(SS);
}

void loop() {
  
      getSensorValues();

  if(!stop) {

    // smooth curve - 0100 or 0010; curve_right means left sensor is on and vice versa 
    boolean curve_right = !sensor[0] && !sensor[1] && sensor[2] && !sensor[3];
    boolean curve_left = !sensor[0] && sensor[1] && !sensor[2] && !sensor[3];

    // right angle: 1100 or 0011
    boolean corner_right = !sensor[0] && !sensor[1] && sensor[2] && sensor[3];
    boolean corner_left = sensor[0] && sensor[1] && !sensor[2] && !sensor[3];

    // stop the car if it sees all white
    boolean stop = sensor[0] && sensor[1] && sensor[2] && sensor[3];
    
    // 0110
    boolean split = !sensor[0] && sensor[1] && sensor[2] && !sensor[3]; 

    /* nothing should be executed anymore if stop=true is encountered - therefore the code is set to execute only if the car is not stopped */

    // turns should be more aggressive - car speed is increased from 130 to 150

  if(split && !corner_left && !corner_right) {
   // delay(2000);
      // check color value
      // color detection analog(100<x<200) = gray; 0110 = white
    int color_val = analogRead(CS);//TODO assign bools for left/right gray/white
    if(color_val > 90 && color_val < 200) {
        //gray
        // turn right and go straight until back on single curve
        right_motor(0);
        left_motor(1);
        delay(600);
        do {
          right_motor(1);
          left_motor(1);
        } while(!sensor[0] && (!sensor[1] || !sensor[2]) && !sensor[3]);
        right_motor(0);
        left_motor(0);
    }
    
    else if(color_val > 400) {
     // white 
     left_motor(0);
      right_motor(1);
      delay(500);
      do {
        right_motor(1);
        left_motor(1);
      } while(!sensor[0] && (!sensor[1] || !sensor[2]) && !sensor[3]);
    }
      
    }


    // smooth turns
    if(curve_right) {
      car_speed=150;
      // do tape_avoider(right) while curve_right remains true 
      tape_avoider(1);
    }

    else if(curve_left) {
      car_speed=150;
      // do tape_avoider(left) while curve_left remains true 
      tape_avoider(0);
    }  

    //corner turns
    else if(corner_right) {
      car_speed=150;
      // do corner turn towards the right 
      corner_turn(1);
      car_speed = 110;
    }

    else if(corner_left) {
      car_speed=150;
      // do corner turn towards the left 
      corner_turn(0);
            car_speed = 110;

    }

    else {
      // in the rare case that the car is perfectly lined up with the tape - take advantage by turning on both motors
      right_motor(1);
      left_motor(1); 
    }

  }

  else if(stop) {
    right_motor(0);
    left_motor(0);
    stop = true; // ensure the car never runs again
  }

}

void corner_turn(int direction) {
  if(direction ==0) {
    // turn left
    while((sensor[0] || sensor[1]) || !(sensor[2] || sensor[3])) {
      getSensorValues();
      right_motor(1);
      left_motor(0); 
      if(sensor[3]) {// if sensor on left side touches tape, correct car direction
        // turn correction: turn on motor from opp side for 0.25s
        right_motor(0);
        left_motor(1);
        delay(250);
        left_motor(0);
        break;
      }
    }
  }

  else if(direction ==1) {
    // turn right
    while((sensor[2] || sensor[3]) || !(sensor[0] || sensor[1])) {
      getSensorValues();
      right_motor(0);
      left_motor(1); 
      if(sensor[1]) { // if sensor on right side touches tape, correct car direction
        // turn correction: turn on motor from opp side for 0.25s
        left_motor(0);
        right_motor(1);
        delay(250);
        right_motor(0);
        break;      
      }
    }
  } 
}

void tape_avoider(int direction) {
  if(direction ==0) {
    // curve_left
    right_motor(1);
    left_motor(0);
  }

  else if(direction ==1) {
    right_motor(0);
    left_motor(1); 
  }
}	
