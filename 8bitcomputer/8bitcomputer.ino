#define SHIFT_DATA 2
#define SHIFT_CLK 3
#define SHIFT_LATCH 4
#define EEPROM_D0 5
#define EEPROM_D7 12
#define WRITE_EN 13

#include <stdio.h>

// 4-bit hex decoder for common anode 7-segment display
byte DATA[] = { 0x81, 0xcf, 0x92, 0x86, 0xcc, 0xa4, 0xa0, 0x8f, 0x80, 0x84, 0x88, 0xe0, 0xb1, 0xc2, 0xb0, 0xb8 };


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


void writeEEPROM(int address, byte data) {
  setAddress(address, false);
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin++) {
    pinMode(pin, OUTPUT);
  }
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin++) {
    digitalWrite(pin, data & 1);
    data = data >> 1;
  }
  digitalWrite(WRITE_EN, LOW); // write
  delayMicroseconds(1);
  digitalWrite(WRITE_EN, HIGH); // disable write
  delay(10);
}

void printContents(int bytes) {
  for (int base = 0; base < bytes * 256 - 1; base+=16) {
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

void setup() {
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);
  digitalWrite(WRITE_EN, HIGH);
  pinMode(WRITE_EN, OUTPUT);
  Serial.begin(57600);

  Serial.print("Erasing EEPROM");
  for (int address = 0; address <= 2047; address += 1) {
    writeEEPROM(address, 0xff);

    if (address % 64 == 0) {
      Serial.print(".");
    }
  }
  Serial.println(" done");
  // Serial.print("Programming EEPROM");
  // for (int address = 0; address < sizeof(DATA); address += 1) {
  //   writeEEPROM(address, DATA[address]);

  //   if (address % 64 == 0) {
  //     Serial.print(".");
  //   }
  // }
  Serial.println(" done");
  Serial.println("Reading EEPROM");
  printContents(2);
  Serial.println(" done");
}
