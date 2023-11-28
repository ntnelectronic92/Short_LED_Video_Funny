#include <Arduino.h>
#include "LedControl.h"
#define volumeControlPin  A3
int volumeValue  = 0;
int volumeValueFilter = 0;

#define MAX_DEVICES 1

#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    10

byte array[][8]={
 { 0x3C, 0x7E, 0xC3, 0xC3, 0xC3, 0xC3, 0x7E, 0x3C, },
 { 0x3C, 0x7E, 0xC3, 0xC3, 0x03, 0xC3, 0x7E, 0x3C, },
 { 0x3C, 0x7E, 0xC3, 0xC3, 0x03, 0x03, 0x7E, 0x3C, },
 { 0x3C, 0x7E, 0xC3, 0xC3, 0x03, 0x03, 0x3E, 0x3C, },
 { 0x3C, 0x7E, 0xC3, 0xC3, 0x03, 0x03, 0x1E, 0x1C, },
 { 0x3C, 0x7E, 0xC3, 0xC3, 0x03, 0x03, 0x0E, 0x0C, },
 { 0x3C, 0x7E, 0xC3, 0xC3, 0x03, 0x03, 0x06, 0x04, },
 { 0x3C, 0x7E, 0xC3, 0xC3, 0x03, 0x03, 0x02, 0x00, },
 { 0x3C, 0x7E, 0xC3, 0xC3, 0x03, 0x03, 0x00, 0x00, },
 { 0x3C, 0x7E, 0xC3, 0xC3, 0x03, 0x00, 0x00, 0x00, },
 { 0x3C, 0x7E, 0xC3, 0xC3, 0x00, 0x00, 0x00, 0x00, },
 { 0x3C, 0x7E, 0xC3, 0xC0, 0x00, 0x00, 0x00, 0x00, },
 { 0x3C, 0x7E, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, },
 { 0x3C, 0x7C, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, },
 { 0x38, 0x78, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, },
 { 0x30, 0x70, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, },
 { 0x20, 0x60, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, },
 { 0x00, 0x40, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, },
 { 0x00, 0x00, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, },
 { 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, },
 { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, },
};

#define SIZE_MAX(X) sizeof(X)/sizeof(X[0])
LedControl lc = LedControl(DATA_PIN, CLK_PIN, CS_PIN, 1);  // Create a LedControl object
/// INIT AREA////////////
void initSerial()
{
  Serial.begin(115200);
  Serial.print("MAX VOLUME = ");
  int valueMaxVolume = SIZE_MAX(array);
  Serial.print(valueMaxVolume);
   Serial.println();
  
}
void initMatrix7219()
{
  lc.shutdown(0, false);  // Wake up the MAX7219
  lc.setIntensity(0, 8);  // Set the brightness (0-15)
  lc.clearDisplay(0);     // Clear the displa
}
void displayIcon(byte icon[]) {
  for (int row = 0; row < 8; row++) {
    lc.setRow(0, row, icon[row]);
  }
}

////HANDLE AREA////////////
void volumeHandle()
{
  volumeValue = analogRead(volumeControlPin);
  volumeValueFilter = map(volumeValue,0,1024,0,SIZE_MAX(array));

}
void matrixHandle()
{
  static int preVolumeValue = 1000;
  if(preVolumeValue!= volumeValueFilter)
  {
    Serial.print("Value of volumeValueFilter =");
    Serial.print(volumeValueFilter);
    Serial.println();
    lc.clearDisplay(0);
    displayIcon(array[volumeValueFilter]);
    preVolumeValue = volumeValueFilter;
  }
}
void setup() {
  initSerial();
  initMatrix7219();
}

void loop() 
{
  volumeHandle();
  matrixHandle();
}