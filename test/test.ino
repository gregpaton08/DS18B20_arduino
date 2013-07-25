#include <OneWire.h>

// DS18S20 Temperature chip i/o
OneWire ds(10);  // on pin 10
byte addr[8];

void setup(void) {
  // initialize inputs/outputs
  // start serial port
  Serial.begin(9600);

  while ( !ds.search(addr)) {
      Serial.print("No more addresses.\n");
      ds.reset_search();
  }
}

void loop(void) {
  int temp = 0;
  byte *pTemp = (byte*)&temp;
  bool negative = false;
  int tf_100;
  

  if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.print("CRC is not valid!\n");
      return;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44);         // start conversion

  //delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  // we need the first 2 bytes
  for (byte i = 0; i < 2; i++)   
    pTemp[i] = ds.read();
  
  negative = temp & 0x8000;  // test most sig bit
  if (negative) // negative
    temp = (temp ^ 0xffff) + 1; // 2's comp
    
  tf_100 = (11 * temp) + temp / 4 + 3200 + 0.5;    // multiply by (100 * 0.0625) or 6.25

  if (negative) // If its negative
     Serial.print("-");
  
  Serial.print(tf_100 / 100);
  Serial.print(".");
  if ((tf_100 % 100) < 10)
     Serial.print("0");
  Serial.println(tf_100 % 100);
}
