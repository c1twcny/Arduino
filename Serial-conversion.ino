
/*
Author:  TWC
Date:    Nov. 2014
Version: 0.1.2

Note:
LSM303D:
Default full scale <acceleration, magnetic> settings on LSM303D is <+/- 2 g, +/- gauss>. 
To convert to m/s^2, the acceleration raw output needs to be divided by (16*1000). 
To convert to gauss, the magnetic output needs to multiplied by 0.16*1e-3.
See LSM303D datasheet (p.11) for more detail.

L3GD20H:
Measurement range (dps)    Sensitivity (mdps/digit
+/- 245                    8.75
+/- 500                    17.50
+/- 2000                   70.00

call writeReg() function to change the measurement rante 7 sensitivity of gyro;
Example 1:
gyro.writeReg(L3G_CTRL_REG4, 0b00110000) -> change the setting to +/- 2000 dps.
0b00000000 -> +/- 245 dps; default value
0b00010000 -> +/- 500 dps
0b00100000 -> +/- 2000 dps
0b00110000 -> +/- 2000 dps

Example 2:
If you are running at the scale of +/- 2000 dps, then a reading of 100 from the
sensor would indicates (70.00*1e-3) * 100 degrees/second rotation.
If you are running at the scale of +/- 500 dps, the a reading of 100 would indicates
(17.50*1e-3) * 100 degrees/second rotation.

IR sensor:
The Sharp IR sensor (GP2Y0A02YK) detection range is from 20cm to 150cm

The Processing file "P_Serial_conversion" plots acceleration (m/s^2) on all the axes
*/

#include <Wire.h>
#include <LSM303.h>
#include <L3G.h>
#include <SoftwareSerial.h>
#include <SharpIR.h>

// define parameters for IR sensor
#define pinLED 13
#define distMin 30 // minimum obstical detection distance: 30 cm

// LSM303 digital compass object
LSM303 compass;
L3G gyro;

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
//gyro parameters
int sampleNum = 500;
int dc_offset= 0;
float sumGYRO_X = 0;
float sumGYRO_Y = 0;
float sumGYRO_Z = 0;

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
  
  if (!gyro.init()) {
    Serial.println("Failed to autodetect gyro type!");
    while(1);
  }
  gyro.enableDefault();
  gyro.writeReg(L3G_CTRL_REG4, 0b00000000);
  for (int n=0; n < sampleNum; n++) {
    gyro.read();
    dc_offset += (int)gyro.g.z;
  }
  dc_offset = dc_offset/sampleNum;
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
    sumGYRO_X = sumGYRO_X + gyro.g.x;
    sumGYRO_Y = sumGYRO_Y + gyro.g.y;
    sumGYRO_Z = sumGYRO_Z + gyro.g.z;
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
    Serial.print(sumDIST/idxMAX, 3), Serial.print('\t');
    Serial.print(sumGYRO_X/idxMAX, 3), Serial.print('\t');
    Serial.print(sumGYRO_Y/idxMAX, 3), Serial.print('\t');
    Serial.println(sumGYRO_Z/idxMAX, 3); 
    idx = 0;
    sumACCE_X = 0;
    sumACCE_Y = 0;
    sumACCE_Z = 1.0;
    sumMAGN_X = -3100;
    sumMAGN_Y = -720;
    sumMAGN_Z = 22150;
    sumHEADING= 0.0;
    sumDIST   = 30.0;
    sumGYRO_X = 0;
    sumGYRO_Y = 0;
    sumGYRO_Z = 0;
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
