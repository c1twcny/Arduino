/***************************************************************************************** 
Author:   TWC
Date:     Nov. 2014
Version:  0.2.0

Processing sketch for testing Zumobot functions:
Run "Serial-conversion.ino" sketch first then execute this sketch which will provide
visual ouput of accelerations on X, Y and Z axis, magnetic heading and distance reading
from Sharp IR sensor (GP2Y0A02YK)

Note:
(1) Sharp GP2Y0A02YK detection distance: 20 cm ~ 150 cm.
*******************************************************************************************/

import processing.serial.*;
Serial myPort;

int tnumValues= 5; // total inputs <acc-x, acc-y, acc-z, magn-heading, IR-distance>
int numValues = 3; // acceleration on X, Y & Z
float[] tarray     = new float[tnumValues];
float[] values     = new float[numValues];
float[] minACC     = new float[numValues];
float[] maxACC     = new float[numValues];
color[] valColor   = new color[numValues];
String[] str       = new String[numValues];
float partH;

// compass heading panel
float magnHEADING = 0.0;
float deltaX, deltaY;
int xMagn = 500; // compass instrumentation panel center position X
int yMagn = 100; // compass instrumentation panel center position Y
int diameterMagnPanel = 100;

// acceleration axis parameters
int xPosI = 20;
int xPos  = xPosI;
int yPosOffset = 12;
int widthMargin = 10;
float xAxisLength;
float lengthScaling = 0.5;

// Sharp IR sensor panel
char algo = 'l';
int xIR = 500;
int yIR = yMagn + int(diameterMagnPanel*2);
int diameterIRPanel = 200;
float distIR = 0.0;
float distIRMax = 200.0; // limit the maximum distance to 200 cm
// logarithm scaling
//float scaling150 = 0.94560315; // log(150.0)/log(200.0)
//float scaling100 = 0.869176;   // log(100.0)/log(200.0)
//float scaling050 = 0.73835194; // log(50.0)/log(200.0)
//linear scaling
float scaling150 = 0.75; // 150.0/200.0
float scaling100 = 0.5;  // 100.0/200.0
float scaling050 = 0.25; // 50.0/200.0
float deltaXIR = diameterIRPanel*0.5*cos(radians(30.0));
float deltaYIR = diameterIRPanel*0.5*sin(radians(30.0));



PFont f;

void setup() {
  size(640,420);
  partH = height / numValues; // "height" is set by the second parameter of the size()
  println(Serial.list());
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 9600);
  xAxisLength = partH*0.75;
  
  values[0] = 0;
  minACC[0] = -2;
  maxACC[0] = 2;
  valColor[0] = color(255, 0, 0);
  str[0] = "X-axis";
  
  values[1] = 0;
  minACC[1] = -2;
  maxACC[1] = 2;
  valColor[1] = color(0, 255, 0);
  str[1] = "Y-axis";
  
  values[2] = 0;
  minACC[2] = -2;
  maxACC[2] = 2;
  valColor[2] = color(0, 0, 255);
  str[2] = "Z-axis";
  
  f = createFont("Arial", 12, true);
  background(255);
}

void draw() {
  // nothing happens in draw...all happens in serialEvent()

  textFont(f, 12);
}

void serialEvent (Serial myPort) {
  String inString = myPort.readStringUntil('\n');
  if (inString != null) {    
    inString = trim(inString);
    tarray = float(splitTokens(inString, ", \t"));
    for (int i = 0; i < numValues; i++) { // extract <x,y,z> acceleration telemetry data
      values[i] = tarray[i];
      print(values[i]);print('\t');
    }
    print(tarray[3]); print('\t');
    println(tarray[4]);
    
    for (int i=0; i < numValues; i++) {

      /* maximum +2g */
      fill(valColor[i]);
      noStroke();
      textAlign(LEFT, CENTER);
      text(str[i],xPosI+5, partH*i+yPosOffset);
      text("max-g", xPosI+45, partH*i+yPosOffset);
      text(maxACC[i], xPosI+80, partH*i+yPosOffset);
      text("m/s^2", xPosI+115, partH*i+yPosOffset);
      fill(255);
      rect(xPosI+160, partH*i+18, 40, -20);
      fill(valColor[i]);
      text(values[i], xPosI+160, partH*i+yPosOffset);
      text("g", xPosI+195, partH*i+yPosOffset);
      
      /* zero-g line */
      //rect(xPosI, partH*(i+0.5), width-widthMargin, 2); //draw the axis
      rect(xPosI, partH*(i+0.5), width*lengthScaling, 2);
      text("0", xPosI-10, partH*(i+0.5)+5);
      rect(xPosI, partH*(i+0.25), 1, partH*0.5);
      rect(xPosI, partH*(i+0.25), 5, 1);
      text("1", xPosI-10, partH*(i+0.25)+5);      
      rect(xPosI, partH*(i+0.75), 5, 1);
      text("-1", xPosI-10, partH*(i+0.75)+5);
      
      float mappedVal = map(values[i], minACC[i], maxACC[i], 0, partH);
      //print(values[i]);
      //print("\t");
      //println(mappedVal);
      stroke(valColor[i]);
      line(xPos, partH*(i+0.5), xPos, partH*(i+1)-mappedVal); //(x1, y1, deltaX, deltaY)
      
      /******************************************************************************
      Compass heading instrumentation panel
      Fill the background with a slightly smaller circle with the background
      color, then draw a second circle with a line pointer pointing to the averaged
      heading reading from LSM303D 
      *******************************************************************************/
       
      fill(255); //create a smaller circle below using the background color
      ellipse(xMagn, yMagn, diameterMagnPanel-1, diameterMagnPanel-1);
       
      /* Trick is using a slightly shorter length to calculate the (x,y) position
         of the tip; the total length of the line plus the radius of the small
         circule should be less or equal of the larger circle. In this case the total
         length is 35 + 8/2 = 39, close to 40 which is the radius of a large circle */ 
          
      deltaX = (diameterMagnPanel/2 - 5)*cos(radians(tarray[3]+90));
      deltaY = -(diameterMagnPanel/2 - 5)*sin(radians(tarray[3]+90));
      noFill();
      ellipse(xMagn,yMagn, diameterMagnPanel, diameterMagnPanel);
      fill(0);
      line(xMagn, yMagn, xMagn+deltaX, yMagn+deltaY);
      fill(255,0,0);
      ellipse(xMagn+deltaX, yMagn+deltaY, diameterMagnPanel/10, diameterMagnPanel/10);
       
      fill(255);
      rect(xMagn+diameterMagnPanel/2+30, yMagn+10, 50, -20);
      fill(0);
      textAlign(LEFT);
      text("Heading", xMagn+diameterMagnPanel/2+30, yMagn-15);
      if (tarray[3] > 360) {
        tarray[3] = 360.0;
      }
      textAlign(LEFT);
      text(tarray[3], xMagn+diameterMagnPanel/2+30, yMagn+3);
      textAlign(CENTER);
      text("N", xMagn, yMagn-diameterMagnPanel/2-10);
      line(xMagn, yMagn-diameterMagnPanel/2-5, xMagn, yMagn-diameterMagnPanel/2+5);
      textAlign(LEFT, CENTER);
      text("W", xMagn-diameterMagnPanel/2-20, yMagn);
      line(xMagn-diameterMagnPanel/2-5, yMagn, xMagn-diameterMagnPanel/2+5, yMagn);
      textAlign(CENTER);
      text("S", xMagn, yMagn+diameterMagnPanel/2+23);
      line(xMagn, yMagn+diameterMagnPanel/2-5, xMagn, yMagn+diameterMagnPanel/2+5);
      textAlign(LEFT, CENTER);
      text("E", xMagn+diameterMagnPanel/2+10, yMagn);
      line(xMagn+diameterMagnPanel/2-5, yMagn, xMagn+diameterMagnPanel/2+5, yMagn);
      
      /******************************************************************************
      IR distance sensor instrumentation panel
      Fill the background with a slightly smaller circle with the background
      color, then draw a second circle with a line pointer pointing to the averaged
      distance reading from Sharp IR sensor 
      *******************************************************************************/   
      noFill();
      stroke(0, 154, 255);      
      ellipse(xIR, yIR, diameterIRPanel, diameterIRPanel);                       //2.0m mark
      textAlign(RIGHT, BOTTOM);
      text("2", xIR-diameterIRPanel/2, yIR);
      textAlign(LEFT, BOTTOM);
      text("2", xIR+diameterIRPanel/2, yIR);
      stroke(0, 255, 85);
      ellipse(xIR, yIR, diameterIRPanel*scaling150, diameterIRPanel*scaling150); //1.5m mark
      stroke(255, 100, 0);
      ellipse(xIR, yIR, diameterIRPanel*scaling100, diameterIRPanel*scaling100); //1.0m mark
      textAlign(RIGHT, BOTTOM);
      text("1", xIR-diameterIRPanel/4, yIR);
      textAlign(LEFT, BOTTOM);
      text("1", xIR+diameterIRPanel/4, yIR);
      stroke(255,0,0);
      ellipse(xIR, yIR, diameterIRPanel*scaling050, diameterIRPanel*scaling050); //0.5m mark
      
      /* Draw a rectangle at the center of circle to represent the Zumobot 
      The inner circle represents the 1-meter distance from the Zumo, where the outer
      circle is the 2-meter mark. 
      */      
      noFill();
      stroke(0);
      rect(xIR-6, yIR-10, 12, 20);
        
      
      // draw horizontal and vertical axes
      fill(200);
      stroke(200);
      rect(xIR-diameterIRPanel/2, yIR, diameterIRPanel, 1);
      //rect(xIR, yIR-diameterIRPanel/2, 1, diameterIRPanel);

      // plot distance data from the IR sensor
      fill(255,0,0);
      stroke(255,0,0);
      /* Sharp IR sensor effective detection range is between 20 cm ~ 150 cm;
         measurements outside of the above range are unreliable and should be reseted to
         reasonable user defined numbers for recalculation.
      */
      if (tarray[4] <= 0) {
        tarray[4] = 0;
      } else if (tarray[4] >= 200) {
        tarray[4] = 200;
      }
      ellipse(xIR, yIR-int(tarray[4])/2, 2, 2);
   
      
    } //for (int i=0; i < numValues; i++)
     
    if (xPos > width*lengthScaling) {
      xPos = xPosI;
      fill(255);
      noStroke();
      rect(0,0,width*lengthScaling+20,height);
    } else {
      xPos+=2;
    }    
  } //if (inString != null) 
} //serialEvent()

