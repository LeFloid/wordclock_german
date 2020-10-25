//Library includes
#include <FastLED.h>
#include <Wire.h>
#include <Time.h>
#include <DCF77.h>
#include <IRremote.h>
#include <Time.h>
#include <TimeLib.h>
#include <DS3231.h>
#include <Wire.h>
#include "default_layout.h"

#define BRIGHTNESS 20

//LED defines
#define NUM_LEDS 121

//PIN defines
#define STRIP_DATA_PIN 6
#define ARDUINO_LED 13 //Default Arduino LED
#define LDR_PIN 0

// LED Strip
CRGB leds[NUM_LEDS];
uint8_t strip[NUM_LEDS];
uint8_t stackptr = 0;
CRGB defaultColor = CRGB::White;
// ------------------------

// DS3231 aka Real Time Clock
DS3231 Clock;
// ------------------------

// initial value > 24
// -> 'linda' will be shown with every reboot
uint8_t lastLindaShownHour = 25;


//forward declaration
void fastTest();
void clockLogic();
void doIRLogic();
void doLDRLogic();
void makeParty();
void off();
void showHeart();
void pushToStrip(int ledId);
void resetAndBlack();
void resetStrip();
void displayStripRandomColor();
void displayStrip();
void displayStrip(CRGB colorCode);
void timeToStrip(uint8_t hours, const uint8_t minutes);
void doDCFLogic();
void pushES_IST();
void pushETWA();
void pushGENAU();
void pushFUENF1();
void pushZEHN1();
void pushVIERTEL();
void pushNACH();
void pushVOR();
void pushHALB();
void pushZWOELF();
void pushSIEBEN();
void pushDREI();
void pushFUENF2();
void pushNEUN();
void pushACHT();
void pushZEHN2();
void pushSECHS();
void pushUHR();
void pushZWANZIG();
void pushEINS(bool);
void pushZWEI();
void pushVIER();
void pushELF();
void pushDREI2();

//#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(str)  Serial.println (str);
#else
#define DEBUG_PRINT(str)
#endif

// board: Arduino Nano
// bootloader: ATmega328P (Old Bootloader)

void setup() {
#ifdef DEBUG
  Serial.begin(9600);
#endif
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);

  pinMode(ARDUINO_LED, OUTPUT);

  //setup leds incl. fastled
  for (int i = 0; i < NUM_LEDS; i++) {
    strip[i] = 0;
  }
  FastLED.addLeds<WS2812B, STRIP_DATA_PIN, GRB>(leds, NUM_LEDS);
  resetAndBlack();
  displayStrip();

  // Start the I2C interface for connecting to the DS3231
  Wire.begin();

  // just set the brightness hard coded to an small value since the LEDs otherwise shine over to their neighbor LEDs
  FastLED.setBrightness(BRIGHTNESS);

  // uncomment this line and adjust the method to set the time. afterwards comment out the line and flash once again
  // to make sure the time does not get reset every time the controller is rebooting
  // setTime();
}

void setTime() {
  Clock.setYear(20);
  Clock.setMonth(10);
  Clock.setDate(25);
  Clock.setHour(11);
  Clock.setMinute(23);
  Clock.setSecond(00);
}

void loop() {
  resetAndBlack();
  bool _;
  timeToStrip(Clock.getHour(_, _), Clock.getMinute());
  displayStrip(defaultColor);
  delay(1000);
}

void handleShowLinda() {
  showICH_LIEBE_DICH();
  delay(5000);
  showHeart();
}

void timeToStrip(uint8_t hours, const uint8_t minutes)
{

  if (hours != lastLindaShownHour) {
    handleShowLinda();
    resetAndBlack();

    // remeber that we already did this fun
    lastLindaShownHour = hours;
  }

  const int offset = -2;
  if (minutes < 5) {
    // full hour -> append 'UHR'
    pushUHR();
  }

  pushES_IST();

  if (minutes % 5 != 0) {
    // since the time did not hit an exact value to display, we append an 'ETWA'
    pushETWA();
  } else {
    // we can display the exact time
    pushGENAU();
  }

  if (minutes >= 5 + offset && minutes < 10 + offset) {
    pushFUENF1();
    pushNACH();
  } else if (minutes >= 10 + offset && minutes < 15 + offset) {
    pushZEHN1();
    pushNACH();
  } else if (minutes >= 15 + offset && minutes < 20 + offset) {
    pushVIERTEL();
    pushNACH();
  } else if (minutes >= 20 + offset && minutes < 25 + offset) {
    pushZEHN1();
    pushVOR();
    pushHALB();
  } else if (minutes >= 25 + offset && minutes < 30 + offset) {
    pushFUENF1();
    pushVOR();
    pushHALB();
  } else if (minutes >= 30 + offset && minutes < 35 + offset) {
    pushHALB();
  } else if (minutes >= 35 + offset && minutes < 40 + offset) {
    pushFUENF1();
    pushNACH();
    pushHALB();
  } else if (minutes >= 40 + offset && minutes < 45 + offset) {
    pushZWANZIG();
    pushVOR();
  } else if (minutes >= 45 + offset && minutes < 50 + offset) {
    pushVIERTEL();
    pushVOR();
  } else if (minutes >= 50 + offset && minutes < 55 + offset) {
    pushZEHN1();
    pushVOR();
  } else if (minutes >= 55 + offset && minutes < 60 + offset) {
    pushFUENF1();
    pushVOR();
  }

  if (hours >= 12) {
    // e. g. 24 -> 12
    //       18 -> 6
    hours -= 12;
  }

  if (minutes >= 20 + offset) {
    hours++;
  }

  if (hours == 12) {
    hours = 0;
  }

  //show hours
  switch (hours) {
    case 0:
      pushZWOELF();
      break;
    case 1:
      if (minutes < 5 + offset) {
        // e. g. Es ist etwa ein Uhr
        pushEINS(false);
      } else {
        // e. g. Es ist etwa zwanzig nach eins
        pushEINS(true);
      }
      break;
    case 2:
      pushZWEI();
      break;
    case 3:
      pushDREI2();
      break;
    case 4:
      pushVIER();
      break;
    case 5:
      pushFUENF2();
      break;
    case 6:
      pushSECHS();
      break;
    case 7:
      pushSIEBEN();
      break;
    case 8:
      pushACHT();
      break;
    case 9:
      pushNEUN();
      break;
    case 10:
      pushZEHN2();
      break;
    case 11:
      pushELF();
      break;
  }
}

void off() {
  DEBUG_PRINT("switching off");
  resetAndBlack();
  displayStrip(CRGB::Black);
}

void makeParty() {
  DEBUG_PRINT("YEAH party party");
  resetAndBlack();
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(random(0, 255), 255, 255);
  }
  FastLED.show();
}

void showHeart() {
  DEBUG_PRINT("showing heart");
  resetAndBlack();
  
  // those are the numbers for the heart itself
  pushToStrip(38); pushToStrip(28); pushToStrip(26); pushToStrip(29);
  pushToStrip(25); pushToStrip(41); pushToStrip(35);
  pushToStrip(46); pushToStrip(52); pushToStrip(58); pushToStrip(62);
  pushToStrip(70); pushToStrip(72); pushToStrip(82);

  // those are the numbers for the frame
  // horizontal lines
  for (uint8_t i = 0; i < 121; i++) {
    if (i == 11) {
      // now we have to switch to the last line
      i = 110;
    }
    pushToStrip(i);
  }

  // vertical lines
  for (uint8_t i = 11; i <= 132; i += 11) {
    if (i == 110) {
      // now we have to switch from the left line to the right line
      i = 21;
    }
    pushToStrip(i);
  }

  displayStrip(CRGB::Red);

  // Let the heart pulsate
  bool forward = true;
  for (int i = 0, j = 20; i <= 100; i++) {

    if (j >= 235) {
      forward = false;
    } else if (j < 50) {
      forward = true;
    }
    if (forward) {
      j += 10;
    } else {
      j -= 10;
    }
    Serial.println(j);

    FastLED.setBrightness(j);
    FastLED.show();
    delay(80);
  }

  FastLED.setBrightness(BRIGHTNESS);
}

void pushToStrip(int ledId) {
  if (ledId > NUM_LEDS) {
    return;
  }
  DEBUG_PRINT(ledId);
  strip[stackptr] = ledId;
  stackptr++;
}

void resetAndBlack() {
  resetStrip();
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
}

void resetStrip() {
  stackptr = 0;
  for (int i = 0; i < NUM_LEDS; i++) {
    strip[i] = 0;
  }
}

void displayStripRandomColor() {
  for (int i = 0; i < stackptr; i++) {
    leds[strip[i]] = CHSV(random(0, 255), 255, 255);
  }
  FastLED.show();
}

void displayStrip() {
  displayStrip(defaultColor);
}

void displayStrip(CRGB colorCode) {
  for (int i = 0; i < stackptr; i++) {
    leds[strip[i]] = colorCode;
  }
  FastLED.show();
}

///////////////////////
//PUSH WORD HELPER
///////////////////////

///////////////////////
// LINE 0
///////////////////////
void pushES_IST()  {
  pushToStrip(L0);
  pushToStrip(L1);
  pushToStrip(L3);
  pushToStrip(L4);
  pushToStrip(L5);
}

void pushETWA() {
  pushToStrip(L7);
  pushToStrip(L8);
  pushToStrip(L9);
  pushToStrip(L10);
}

///////////////////////
// LINE 1
///////////////////////

void pushGENAU() {
  pushToStrip(16);
  pushToStrip(17);
  pushToStrip(18);
  pushToStrip(19);
  pushToStrip(L20);
}

void pushFUENF1() {
  pushToStrip(11);
  pushToStrip(12);
  pushToStrip(13);
  pushToStrip(14);
}

///////////////////////
// LINE 2
///////////////////////

void pushZEHN1() {
  pushToStrip(22);
  pushToStrip(23);
  pushToStrip(24);
  pushToStrip(25);
}

void pushZWANZIG() {
  pushToStrip(26);
  pushToStrip(27);
  pushToStrip(28);
  pushToStrip(29);
  pushToStrip(L30);
  pushToStrip(31);
  pushToStrip(32);
}

///////////////////////
// LINE 3
///////////////////////

void pushDREI() {
  pushToStrip(L40);
  pushToStrip(41);
  pushToStrip(42);
  pushToStrip(43);
}

void pushVIERTEL() {
  pushToStrip(33);
  pushToStrip(34);
  pushToStrip(35);
  pushToStrip(36);
  pushToStrip(37);
  pushToStrip(38);
  pushToStrip(39);
}

///////////////////////
// LINE 4
///////////////////////

void pushNACH() {
  pushToStrip(46);
  pushToStrip(47);
  pushToStrip(48);
  pushToStrip(49);
}

void pushVOR() {
  pushToStrip(50);
  pushToStrip(51);
  pushToStrip(52);
}

///////////////////////
// LINE 5
///////////////////////

void pushHALB() {
  pushToStrip(62);
  pushToStrip(63);
  pushToStrip(64);
  pushToStrip(65);
}

void pushZWOELF() {
  pushToStrip(56);
  pushToStrip(57);
  pushToStrip(58);
  pushToStrip(59);
  pushToStrip(L60);
}

///////////////////////
// LINE 6
///////////////////////

void pushZWEI() {
  pushToStrip(66);
  pushToStrip(67);
  pushToStrip(68);
  pushToStrip(69);
}

void pushEINS(bool s) {
  pushToStrip(68);
  pushToStrip(69);
  pushToStrip(L70);
  if (s) {
    pushToStrip(71);
  }
}

void pushSIEBEN() {
  pushToStrip(71);
  pushToStrip(72);
  pushToStrip(73);
  pushToStrip(74);
  pushToStrip(75);
  pushToStrip(76);
}

///////////////////////
// LINE 7
///////////////////////

void pushDREI2() {
  pushToStrip(83);
  pushToStrip(84);
  pushToStrip(85);
  pushToStrip(86);
}

void pushFUENF2() {
  pushToStrip(77);
  pushToStrip(78);
  pushToStrip(79);
  pushToStrip(L80);
}

///////////////////////
// LINE 8
///////////////////////

void pushELF() {
  pushToStrip(88);
  pushToStrip(89);
  pushToStrip(L90);
}

void pushNEUN() {
  pushToStrip(91);
  pushToStrip(92);
  pushToStrip(93);
  pushToStrip(94);
}

void pushVIER() {
  pushToStrip(95);
  pushToStrip(96);
  pushToStrip(97);
  pushToStrip(98);
}

///////////////////////
// LINE 9
///////////////////////

void pushACHT() {
  pushToStrip(105);
  pushToStrip(106);
  pushToStrip(107);
  pushToStrip(108);
}

void pushZEHN2() {
  pushToStrip(101);
  pushToStrip(102);
  pushToStrip(103);
  pushToStrip(104);
}

///////////////////////
// LINE 10
///////////////////////

void pushSECHS() {
  pushToStrip(111);
  pushToStrip(112);
  pushToStrip(113);
  pushToStrip(114);
  pushToStrip(115);
}

void pushUHR() {
  pushToStrip(118);
  pushToStrip(119);
  pushToStrip(120);
}

///////////////////////
// MULTI LINE
///////////////////////
void showICH_LIEBE_DICH() {
  resetAndBlack();

  // ICH
  leds[40] = defaultColor;
  leds[48] = defaultColor;
  leds[49] = defaultColor;

  // LIEBE
  leds[63] = CRGB::Yellow;
  leds[72] = CRGB::Yellow;
  leds[73] = CRGB::Yellow;
  leds[74] = CRGB::Yellow;
  leds[75] = CRGB::Yellow;

  // DICH
  leds[81] = defaultColor;
  leds[82] = defaultColor;
  leds[83] = defaultColor;
  leds[86] = defaultColor;

  // LINDA
  leds[89] = CRGB::Yellow;
  leds[96] = CRGB::Yellow;
  leds[99] = CRGB::Yellow;
  leds[100] = CRGB::Yellow;
  leds[101] = CRGB::Yellow;
  FastLED.show();
}
