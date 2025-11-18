#include <Arduino.h>
#include <MicroOscSlip.h>
#include <M5_PbHub.h>
#include <FastLED.h>
#include <VL53L0X.h>
#include <M5_Encoder.h>
M5_Encoder myEncoder;
VL53L0X myTOF;
CRGB monPixel;
M5_PbHub myPbHub;
MicroOscSlip<128> monOsc(&Serial);
#define KEY_CHANNEL_ANGLE 1
#define BROCHE_ATOM_PIXEL 27
#define BROCHE_ATOM_BOUTON 39
#define CANAL_KEY_UNIT 2
unsigned long monChronoDepart;

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  myPbHub.begin();
  myTOF.init();
  FastLED.addLeds<WS2812, BROCHE_ATOM_PIXEL, GRB>(&monPixel, 1);
  myPbHub.setPixelCount(CANAL_KEY_UNIT, 2);
  myPbHub.setPixelCount(KEY_CHANNEL_ANGLE, 1);

  monPixel = CRGB(255, 0, 0);
  FastLED.show();
  delay(1000);
  monPixel = CRGB(255, 255, 0);
  FastLED.show();
  delay(1000);
  monPixel = CRGB(0, 255, 0);
  FastLED.show();
  delay(1000);
  monPixel = CRGB(0, 0, 0);
  FastLED.show();
  delay(1000);
}

void loop()
{
  if (millis() - monChronoDepart >= 20)
  {
    monChronoDepart = millis();
    myEncoder.update();

    int valeurEncodeur = myEncoder.getEncoderRotation();
    // Lecture du changement depuis la dernière lecture
    int changementEncodeur = myEncoder.getEncoderChange();

    // Lecture du bouton
    int etatBouton = myEncoder.getButtonState();

    if (myEncoder.getEncoderChange() > 0){

      myEncoder.setLEDColorLeft(0, 255, 200);
      myEncoder.setLEDColorRight(0, 0, 0);

    } else if (myEncoder.getEncoderChange() < 0)
    {
      myEncoder.setLEDColorRight(0, 0, 255);
      myEncoder.setLEDColorLeft(0, 0, 0);
    }
    monOsc.sendInt("/boutonEncodeur", etatBouton);
    monOsc.sendInt("/changeEncoder", changementEncodeur); 

  }
}

