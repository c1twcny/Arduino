// Basic test on Arduino Uno
// 
// Sharp IR sensor: 2Y0A02
// Yellow -> A1
// Black  -> GND
// Red    -> 5V

#define LED_PIN0 11
#define LED_PIN1 10
#define LED_PIN2 9
#define IRpin 1

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN0, OUTPUT);
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  float volts = analogRead(IRpin)*0.0048828125;
  float distance = 65*pow(volts, -1.10);
  Serial.print(distance);
  Serial.println(" cm");
  delay(100);
  digitalWrite(LED_PIN0, HIGH);
  digitalWrite(LED_PIN1, LOW);
  digitalWrite(LED_PIN2, LOW);
  delay(100);
  digitalWrite(LED_PIN0, LOW);
  digitalWrite(LED_PIN1, HIGH);
  digitalWrite(LED_PIN2, LOW);
  delay(100);
  digitalWrite(LED_PIN0, LOW);
  digitalWrite(LED_PIN1, LOW);
  digitalWrite(LED_PIN2, HIGH);
  delay(100); 
}
