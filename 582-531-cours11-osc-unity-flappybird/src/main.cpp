#include <Arduino.h>
#include <MicroOscSlip.h>
#include <M5_PbHub.h>
#include <FastLED.h>
#include <VL53L0X.h>
#include <M5_Encoder.h>
M5_Encoder myEncoder;
VL53L0X  myTOF;
CRGB monPixel;
M5_PbHub myPbHub;
MicroOscSlip <128> monOsc(&Serial);
#define KEY_CHANNEL_ANGLE 1
#define BROCHE_ATOM_PIXEL 27
#define BROCHE_ATOM_BOUTON 39
#define CANAL_KEY_UNIT 2

void setup() {
  Serial.begin(115200);
  Wire.begin();
  myPbHub.begin();
  myEncoder.begin();
  myTOF.init();
  FastLED.addLeds<WS2812, BROCHE_ATOM_PIXEL , GRB>(&monPixel, 1); 
  myPbHub.setPixelCount(CANAL_KEY_UNIT, 2);
  myPbHub.setPixelCount(KEY_CHANNEL_ANGLE, 1);


  monPixel = CRGB(255,0,0); // ROUGE
  FastLED.show();
  delay(1000); // PAUSE 1 SECONDE
  monPixel = CRGB(255,255,0); // JAUNE
  FastLED.show();
  delay(1000); // PAUSE 1 SECONDE
  monPixel = CRGB(0,255,0); // VERT
  FastLED.show();
  delay(1000); // PAUSE 1 SECONDE
  monPixel = CRGB(0,0,0);
  FastLED.show(); // PAUSE 1 SECONDE
}

void loop() {
  int maLectureKey = myPbHub.digitalRead(CANAL_KEY_UNIT);
  monOsc.sendInt("/Keyunit", maLectureKey);    

  int mesure = myTOF.readRangeSingleMillimeters();
  monOsc.sendInt("/tof", mesure);

  if (maLectureKey == 0)
{
  myPbHub.setPixelColor(CANAL_KEY_UNIT, 0, 0, 0, 255); // Bleu
}
else
{
  myPbHub.setPixelColor(CANAL_KEY_UNIT, 0, 0, 0, 0); // noir
}

    // Mise à jour des valeurs de l'encodeur. 
    // Doit être appelé régulièrement.
    // Doit être appelé avant de lire les valeurs.
    myEncoder.update();

    // Lecture de la rotation de l'encodeur
    int valeurEncodeur = myEncoder.getEncoderRotation();

       // Lecture du changement depuis la dernière lecture
    int changementEncodeur = myEncoder.getEncoderChange();

    // Lecture du bouton 
    int etatBouton = myEncoder.getButtonState();

    // Changer la couleur du pixel de gauche
    // CHANGER ROUGE, VERT, BLEU pour des valeurs entre 0 et 255 (inclusivement)
    myEncoder.setLEDColorLeft( 0, 255, 30 );
    // Changer la couleur du pixel de droite
    // CHANGER ROUGE, VERT, BLEU pour des valeurs entre 0 et 255 (inclusivement)
    myEncoder.setLEDColorRight( 0, 0, 255 );






}
