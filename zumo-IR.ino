/* ************************************************************
 *                                                            *
 * Created by:  TWC                                           *
 * Date:        Nov. 2014                                     *
 * Version:     0.1.0                                         *
 *                                                            *
 **************************************************************/

#include <LSM303.h>
#include <Wire.h>
#include <ZumoReflectanceSensorArray.h>
#include <SharpIR.h>
#include <QTRSensors.h>
#include <ZumoMotors.h>

/* ******* definition of motor speed parameters ******* */
#define pinLED 13
#define speedForwardMax 200
#define speedBackwardMax -200
#define speedStop 0 
#define speedDelay 10
#define distMin 30 // cm
#define speedBackDuration 500 // ms

// LSM303 digital campass settings
LSM303 compass;

// Accelermeter settings
#define readingSize 3
#define xyAccelerationThreshold 2400 //detection of contact; ~16000 = 9.8 m/s^2

//Reflectance sensor settings
#define numSensors 6
unsigned int sensorValues[numSensors];
#define qtrThreshold 1500 //microseconds
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);

/* ******* Initialization ******* */
int pinIR = 1; //analog PIN for Sharp IR sensor
int startMillis = 0;
unsigned long currentMillis = 0;
long interval = 1000; //milliseconds
SharpIR sharp(pinIR, 50, 93, 20150);
ZumoMotors motors;

enum dirForward {dirStraight, dirBearRight, dirBearLeft};
dirForward currentCourse = dirStraight;

void setup() {
  Serial.begin(9600);
  pinMode(pinLED, OUTPUT);
  Wire.begin();
  compass.init();
  compass.enableDefault();
  //Calibration values
  compass.m_min = (LSM303::vector<int16_t>){-3333, -835, +21725};
  compass.m_max = (LSM303::vector<int16_t>){-3300, -775, +21798};
}

/* Start the main loop */
void loop() {
  compass.read();
  time_loop(10);
  float heading = compass.heading();
  int distance = sharp.distance();
  Serial.print("IR distance: ");
  Serial.print(distance);
  Serial.print(" cm");
  Serial.print("\t");
  Serial.print("Compass heading: ");
  Serial.println(heading);
  //delay(50);
  time_loop(10);
/* negivation block 
(a) Move forward when there is no blockage within 30 cm 
(b) When detecting object within 30 cm, Zumobot will move backward, rotate
to the right and try to find an opening.
*/
  if (distance >= distMin) {
   digitalWrite(pinLED, HIGH);
   motors.setSpeeds(speedForwardMax, speedForwardMax); //(Left, Right)
   //motors.setRightSpeed(speedFMax/ 2);
   //motors.setLeftSpeed(speedFMax / 2);
  } else if (distance < distMin) {
    digitalWrite(pinLED, LOW);
    
    //Back off for 1 second
    motors.setSpeeds(speedBackwardMax, speedBackwardMax); //(Left, Right)
    //delay(speedBackDuration);
    time_loop(speedBackDuration);
    
    //Turn to the right; search for path with at least 50 cm opening
    do {
    motors.setSpeeds(speedForwardMax/2, speedBackwardMax); //(Left, Right)    
    } while (sharp.distance() < 2*distMin);
    
  }
}

/* ------------------------------------------------ */
/* Timeloop function                                */
/*                                                  */
/* Input()                                          */
/* <int> delayMilliSecond: time in milliseconds     */
/*                                                  */
/* Output()                                         */
/* None                                             */
/* ------------------------------------------------ */
void time_loop(int delayMilliSecond) {
  unsigned long startTime;
  unsigned long endTime;
  
  startTime = millis();
  while(millis() - startTime < delayMilliSecond) {
    //Do absolutely nothing.
  }
}
