#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 

String readString="";// a maximum of eight servo objects can be created 
int pos=1;  

void setup() {
  myservo.attach(4);
  Serial.begin(9600);
}

void loop() {
  
  
  if (Serial.available() > 2) {
    while (Serial.read() != 'a') {
      delay(3);
    }
    while (Serial.peek() != 'z') {
      char c = Serial.read();  //gets one byte from serial buffer
      readString += c; //makes the string readString
  
    }
  
    Serial.println(360-readString.toInt());
    myservo.write(360 - readString.toInt());
   
    readString="";
  }
}


