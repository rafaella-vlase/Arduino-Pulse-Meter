#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <TimerOne.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int PULSE_SENSOR_PIN = 0;
const int LED_PIN = 13;


volatile boolean beatHappened = false;
volatile boolean interruptsEnabled = true;

void enableInterrupts() {
  interruptsEnabled = true;
}

void disableInterrupts() {
  interruptsEnabled = false;
}

void onBeatDetected() {
  if (interruptsEnabled) {
    beatHappened = true;
  }
}

void printHeart() {
  lcd.write(byte(0));
}

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  byte heart[8] = {
    0b00000,
    0b01010,
    0b11111,
    0b11111,
    0b11111,
    0b01110,
    0b00100,
    0b00000
  };
  lcd.createChar(0, heart);

  pinMode(LED_PIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(PULSE_SENSOR_PIN), onBeatDetected, RISING);

  Timer1.initialize(1000000);
  Timer1.attachInterrupt(onBeatDetected);

  if (pulseSensorBegin()) {
    Serial.println("Pulse sensor initialized successfully!");
  }
}

void loop() {
  lcd.setCursor(0, 0);
  printHeart();
  lcd.print("BPM: ");

  int currentBPM = getCurrentBPM();

  if (beatHappened) {
    noInterrupts();
    beatHappened = false;
    interrupts();

    Serial.println("♥ A HeartBeat Happened!");
    Serial.print("BPM: ");
    Serial.println(currentBPM);

    lcd.clear();
    lcd.setCursor(0, 0);
    printHeart();
    lcd.print("BPM: ");
    lcd.print(currentBPM);
  }
}

bool pulseSensorBegin() {
  // Initialize Pulse Sensor 
  return true; 
}

int getCurrentBPM() {
  int sensorValue = analogRead(PULSE_SENSOR_PIN); // Read sensor data 

  // Calculate BPM
  int bpm = map(sensorValue, 0, 1023, 40, 200); 
  return bpm;
}
