#include <Servo.h>
#include <Math.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>


#define RXPIN 2
#define TXPIN 3

#define TERMBAUD  115200

// Set this value equal to the baud rate of your GPS
#define GPSBAUD  4800

TinyGPS gps;   // create gps object to pull data from the gps unit

SoftwareSerial uart_gps(RXPIN, TXPIN);  // set up new soft serial connection

Servo myservo;  // create servo object to control a servo

float destination[2] = {48.85837, 2.294481};  // lat/long of paris

void getgps(TinyGPS &gps);

void setup()  // set up servo
{
  myservo.attach(4); // servo is in output pin 4
   // Sets baud rate of your terminal program
  Serial.begin(TERMBAUD);
  // Sets baud rate of your GPS
  uart_gps.begin(GPSBAUD);

  Serial.println("");
  Serial.println("GPS Shield QuickStart Example Sketch v12");
  Serial.println("       ...waiting for lock...           ");
  Serial.println("");
}

void loop() // main loop
{
  float theta = 0;
  float latitude, longitude;

  while (uart_gps.available())
  {
    int c = uart_gps.read();
    if (gps.encode(c)) //when encode() returns true, a valid sentence
                      //has just changed the gps object's internal state
    {
      getgps(gps);
      // process new gps info here

      gps.f_get_position(&latitude, &longitude); // get lat long from gps module
      
      Serial.print("Lat/Long: ");
      Serial.print(latitude,5);
      Serial.print(", ");
      Serial.println(longitude,5);
          

      // math
      int tc1 = atan2(sin(destination[1]-longitude)*cos(destination[0]),
                  cos(latitude)*sin(destination[0])-sin(latitude)
                  *cos(destination[0])*cos(destination[1]-longitude));
                  
      Serial.print("tc1: ");
      Serial.print(tc1);

      tc1 = (int)((tc1 % 2*M_PI) * 360/M_PI)%360;
      Serial.print("new tc1: ");
      Serial.print(tc1);

      // calculate difference
      int servoPosition = (int) (360 - (theta - tc1));

      Serial.print("servoPosition: ");
      Serial.print(servoPositon);
      
      myservo.write(360 - servoPosition);
      
    }
  }

}

// The getgps function will get and print the values we want.
void getgps(TinyGPS &gps)
{
  // To get all of the data into varialbes that you can use in your code,
  // all you need to do is define variables and query the object for the
  // data. To see the complete list of functions see keywords.txt file in
  // the TinyGPS and NewSoftSerial libs.

  // Define the variables that will be used
  float latitude, longitude;
  // Then call this function
  gps.f_get_position(&latitude, &longitude);
  // You can now print variables latitude and longitude
//  Serial.print("Lat/Long: ");
//  Serial.print(latitude,5);
//  Serial.print(", ");
//  Serial.println(longitude,5);

  // Same goes for date and time
  int year;
  byte month, day, hour, minute, second, hundredths;
  gps.crack_datetime(&year,&month,&day,&hour,&minute,&second,&hundredths);
  // Print data and time
//  Serial.print("Date: "); Serial.print(month, DEC); Serial.print("/");
//  Serial.print(day, DEC); Serial.print("/"); Serial.print(year);
//  Serial.print("  Time: "); Serial.print(hour, DEC); Serial.print(":");
//  Serial.print(minute, DEC); Serial.print(":"); Serial.print(second, DEC);
//  Serial.print("."); Serial.println(hundredths, DEC);
  //Since month, day, hour, minute, second, and hundr

  // Here you can print the altitude and course values directly since
  // there is only one value for the function
//  Serial.print("Altitude (meters): "); Serial.println(gps.f_altitude());
//  // Same goes for course
//  Serial.print("Course (degrees): "); Serial.println(gps.f_course());
//  // And same goes for speed
//  Serial.print("Speed(kmph): "); Serial.println(gps.f_speed_kmph());
//  //Serial.println();

  // Here you can print statistics on the sentences.
  unsigned long chars;
  unsigned short sentences, failed_checksum;
  gps.stats(&chars, &sentences, &failed_checksum);
  //Serial.print("Failed Checksums: ");Serial.print(failed_checksum);
  //Serial.println(); Serial.println();

  // Here you can print the number of satellites in view
//  Serial.print("Satellites: ");
  Serial.println(gps.satellites());
}
