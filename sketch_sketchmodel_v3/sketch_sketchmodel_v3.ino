#include <Servo.h>
#include <Math.h>
#include <TinyGPS.h>

TinyGPS gps;   // create gps object to pull data from the gps unit

#define RXPIN 3   // define pin on which to receive serial data from gps
#define TXPIN 2   // define pin on which to transmit serial data to gps
SoftwareSerial nss(RXPIN, TXPIN);  // set up new soft serial connection

Servo myservo;  // create servo object to control a servo

float destination[2] = {48.85837, 2.294481};  // lat/long of paris

void setup()  // set up servo
{
  myservo.attach(4); // servo is in output pin 4
  Serial.begin(9600); // set baud rate
}

void loop() // main loop
{
  float theta;
  float latitude, longitude;

  while (nss.available())
  {
    int c = nss.read();
    if (gps.encode(c)) //when encode() returns true, a valid sentence 
                      //has just changed the gps object's internal state
    {
      // process new gps info here
    
      gps.f_get_position(&latitude, &longitude); // get lat long from gps module

      // math 
      int tc1 = atan2(sin(destination[1]-longitude)*cos(destination[0]),
                  cos(latitude)*sin(destination[0])-sin(latitude)
                  *cos(destination[0])*cos(destination[1]-longitude));

      tc1 = (int) (((tc1 % 2*M_PI) * 360/M_PI)%360);

      // calculate difference
      servoPosition = (int) (360 - (theta - tc1))

      myservo.write(360 - servoPosition);
    }
  }
  
}


