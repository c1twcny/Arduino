

import processing.serial.*;
Serial myPort;

int numValues = 3; // acceleration on X, Y & Z 
float[] values  = new float[numValues];
float[] min     = new float[numValues];
float[] max     = new float[numValues];
color[] valColor= new color[numValues];
String[] str    = new String[numValues];
float partH;


// axis parameters
int xPosI = 20;
int xPos  = xPosI;
int widthMargin = 10;
float xAxisLength;

void setup() {
  size(480,320);
  partH = height / numValues; // "height" is set by the second parameter of the size()
  println(Serial.list());
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 9600);
  xAxisLength = partH*0.75;
  
  values[0] = 0;
  min[0] = -2;
  max[0] = 2;
  valColor[0] = color(255, 0, 0);
  str[0] = "X-axis";
  
  values[1] = 0;
  min[1] = -2;
  max[1] = 2;
  valColor[1] = color(0, 255, 0);
  str[1] = "Y-axis";
  
  values[2] = 0;
  min[2] = -2;
  max[2] = 2;
  valColor[2] = color(0, 0, 255);
  str[2] = "Z-axis";
  
  background(255);
}

void draw() {
  // nothing happens in draw...all happens in serialEvent()
}

void serialEvent (Serial myPort) {
  String inString = myPort.readStringUntil('\n');
  if (inString != null) {
    //println(inString);
    
    inString = trim(inString);
    values = float(splitTokens(inString, ", \t"));
    
    for (int i=0; i < numValues; i++) {
      // maximum +2g
      fill(0);
      noStroke();
      rect(xPosI, partH*i+1, width-widthMargin, 2); // rect(X, Y, width, height)
      fill(0);
      text(str[i],xPosI+5, partH*i+12);
      text("max-g", xPosI+45, partH*i+12);
      text(max[i], xPosI+80, partH*i+12);
      text("m/s^2", xPosI+115, partH*i+12);
      // zero g line
      fill(0);
      rect(xPosI, partH*(i+0.5), width-widthMargin, 2);
      
      fill(0);
      text(str[i], xPosI+5, partH*(i+0.5)-3);
      text("zero-g", xPosI+45, partH*(i+0.5)-3);
      fill(0);
      rect(xPosI, partH*(i+0.25), 2, partH*0.5);
      rect(xPosI, partH*(i+0.25), 5, 1);
      rect(xPosI, partH*(i+0.75), 5, 1);
      
      float mappedVal = map(values[i], min[i], max[i], 0, partH);
      print(values[i]);
      print("\t");
      println(mappedVal);
      stroke(valColor[i]);
      line(xPos, partH*(i+0.5), xPos, partH*(i+1)-mappedVal); //(x1, y1, x2, y2)
    }
    if (xPos >= width-widthMargin) {
      xPos = xPosI;
      fill(255);
      noStroke();
      rect(0,0,width,height);
    } else {
      xPos+=2;
    }
  }
}


