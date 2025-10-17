#include <Arduino.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

// Define pins
#define DHTPIN 2
#define DHTTYPE DHT22
#define TRIG_PIN 9
#define ECHO_PIN 10
#define RED_LED 6
#define GREEN_LED 7
#define BUZZER 8

// Initialize
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);

long duration;
int distance;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  lcd.init();
  lcd.backlight();
  dht.begin();

  lcd.setCursor(0, 0);
  lcd.print("Smart Weather");
  lcd.setCursor(0, 1);
  lcd.print("& Object Alert");
  delay(2000);
  lcd.clear();
}

void loop() {
  // --- Measure temperature and humidity ---
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  // --- Measure distance ---
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;

  // --- Display readings ---
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temp, 1);
  lcd.print("C H:");
  lcd.print(hum, 0);
  lcd.print("%   ");

  lcd.setCursor(0, 1);
  lcd.print("D:");
  lcd.print(distance);
  lcd.print("cm ");

  // --- Temperature Condition ---
  if (temp > 35) {
    lcd.print("HighTemp!");
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    tone(BUZZER, 1000);
  } else {
    lcd.print("Normal   ");
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    noTone(BUZZER);
  }

  // --- Object Detection ---
  if (distance <= 10) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Object Detected!");
    tone(BUZZER, 1500);
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    delay(2000);
    lcd.clear();
  } else {
    noTone(BUZZER);
  }

  delay(500);
}
