#include <SoftwareSerial.h>
#include <ParallaxLCD.h>

SoftwareSerial portOne(7,3); // pin 7: digital RX; pin 3: digital TX

const int pingPin = 7;

long microsecondsToInches(long microseconds) {
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  portOne.begin(9600);
  //portOne.println("Hello from Princeton Junction!");

}

void loop() {
  // put your main code here, to run repeatedly:
  long duration, inches, cm;
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(4);
  digitalWrite(pingPin, LOW);
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);

  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);

  //Serial.print(inches);
  portOne.print("Distance: ");
  portOne.print(inches);
  portOne.println(" inches");
  portOne.flush();
  delay(100);
  portOne.print("Distance: ");
  portOne.print(cm);
  portOne.println(" cm....");
  portOne.flush();


  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm, ");
  Serial.println();
  delay(500);

  Serial.println("Data from ultrasonic sensor:");
  while (portOne.available() >0) {
    char inByte = portOne.read();
    Serial.write(inByte);
  }

}
