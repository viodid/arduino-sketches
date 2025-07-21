#define SHIFT_DATA 2
#define SHIFT_CLK 3
#define SHIFT_LATCH 4
#define EEPROM_D0 5
#define EEPROM_D7 12

#include <stdio.h>

void setAddress(int add, bool outEnable) {
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, outEnable ? (add >> 8 | 1 << 8) : (add >> 8));
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, add);

  digitalWrite(SHIFT_LATCH, LOW);
  digitalWrite(SHIFT_LATCH, HIGH);
  digitalWrite(SHIFT_LATCH, LOW);
}

/*
 * Read a byte from the EEPROM at the specified address.
 */
byte readEEPROM(int address) {
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin++) {
    pinMode(pin, INPUT);
  }
  setAddress(address, true);

  byte data = 0;
  for (int pin = EEPROM_D7; pin >= EEPROM_D0; pin -= 1) {
    data = (data << 1) + digitalRead(pin);
  }
  return data;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);
  Serial.begin(57600);
  Serial.println(readEEPROM(3));
}

void loop() {
  // put your main code here, to run repeatedly:

}
