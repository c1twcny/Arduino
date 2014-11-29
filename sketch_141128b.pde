/************************************************************************************* 
Author:   TWC
Date:     Nov. 2014
Version:  0.2.0

Processing sketch for testing Zumobot functions:
Run "Serial-conversion.ino" sketch first then execute this sketch which will provide
visual ouput of accelerations on X, Y and Z axis as well as magnetic heading
**************************************************************************************/

import processing.serial.*;
Serial myPort;

int tnumValues= 4; // total inputs <acc-x, acc-y, acc-z, magn-heading>
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
float x2, y2;
int x3 = 500; // compass instrumentation panel center position X
int y3 = 100; // compass instrumentation panel center position Y
int diameterMagnPanel = 100;

// acceleration axis parameters
int xPosI = 20;
int xPos  = xPosI;
int yPosOffset = 12;
int widthMargin = 10;
float xAxisLength;
float lengthScaling = 0.6;

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
    for (int i = 0; i < numValues; i++) {
      values[i] = tarray[i];
    }
    
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
      line(xPos, partH*(i+0.5), xPos, partH*(i+1)-mappedVal); //(x1, y1, x2, y2)
      
      /* Compass heading instrumentation panel:
      fill the background with a slightly smaller circle with the background
      color, then draw a second circle with a line pointer pointing to the averaged
      heading reading from LSM303D */
       
      fill(255);
      ellipse(x3, y3, diameterMagnPanel-1, diameterMagnPanel-1); //(400, 120, 79, 79)
       
      /* Trick is using a slightly shorter length to calculate the (x,y) position
         of the tip; the total length of the line plus the radius of the small
         circule should be less or equal of the larger circle. In this case the total
         length is 35 + 8/2 = 39, close to 40 which is the radius of a large circle */ 
          
      x2 = (diameterMagnPanel/2 - 5)*cos(radians(tarray[3]+90));
      y2 = -(diameterMagnPanel/2 - 5)*sin(radians(tarray[3]+90));
      noFill();
      ellipse(x3,y3, diameterMagnPanel, diameterMagnPanel);
      fill(0);
      line(x3, y3, x3+x2, y3+y2);
      fill(255,0,0);
      ellipse(x3+x2, y3+y2, diameterMagnPanel/10, diameterMagnPanel/10);
       
      fill(255);
      rect(x3+diameterMagnPanel/2+30, y3+10, 50, -20);
      fill(0);
      textAlign(LEFT);
      text("Heading", x3+diameterMagnPanel/2+30, y3-15);
      if (tarray[3] > 360) {
        tarray[3] = 360.0;
      }
      textAlign(LEFT);
      text(tarray[3], x3+diameterMagnPanel/2+30, y3+3);
      textAlign(CENTER);
      text("N", x3, y3-diameterMagnPanel/2-10);
      line(x3, y3-diameterMagnPanel/2-5, x3, y3-diameterMagnPanel/2+5);
      textAlign(LEFT, CENTER);
      text("W", x3-diameterMagnPanel/2-20, y3);
      line(x3-diameterMagnPanel/2-5, y3, x3-diameterMagnPanel/2+5, y3);
      textAlign(CENTER);
      text("S", x3, y3+diameterMagnPanel/2+23);
      line(x3, y3+diameterMagnPanel/2-5, x3, y3+diameterMagnPanel/2+5);
      textAlign(LEFT, CENTER);
      text("E", x3+diameterMagnPanel/2+10, y3);
      line(x3+diameterMagnPanel/2-5, y3, x3+diameterMagnPanel/2+5, y3);    
    } //for (int i=0; i < numValues; i++)
     
    if (xPos > width*lengthScaling) {
      xPos = xPosI;
      fill(255);
      noStroke();
      rect(0,0,width,height);
    } else {
      xPos+=2;
    }    
  } //if (inString != null) 
} //serialEvent()

