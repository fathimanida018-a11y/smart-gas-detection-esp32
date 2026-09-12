#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"

// OLED SETTINGS
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// SENSOR PINS
#define MQ2_PIN 34
#define BUZZER 26
#define FLAME_SENSOR 27

// DHT11 SETTINGS
#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {

  Serial.begin(115200);

  // PIN MODES
  pinMode(BUZZER, OUTPUT);
  pinMode(FLAME_SENSOR, INPUT);

  // START DHT
  dht.begin();

  // START OLED
  Wire.begin(21, 22);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {

    Serial.println("OLED FAILED");

    while(true);
  }

  // START SCREEN
  display.clearDisplay();

  display.setTextSize(1);

  display.setTextColor(SSD1306_WHITE);

  display.setCursor(10,10);
  display.println("SMART SAFETY");

  display.setCursor(10,25);
  display.println("SYSTEM");

  display.display();

  delay(2000);
}

void loop() {

  // READ VALUES
  int gasValue = analogRead(MQ2_PIN);

  int flameValue = digitalRead(FLAME_SENSOR);

  float temperature = dht.readTemperature();

  float humidity = dht.readHumidity();

  // SERIAL MONITOR
  Serial.print("Gas Value: ");
  Serial.println(gasValue);

  Serial.print("Temperature: ");
  Serial.println(temperature);

  Serial.print("Humidity: ");
  Serial.println(humidity);

  // OLED DISPLAY
  display.clearDisplay();

  display.setTextSize(1);

  display.setCursor(0,0);
  display.println("SMART SAFETY");

  display.setCursor(0,12);
  display.print("Temp: ");
  display.print(temperature);
  display.println(" C");

  display.setCursor(0,24);
  display.print("Humidity: ");
  display.print(humidity);
  display.println(" %");

  display.setCursor(0,36);
  display.print("Gas: ");
  display.println(gasValue);

  // FIRE DETECTION
  if(flameValue == LOW) {

    digitalWrite(BUZZER, HIGH);

    display.setCursor(0,52);
    display.println("FIRE DETECTED!");
  }

  // GAS DETECTION
  else if(gasValue > 1500) {

    digitalWrite(BUZZER, HIGH);

    display.setCursor(0,52);
    display.println("TURN ON FAN!");
  }

  // SAFE CONDITION
  else {

    digitalWrite(BUZZER, LOW);

    display.setCursor(0,52);
    display.println("AIR SAFE");
  }

  display.display();

  delay(1000);
}
