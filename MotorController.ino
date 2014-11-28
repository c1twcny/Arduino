/* --------------------------------------------------------- */
/*                                                           */
/* Serial Monitor is set at 19200 baud rate                  */
/*                                                           */
/* --------------------------------------------------------- */

/* --------------------------------------------------------- */
/*                                                           */
/*                    Front                                  */
/*               ---------------                             */
/*               |             |                             */
/*           M1  |             |  M2                         */
/*               |             |                             */
/*               |             |                             */
/*               |             |                             */
/*               |             |                             */
/*         L3/L4 |             | L1/L2                       */
/*               |             |                             */
/*               ---------------                             */
/*                     Back                                  */
/*                                                           */
/* --------------------------------------------------------- */

/* --------------------------------------------------------- */
/*                                                           */
/* Solarbotics L298 Motor Driver logic table                 */
/*                                                           */
/* Enable  L1  L2  Result    Enable  L3  L4  Result          */
/*    L    L    L  Off        L      L    L  Off             */
/*    L    L    H  Off        L      L    H  Off             */
/*    L    H    L  Off        L      H    L  Off             */
/*    L    H    H  Off        L      H    H  Off             */
/* ......................................................... */
/*    H    L    L  Break      H      L    L  Break           */
/*    H    L    H  Forward    H      L    H  Forward         */
/*    H    H    L  Backward   H      H    L  Backward        */
/*    H    H    H  Break      H      H    H  Break           */
/* ......................................................... */
/*    H    L    L  Break      H      L    L  Break           */
/*    H    pwm  H  FWD-SPD    H      pwm  H  FWD-SPD         */
/*    H    pwm  L  BCK-SPD    H      pwm  L  BCK-SPD         */
/*    H    H    H  Break      H      H    H  Break           */
/*                                                           */
/* --------------------------------------------------------- */



/* --------------------------------------------------------- */
/* Define libraries and header files                         */
/* --------------------------------------------------------- */
#include <SoftwareSerial.h>
#include <Servo.h>
#include <Ping.h>
#include <math.h>


/* --------------------------------------------------------- */
/* Motor drive variables                                     */
/* --------------------------------------------------------- */
/* --------------------------------------------------------- */
/* Define motor controller pin setting                       */
/* --------------------------------------------------------- */
#define rxPin      11    //pololuTrexJr Tx: 9 Red
#define txPin      12    //pololuTrexJr Rx: 10 Green 
#define l4Pin      10    //Solarbotics Tx  l4 (Back-Right)
#define l3Pin      9     //Solarbotics Rx  l3 (Back-Right)
#define l2Pin      7     //Solarbotics Tx  l2 (Back-Left)
#define l1Pin      6     //Solarbotics Rx  l1 (Back-Left)
#define enablePin  5     //Solarbotics enable pin -- works on pins 3,5,6,9,10,11. See analogWrite() for more detail

/* --------------------------------------------------------- */
/* Create a Pololu TrexJr motor instance                     */
/* --------------------------------------------------------- */
//SoftwareSerial pololuTrexJr = SoftwareSerial(rxPin, txPin); //create new pololuTrexJr instance
SoftwareSerial pololuTrexJr(rxPin, txPin);



/* ------------------------------------------------------------------------------ */
/* Initialize the program                                                         */
/* ------------------------------------------------------------------------------ */
void setup() {
  Serial.begin(19200);
  pololuTrexJr.begin(19200);
  
  pinMode(rxPin, INPUT);              //pololuTrexJr
  pinMode(txPin, OUTPUT);             //pololuTrexJr  
  pinMode(l1Pin, OUTPUT);             //Solarbotics
  pinMode(l2Pin, OUTPUT);             //Solarbotics
  pinMode(l3Pin, OUTPUT);             //Solarbotics
  pinMode(l4Pin, OUTPUT);             //Solarbotics
  pinMode(enablePin, OUTPUT);         //Solarbotics
  
  timeLoop(1000);
}

/* ------------------------------------------------------------------------------ */
/* Start the main program loop                                                    */
/* ------------------------------------------------------------------------------ */
void loop() {

/* --------------------------------------------------------- */
/* Start all 4 motors                                        */
/* --------------------------------------------------------- */

 analogWrite(enablePin, 96);
 digitalWrite(l1Pin, LOW);   //Rear-Right, clockwise
 digitalWrite(l2Pin, HIGH);  //Rear-Right, clockwise
 digitalWrite(l3Pin, HIGH);  //Rear-Left, counter-clockwise
 digitalWrite(l4Pin, LOW);   //Rear-Left, counter-clockwise

// Pololu forward: M1: 0xC2; M2:0xCA  
 pololuTrexJr.write(0xC2); //set M1 motor forward (0xC2)
 pololuTrexJr.print(0x64, HEX); //set M1 motor speed at 80 (0~127)
 pololuTrexJr.write(0xCA); //set M2 motor forward (0xCA)
 pololuTrexJr.print(0x64, HEX); //set M2 motor speed at 80 (0~127)

// Pololu reverse: M1: 0xC1; M2:0xC9  
 /* pololuTrexJr.write(0xC1); //set M1 motor forward (0xC2)
 pololuTrexJr.print(0x64, HEX); //set M1 motor speed at 80 (0~127)
 pololuTrexJr.write(0xC9); //set M2 motor forward (0xCA)
 pololuTrexJr.print(0x64, HEX); //set M2 motor speed at 80 (0~127) */
 
 
  timeLoop(100);   
  digitalWrite(l3Pin, LOW); //Rear-Left
  digitalWrite(l4Pin, LOW); //Rear-Left
  digitalWrite(l1Pin, LOW); //Rear-Right
  digitalWrite(l2Pin, LOW); //Rear-Right
  digitalWrite(enablePin, LOW);

  //pololuTrexJr.write(0xC0);    //set M1 motor stop
  //pololuTrexJr.print(0x0, HEX);//set M1 motor speed at 0 (coasting)
  //pololuTrexJr.write(0xC8);    //set M2 motor stop
  //pololuTrexJr.print(0x0, HEX);//set M2 motor speed at 0 (coasting)
}


/* --------------------------------------------------------- */
/* Timeloop function                                         */
/* delay in milliseconds                                     */
/* --------------------------------------------------------- */
void timeLoop (int delayMilliSecond) {
  unsigned long startTime;
  unsigned long endTime;
  
  startTime = millis();
  while (millis() - startTime < delayMilliSecond) {
    //do nothing; 
  }
}
