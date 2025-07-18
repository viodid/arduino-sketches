#define SHIFT_DATA 2
#define SHIFT_CLK 3
#define SHIFT_LATCH 4
#define EEPROM_D0 5
#define EEPROM_D7 12

#include <stdio.h>

void setAddress(int add, bool outEnable) {
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);

  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, outEnable ? (add >> 8 | 1 << 8) : (add >> 8));
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, add);
  digitalWrite(SHIFT_LATCH, LOW);
  digitalWrite(SHIFT_LATCH, HIGH);
  digitalWrite(SHIFT_LATCH, LOW);
}

byte readEEPROM(int add) {
  setAddress(1024, true);
  byte data = 0;
  for (int i = EEPROM_D7; i >= EEPROM_D0; i--) {
      data = (data << 1) + digitalRead(i);
  }
  return data;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  Serial.println(readEEPROM(1));
}

void loop() {
  // put your main code here, to run repeatedly:

}
