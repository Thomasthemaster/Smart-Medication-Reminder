#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>
#include <DIYables_OLED_SSD1309.h>

#define SCREEN_WIDTH 128
#define SCREEN_LENGTH 64
#define OLED_RESET -1

DIYables_OLED_SSD1309 display(
  SCREEN_WIDTH, SCREEN_LENGTH, &Wire, OLED_RESET
);

RTC_DS1307 rtc;

int ackButtonVal;
int alertVal = 0;

int redLED = 2;
int greenLED = 3;
int ackButton = 4;
int buzzPin = 12;
int vibMotor = 10;

int setUpDelay = 2000;
int alertDelay = 150;
int shortDelay = 450;
long medToMedTime;

void setup() {
  randomSeed(analogRead(A0));
  Serial.begin(9600);
  Wire.begin();

  if (!rtc.begin()) {
    Serial.println("RTC not found!");
  }

  if (!display.begin(SSD1309_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found!");
    while (true) {}
  }

  display.clearDisplay();
  display.setTextColor(SSD1309_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Medication Reminder");
  display.println("Clock starting...");
  display.display();
  
  delay(setUpDelay);

  pinMode(buzzPin, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(vibMotor, OUTPUT);
  pinMode(ackButton, INPUT_PULLUP);

  digitalWrite(greenLED, LOW);
  digitalWrite(redLED, LOW);
  digitalWrite(vibMotor, LOW);

  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop() {
  ackButtonVal = digitalRead(ackButton);
  display.clearDisplay();
  while (ackButtonVal == 1 && alertVal == 0) {
    display.setCursor(0, 0);
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, HIGH);
    digitalWrite(vibMotor, HIGH);
    delay(alertDelay);
    digitalWrite(redLED, LOW);
    tone(buzzPin, 1000, 200);
    delay(alertDelay);
    display.setTextSize(2);
    display.println("ALERT");
    display.setTextSize(1);
    display.println("Time to take your");
    display.println("medication! Press");
    display.println("the white button to");
    display.println("acknowledge!!");
    display.display();
    ackButtonVal = digitalRead(ackButton);
  }
  alertVal++;

  digitalWrite(greenLED, HIGH);
  digitalWrite(redLED, LOW);
  ackButtonVal = digitalRead(ackButton);
  delay(shortDelay);
  if (ackButtonVal == 0) {
    alertVal = 0;
  }
  digitalWrite(vibMotor, LOW);

  DateTime now = rtc.now();

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Medication Reminder");
  display.println();

  display.setTextSize(2);
  display.print(now.hour() < 10 ? "0" : "");
  display.print(now.hour());
  display.print(":");
  display.print(now.minute() < 10 ? "0" : "");
  display.print(now.minute());
  display.print(":");
  display.print(now.second() < 10 ? "0" : "");
  display.println(now.second());

  display.setTextSize(1);
  display.println();
  display.print(now.month());
  display.print("/");
  display.print(now.day());
  display.print("/");
  display.println(now.year());

  display.display();

  medToMedTime = random(50000, 75000);
  delay(medToMedTime);
}


