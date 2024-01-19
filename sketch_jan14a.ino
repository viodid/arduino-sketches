void setup() {
  // put your setup code here, to run once:
  pinMode(2, INPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
}

int switchState = LOW;

int toggleSwitch() {
  int isPushed = digitalRead(2);
  if (isPushed && !switchState)
    return HIGH;
  else if (isPushed && switchState)
    return LOW;
  return switchState;
}

void loop() {
  switchState = toggleSwitch();
  Serial.begin(9600);
  Serial.print("switchState: ");
  Serial.println(switchState);
  if (switchState) {
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
  }
  else {
    digitalWrite(3,LOW);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
  }
  delay(50);
}
