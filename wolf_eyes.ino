#include <Streaming.h>
#include <Adafruit_NeoPixel.h>

#define EYES_PIN 11
#define FADE_STEP 100

Adafruit_NeoPixel eyes = Adafruit_NeoPixel(2, EYES_PIN, NEO_GRB | NEO_KHZ800);

bool fading = false;
unsigned long fadeStartedAtMilliSeconds = 0;
unsigned long fadeInMilliSeconds = 0; 

typedef struct Colour {
  uint8_t current;
  uint8_t destination;
  uint8_t original;
} Colour;

Colour red = {0, 0, 0};
Colour green = {0, 0, 0};
Colour blue = {0, 0, 0};

uint16_t totalSteps = 0;
uint16_t currentStep = 0;

void startFade(uint8_t newRed, uint8_t newGreen, uint8_t newBlue, unsigned long milliSeconds)
{
  currentStep = 0;
  totalSteps = milliSeconds / (1000 / FADE_STEP);

  red.original = red.current;
  red.destination = newRed;
  green.original = green.current;
  green.destination = newGreen;
  blue.original = blue.current;
  blue.destination = newBlue;
  
  fading = true;
}

void fade()
{
  if (fading && currentStep < totalSteps) {
    red.current = ((red.original * (totalSteps - currentStep)) + (red.destination * currentStep)) / totalSteps;
    green.current = ((green.original * (totalSteps - currentStep)) + (green.destination * currentStep)) / totalSteps;
    blue.current = ((blue.original * (totalSteps - currentStep)) + (blue.destination * currentStep)) / totalSteps;
    currentStep++;
  } else if (fading) {
    fading = false;
  }
}

void setup() {
  Serial.begin(115200);

  eyes.begin();
  eyes.show();
}

void loop() {
  if (!fading) {
    startFade(red.current >= 0x80 ? random(0, 0x80) : random(red.current, 0x100), random(0, 0x80), random(0, 0x100), random(100, 500));
  }
  
  fade();
  
  for (int eye = 0; eye < 2; eye++) {
    eyes.setPixelColor(eye, red.current, green.current, blue.current);
  }

  Serial << currentStep << "/" << totalSteps << ": " << red.current << ", " << green.current << ", " << blue.current << endl;
  
  eyes.show();

  delay(FADE_STEP);
}
