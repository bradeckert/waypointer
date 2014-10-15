#include <SoftwareSerial.h>
#include <TinyGPS.h>

#define RXPIN 2   
#define TXPIN 3
#define GPSBAUD 4800

TinyGPS gps;
SoftwareSerial nss(RXPIN, TXPIN);

void getgps(TinyGPS &gps)
{
   float latitude, longitude;
gps.f_get_position(&latitude, &longitude);
Serial.println(latitude, 5);
Serial.println(longitude, 5);
}

void setup()
{

Serial.begin(115200);
uart_gps.begin(GPSBAUD);
}

void loop()
{
while(uart_gps.available()) 
{
      int c = uart_gps.read();    
      if(gps.encode(c)) 
      {   
        getgps(gps);         
      }
 }
}

