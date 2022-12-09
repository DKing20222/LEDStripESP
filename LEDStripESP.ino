#include <FastLED.h>
#include <Key.h>
#include <Keypad.h>

//Ultrasonic sensor
#define echoPin 14
#define trigPin 27
long duration;
int distance;
byte ModeSwitchDistance = 30;

//Sound sensor
#define AudioPin 13
bool SoundIO = 0;

//Keypad
const byte rows = 1;
const byte cols = 4;
char keys[rows][cols] = {
  {'1', '2', '3', '4'},
};
byte rowPins[rows] = {25};
byte colPins[cols] = {33, 32, 35, 34};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols );

//LED
#define NUM_LEDS 240
#define LEDPin 12

//Loop
int Delay = 200;
int LoopDelay = 50;

//Keys
byte LEDMode = 1;
byte LEDColor = 2;
int LineBPM = 7.5;
byte LEDBrightness = 255;

int beatsinCorrection = -65536 / 4;
byte myHue = 0;
byte juggleColorsNum = 8;

CHSV breathColor;
CHSV stillColor;
CHSV gradientFirst;
CHSV gradientSecond;

DEFINE_GRADIENT_PALETTE(firstPalette) {
  0, 0, 0, 0,
  64, 255, 0, 0,
  128, 0, 255, 0,
  255, 0, 0, 255
};
CRGBPalette16 jugglePalette = firstPalette;


CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(9600);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(AudioPin, INPUT);
  pinMode(LEDPin, OUTPUT);
  FastLED.addLeds<NEOPIXEL, LEDPin>(leds, NUM_LEDS);
  FastLED.setBrightness(LEDBrightness);
  FastLED.setTemperature(ClearBlueSky);
  FastLED.setCorrection(TypicalPixelString);
  ResetLED();
}
void loop () {
  SoundIO = digitalRead(AudioPin);
  /*EVERY_N_SECONDS(LoopDelay / 10) {
    Ultrasonic();
  }*/
  char key = keypad.getKey();
  switch (key) {
    case '1': {
      Serial.println('1');
        LEDMode += 1;
        LEDColor = 2;
        ResetLED();
        break;
      }
    case '2': {
      Serial.println('2');
        LEDColor += 1;
        ResetLED();
        break;
      }
    case '3': {
      Serial.println('3');
        LineBPM += 1.5;
        if (LineBPM > 20) {
          LineBPM = 1.5;
        }
        break;
      }
    case '4': {
      Serial.println('4');
        LEDBrightness += 25;
        if (LEDBrightness > 250) {
          LEDBrightness = 25;
        }
        break;
      }
  }
  switch (LEDMode) {
    case 1: {
        RGBContinuous();
        break;
      }
    case 2: {
        Breath();
        SoundModeSwitch();
        break;
      }
    case 3: {
        RainbowPulse();
        break;
      }
    case 4: {
        Juggle();
        break;
      }
    case 5: {
        Firework();
        break;
      }
    case 6: {
        LineOneColor();
        SoundModeSwitch();
        break;
      }
    case 7: {
        LineRainbow();
        break;
      }
    case 8: {
        LineCollision();
        break;
      }
    case 9: {
        StillRainbow();
        break;
      }
    case 10: {
        StillSolid();
        SoundModeSwitch();
        break;
      }
    case 11: {
        Gradient();
        SoundModeSwitch();
        break;
      }
    case 12: {
        LEDMode = 1;
        break;
      }
  }
  FastLED.show();
}
void ResetLED() {
  fill_solid(leds, NUM_LEDS, CRGB(0, 0, 0));
}
void SoundModeSwitch() {
  if (SoundIO == HIGH) {
    Serial.println("HENLOOO");
    LEDColor += 1;
    ResetLED();
  }
}
void Ultrasonic() {
  digitalWrite(trigPin, LOW);
  digitalWrite(trigPin, HIGH);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  Serial.println(distance);
  if ((distance < ModeSwitchDistance) && (distance > 0)) {
    LEDMode += 1;
  }
}
/* EFFECTS */

void RGBContinuous() {
  EVERY_N_MILLISECONDS(LoopDelay) {
    fill_solid(leds, NUM_LEDS, CHSV(myHue, 255, LEDBrightness));
    FastLED.show();
    myHue++;
  }
}
void RGBMoving() {
  FastLED.setBrightness(LEDBrightness);
  fill_rainbow(leds, NUM_LEDS, millis() / (2 * LineBPM), 1);
}

void Breath() {
  float breath = beatsin16(LineBPM, 64, 255, 0, beatsinCorrection);
  float breathSpecial = beatsin16(LineBPM, 64, 190, 0, beatsinCorrection);
  CHSV breathWhite = CHSV(0, 0, breathSpecial);
  CHSV breathRed = CHSV(0, 255, breath);
  CHSV breathGreen = CHSV(85, 255, breath);
  CHSV breathBlue = CHSV (170, 255, breath);
  CHSV breathYellow = CHSV(42.5 , 255, breath);
  CHSV breathCyan = CHSV(127.5 , 255, breath);
  CHSV breathPurple = CHSV(205 , 255, breath);
  CHSV breathPink = CHSV(227, 255, breath);
  CHSV breathOrange = CHSV(15, 255, breath);
  switch (LEDColor) {
    case 1: {
        breathColor = breathWhite;
        break;
      }
    case 2: {
        breathColor = breathRed;
        break;
      }
    case 3: {
        breathColor = breathGreen;
        break;
      }
    case 4: {
        breathColor = breathBlue;
        break;
      }
    case 5: {
        breathColor = breathYellow;
        break;
      }
    case 6: {
        breathColor = breathCyan;
        break;
      }
    case 7: {
        breathColor = breathPurple;
        break;
      }
    case 8: {
        breathColor = breathPink;
        break;
      }
    case 9: {
        breathColor = breathOrange;
        break;
      }
    case 10: {
        LEDColor = 1;
        break;
      }
  }
  fill_solid(leds, NUM_LEDS, breathColor);
}
void Pulse() {
  int j = NUM_LEDS / 2;
  for (uint8_t i = NUM_LEDS / 2; i < NUM_LEDS; i++) {
    leds[i] = CHSV(millis() / 10, 255, LEDBrightness);
    leds[j] = CHSV(millis() / 10, 255, LEDBrightness);
    j--;
    fadeToBlackBy(leds, NUM_LEDS, 2 * LineBPM);
    FastLED.show();
    delay(20);
  }
}

void RainbowPulse()
{
  int pos = beatsin16(LineBPM, 0, NUM_LEDS - 1, 0, beatsinCorrection);
  leds[pos] = CHSV (myHue, 255, LEDBrightness);
  EVERY_N_MILLISECONDS(NUM_LEDS / 10) {
    myHue++;
  }
}

void Juggle() {
  FastLED.setBrightness(LEDBrightness);
  fadeToBlackBy(leds, NUM_LEDS, juggleColorsNum * (juggleColorsNum / 2));
  for (int i = 1; i <= juggleColorsNum; i++) {
    leds[beatsin16(i + 3 , 0, NUM_LEDS - 1, 0, beatsinCorrection )] |= ColorFromPalette(jugglePalette, i * (255 / juggleColorsNum));
  }
}
void Firework()
{
  FastLED.setBrightness(LEDBrightness);
  EVERY_N_MILLISECONDS(random8(30, 70)) {
    fadeToBlackBy(leds, NUM_LEDS, 80);
    int pos = random16(0, NUM_LEDS);
    int fireworkColor = random16(0, 255);
    for (int i = 0; i < random(2, 7); i++) {
      leds[pos + i] += ColorFromPalette(jugglePalette, fireworkColor);
    }
  }
}

void LineOneColor()
{
  CHSV stillWhite = CHSV(0, 0, LEDBrightness);
  CHSV stillRed = CHSV(0, 255, LEDBrightness);
  CHSV stillGreen = CHSV(85, 255, LEDBrightness);
  CHSV stillBlue = CHSV (170, 255, LEDBrightness);
  CHSV stillYellow = CHSV(42.5 , 255, LEDBrightness);
  CHSV stillCyan = CHSV(127.5 , 255, LEDBrightness);
  CHSV stillPurple = CHSV(205 , 255, LEDBrightness);
  CHSV stillPink = CHSV(227, 255, LEDBrightness);
  CHSV stillOrange = CHSV(15, 255, LEDBrightness);
  switch (LEDColor) {
    case 1: {
        stillColor = stillWhite;
        break;
      }
    case 2: {
        stillColor = stillRed;
        break;
      }
    case 3: {
        stillColor = stillGreen;
        break;
      }
    case 4: {
        stillColor = stillBlue;
        break;
      }
    case 5: {
        stillColor = stillYellow;
        break;
      }
    case 6: {
        stillColor = stillCyan;
        break;
      }
    case 7: {
        stillColor = stillPurple;
        break;
      }
    case 8: {
        stillColor = stillPink;
        break;
      }
    case 9: {
        stillColor = stillOrange;
        break;
      }
    case 10: {
        LEDColor = 1;
        break;
      }
  }
  int pos = beatsin16(LineBPM, 0, NUM_LEDS - 1, 0, beatsinCorrection);
  fadeToBlackBy(leds, NUM_LEDS, 2 * LineBPM);
  leds[pos] = stillColor;
}

void LineRainbow()
{
  int pos = beatsin16(LineBPM, 0, NUM_LEDS - 1, 0, beatsinCorrection);
  fadeToBlackBy(leds, NUM_LEDS, 2 * LineBPM);
  leds[pos] = CHSV (myHue, 255, LEDBrightness);
  EVERY_N_MILLISECONDS(LineBPM * (NUM_LEDS / 100)) {
    myHue++;
  }
}

void LineCollision()
{
  int pos = beatsin16(LineBPM, 0, NUM_LEDS - 1, 0, beatsinCorrection);
  fadeToBlackBy(leds, NUM_LEDS, 2 * LineBPM);
  leds[pos] = CHSV (myHue, 255, LEDBrightness);
  if ((pos == NUM_LEDS - 1) || (pos == 0)) {
    if (LineBPM <= 5) {
      delay(LineBPM * LineBPM * LineBPM);
    }
    else {
      delay(LineBPM * (LineBPM - (LineBPM / 2)));
    }
    myHue = myHue + LineBPM;
  }
}

/* MY STILL COLORS */
void StillRainbow()
{
  FastLED.setBrightness(LEDBrightness);
  fill_rainbow(leds, NUM_LEDS, 0, 255 / NUM_LEDS);
}

void StillSolid() {
  CHSV stillWhite = CHSV(0, 0, LEDBrightness - 50);
  CHSV stillRed = CHSV(0, 255, LEDBrightness);
  CHSV stillGreen = CHSV(85, 255, LEDBrightness);
  CHSV stillBlue = CHSV (170, 255, LEDBrightness);
  CHSV stillYellow = CHSV(42.5 , 255, LEDBrightness);
  CHSV stillCyan = CHSV(127.5 , 255, LEDBrightness);
  CHSV stillPurple = CHSV(205 , 255, LEDBrightness);
  CHSV stillPink = CHSV(227, 255, LEDBrightness);
  CHSV stillOrange = CHSV(15, 255, LEDBrightness);
  switch (LEDColor) {
    case 1: {
        stillColor = stillWhite;
        break;
      }
    case 2: {
        stillColor = stillRed;
        break;
      }
    case 3: {
        stillColor = stillGreen;
        break;
      }
    case 4: {
        stillColor = stillBlue;
        break;
      }
    case 5: {
        stillColor = stillYellow;
        break;
      }
    case 6: {
        stillColor = stillCyan;
        break;
      }
    case 7: {
        stillColor = stillPurple;
        break;
      }
    case 8: {
        stillColor = stillPink;
        break;
      }
    case 9: {
        stillColor = stillOrange;
        break;
      }
    case 10: {
        LEDColor = 1;
        break;
      }
  }
  fill_solid(leds, NUM_LEDS, stillColor);
}

/*MY GRADIENT COLORS*/

void Gradient() {
  CHSV gradientRed = CHSV(0, 255, LEDBrightness);
  CHSV gradientGreen = CHSV(85, 255, LEDBrightness);
  CHSV gradientBlue = CHSV(170, 255, LEDBrightness);
  switch (LEDColor) {
    case 1: {
        gradientFirst = gradientRed;
        gradientSecond = gradientGreen;
        break;
      }
    case 2: {
        gradientFirst = gradientRed;
        gradientSecond = gradientBlue;
        break;
      }
    case 3: {
        gradientFirst = gradientGreen;
        gradientSecond = gradientBlue;
        break;
      }
    case 4: {
        LEDColor = 1;
        break;
      }
  }
  fill_gradient_RGB(leds, 0, gradientFirst, NUM_LEDS - 1, gradientSecond);
}
