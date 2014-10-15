#include <Servo.h>
#include <Math.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>


#define RXPIN 2
#define TXPIN 3

#define TERMBAUD  115200

// Set this value equal to the baud rate of your GPS
#define GPSBAUD  4800

TinyGPS gps;   // create gps object to pull data from the gps unit

SoftwareSerial uart_gps(RXPIN, TXPIN);  // set up new soft serial connection

Servo myservo;  // create servo object to control a servo

float destination[2] = {48.85837, 2.294481};  // lat/long of paris

Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

void getgps(TinyGPS &gps);

void setup()  // set up servo
{
  Serial.begin(9600);
   /* Initialise the sensor */
  if(!mag.begin())
  {
    /* There was a problem detecting the HMC5883 ... check your connections */
    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
    while(1);
  }
  displaySensorDetails();
  
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
  float headingDegrees;
  float latitude, longitude;

  while (uart_gps.available())
  {
    int c = uart_gps.read();
    if (gps.encode(c)) //when encode() returns true, a valid sentence
                      //has just changed the gps object's internal state
    {
       /* Get a new sensor event */ 
        sensors_event_t event; 
        mag.getEvent(&event);
       
        /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
        Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("  ");
        Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
        Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  ");Serial.println("uT");
      
        // Hold the module so that Z is pointing 'up' and you can measure the heading with x&y
        // Calculate heading when the magnetometer is level, then correct for signs of axis.
        float heading = atan2(event.magnetic.x, -event.magnetic.z);
        
        // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
        // Find yours here: http://www.magnetic-declination.com/
        // Mine is: -13* 2' W, which is ~13 Degrees, or (which we need) 0.22 radians
        // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
        float declinationAngle = 0.24;
        heading += declinationAngle;
        
        // Correct for when signs are reversed.
        if(heading < 0)
          heading += 2*PI;
          
        // Check for wrap due to addition of declination.
        if(heading > 2*PI)
          heading -= 2*PI;
         
        // Convert radians to degrees for readability.
        float headingDegrees = heading * 180/M_PI; 
        
        Serial.print("Heading (degrees): "); Serial.println(headingDegrees);
        
        delay(500);
      
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
      Serial.println(tc1);

      tc1 = (int)((tc1 % 2*M_PI) * 360/M_PI)%360;
      Serial.print("new tc1: ");
      Serial.println(tc1);

      // calculate difference
      int servoPosition = (int) (360 - (headingDegrees - tc1));

      Serial.print("servoPosition: ");
      Serial.println(servoPosition);
      
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

void displaySensorDetails(void)
{
  sensor_t sensor;
  mag.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" uT");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" uT");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" uT");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}
