
/*
Author:  TWC
Date:    Nov. 2014
Version: 0.1.0

Description:
Default full scale acceleration settings on LSM303D is +/- 2 g. To convert to m/s^2, the
raw output needs to be divided by (16*1000). See LSM303D datasheet (p.11) for more 
detail.

The Processing file "P_Serial_conversion" plots acceleration (m/s^2) on all the axes

*/

#include <Wire.h>
#include <LSM303.h>
#include <SoftwareSerial.h>

LSM303 compass;

char report[80];
int  idx = 1; //loop index
int  idxMAX = 10; //number of measurements required for calculating the mean
int  delayTime = 10; // time-loop delay in ms
float sumX = 0.0;
float sumY = 0.0;
float sumZ = 1.0;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  compass.init();
  compass.enableDefault();
}

void loop()
{
  compass.read();
  if (idx <= idxMAX) {
    sumX = sumX + compass.a.x/(16.0*1000); // 16.0*1000 -> m/s^2
    sumY = sumY + compass.a.y/(16.0*1000); // 16.0*1000 -> m/s^2
    sumZ = sumZ + compass.a.z/(16.0*1000); // 16.0*1000 -> m/s^2
    ++idx;
  } else {
    //Serial.print("X-acc: ");
    Serial.print(sumX/idxMAX, 3);
    Serial.print("\t");
    //Serial.print("\tY-acc: ");
    Serial.print(sumY/idxMAX, 3);
    Serial.print("\t");
    //Serial.print("\tZ-acc: ");
    Serial.println(sumZ/idxMAX, 3);
    idx = 0;
    sumX = 0;
    sumY = 0;
    sumZ = 1.0;
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
