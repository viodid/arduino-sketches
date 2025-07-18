const int sensorPin = A0;

void setup() {
  // put your setup code here, to run once:
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(5, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(3, HIGH);
  int sensorVal = analogRead(sensorPin);
  Serial.print("Sensor Value: ");
  Serial.println(sensorVal);
  delay(1000);
}
