#include <Servo.h>
#include <Math.h>

Servo myservo;  // create servo object to control a servo


// lat/long of paris
float destination[2] = {48.85837, 2.294481};

// setup servo
void setup() {
  myservo.attach(4); //output pin 4
  Serial.begin(9600); //set baud rate
}


//main loop
void loop() {
  float theta;
  float latitude, longitude;

  //get lat long from gps module
  gps.f_get_position(&latitude, &longitude);


  // math
  int tc1 = atan2(sin(destination[1]-longitude)*cos(destination[0]),
                  cos(latitude) * sin(destination[0]) - sin(latitude)
                  * cos(destination[0]) * cos(destination[1]-longitude));

  tc1 = (int) (((tc1 % 2*M_PI) * 360/M_PI)%360);

  // calculate difference
  servoPosition = (int) (360 - (theta - tc1))

  myservo.write(360 - servoPosition);
}


