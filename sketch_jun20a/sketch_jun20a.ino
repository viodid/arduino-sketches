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


  for (int base = 0; base < 255; base+=16) {
    byte data[16];  
    for (int offset = 0; offset < 16; offset++) {
      data[offset] = readEEPROM(offset + base);
    }
    char buf[80];
    sprintf(buf, "%03x: %02x %02x %02x %02x %02x %02x %02x %02x\t%02x %02x %02x %02x %02x %02x %02x %02x",
        base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9],
        data[10], data[11], data[12], data[13], data[14], data[15]);
    Serial.println(buf);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
