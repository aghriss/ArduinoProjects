#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"


// Initialize the OLED display using Arduino Wire:
SSD1306Wire display(0x3c, SDA, SCL);
#define RESISTOR A0
#define THERMISTORNOMINAL 100000      
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25   
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 5
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950

int samples[NUMSAMPLES];
float temp = 0;
float average;
float steinhart;
int i;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  pinMode(RESISTOR, INPUT);
  display.init();
//  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
}

void drawTemp() {
  // Font Demo1
  // create more fonts at http://oleddisplay.squix.ch/
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 10, "Voltage");
  display.setFont(ArialMT_Plain_24);
  display.drawString(0, 26, String(temp) + "C");
}

void loop() {
   for (i=0; i< NUMSAMPLES; i++) {
   samples[i] = analogRead(RESISTOR);
   delay(50);
  }
  average = 0;
  for (i=0; i< NUMSAMPLES; i++) {
     average += samples[i];
  }
  average /= NUMSAMPLES;

  average = (1023/average -1);

  steinhart = average;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;      // convert absolute temp to C
  temp = steinhart;
  display.clear();
  drawTemp();
  // write the buffer to the display
  display.display();

  delay(1000);
}
