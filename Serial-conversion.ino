
/*
Author:  TWC
Date:    Nov. 2014
Version: 0.1.2

Note:
Default full scale <acceleration, magnetic> settings on LSM303D is <+/- 2 g, +/- gauss>. 
To convert to m/s^2, the acceleration raw output needs to be divided by (16*1000). 
To convert to gauss, the magnetic output needs to multiplied by 0.16*1e-3.
See LSM303D datasheet (p.11) for more detail.

The Sharp IR sensor (GP2Y0A02YK) detection range is from 20cm to 150cm

The Processing file "P_Serial_conversion" plots acceleration (m/s^2) on all the axes
*/

#include <Wire.h>
#include <LSM303.h>
#include <SoftwareSerial.h>
#include <SharpIR.h>

// define parameters for IR sensor
#define pinLED 13
#define distMin 30 // minimum obstical detection distance: 30 cm

// LSM303 digital compass object
LSM303 compass;

// Initialization
char report[80];
int  idx = 1; //loop index
int  idxMAX = 10; //number of measurements required for calculating the mean
int  delayTime = 10; // time-loop delay in ms
float sumACCE_X = 0.0;
float sumACCE_Y = 0.0;
float sumACCE_Z = 1.0;
/* initial magnetic fileds numbers are derived from running sketch located in
   LSM303/Serial folder */
float sumMAGN_X = -3100;
float sumMAGN_Y = -720;
float sumMAGN_Z = 22150;
float sumHEADING = 0.0;
float convACCE = 1.0/(16.0*1.0e+3); // unit: m/s^2; full range: +/- 2g
float convMAGN = 0.16*1e-3; // unit: gauss; full range: +/- 4 gauss
// IR sensor initial parameters
int pinIR = 1; //analog pin #1 for Sharp IR sensor
int startMillis = 0;
unsigned long currentMillis = 0;
long interval = 1000; // in milliseconds
SharpIR sharp(pinIR, 50, 93, 20150); // IR sensor object; see document for more detail
float sumDIST = 30.0; // set the initial detection distance at 30 cm

void setup()
{
  Serial.begin(9600);
  pinMode(pinLED, OUTPUT);
  Wire.begin();
  compass.init();
  compass.enableDefault();
  //Calibrating the LSM303 compass
  compass.m_min = (LSM303::vector<int16_t>) {-4657, -3834, +21842};
  compass.m_max = (LSM303::vector<int16_t>){-960, +1722, +27255};
}

void loop()
{
  compass.read();
  if (idx <= idxMAX) {
    sumACCE_X = sumACCE_X + compass.a.x*convACCE; // (16.0*1000)^-1 -> m/s^2
    sumACCE_Y = sumACCE_Y + compass.a.y*convACCE; // (16.0*1000)^-1 -> m/s^2
    sumACCE_Z = sumACCE_Z + compass.a.z*convACCE; // (16.0*1000)^-1 -> m/s^2
    sumMAGN_X = sumMAGN_X + compass.m.x*convMAGN; // (0.16*1e-3) -> gauss
    sumMAGN_Y = sumMAGN_Y + compass.m.x*convMAGN; // (0.16*1e-3) -> gauss
    sumMAGN_Z = sumMAGN_Z + compass.m.x*convMAGN; // (0.16*1e-3) -> gauss
    sumHEADING= sumHEADING+ compass.heading();
    sumDIST   = sumDIST   + sharp.distance();
    ++idx;
  } else {
    //Serial.print("X-acc: ");
    Serial.print(sumACCE_X/idxMAX, 3), Serial.print('\t');
    //Serial.print("\tY-acc: ");
    Serial.print(sumACCE_Y/idxMAX, 3), Serial.print('\t');
    //Serial.print("\tZ-acc: ");
    Serial.print(sumACCE_Z/idxMAX, 3), Serial.print('\t');    
    //Serial.print(sumMAGN_X/idxMAX, 3), Serial.print('\t');
    //Serial.print(sumMAGN_Y/idxMAX, 3), Serial.print('\t');
    //Serial.print(sumMAGN_Z/idxMAX, 3), Serial.print('\t');
    Serial.print(sumHEADING/idxMAX, 3), Serial.print('\t');
    Serial.println(sumDIST/idxMAX, 3); 
    idx = 0;
    sumACCE_X = 0;
    sumACCE_Y = 0;
    sumACCE_Z = 1.0;
    sumMAGN_X = -3100;
    sumMAGN_Y = -720;
    sumMAGN_Z = 22150;
    sumHEADING= 0.0;
    sumDIST   = 30.0;
  }
  
/*  snprintf(report, sizeof(report), "A: %6d %6d %6d    M: %6d %6d %6d",
    compass.a.x/(16), compass.a.y/(16), compass.a.z/(16),
    compass.m.x, compass.m.y, compass.m.z);
  Serial.println(report); */

  time_loop(delayTime);
}

void time_loop(int delayMilliSecond) {
  unsigned long startTime;
  unsigned long endTime;
  
  startTime = millis();
  while(millis() - startTime < delayMilliSecond) {
    // Do nothing.
  }
}
