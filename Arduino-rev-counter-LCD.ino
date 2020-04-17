#include <Servo.h>
#include <Math.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

int tim = 500; //the value of delay time
LiquidCrystal_I2C lcd(0x27, 16, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

Servo driverServo;
int mySwitchPin = 4;
int driverServoPin = 8;
int mySwitchDown = false;
int sensorValue;
int hits = 0;
boolean hitOn = false;
float signalLength;
unsigned long startMillis;
unsigned long lastMillis = 0;
unsigned long currentMillis = 0;
unsigned long deltaMillis = 0;
unsigned long rpmDiff = 60;
float rpm;

void setup() {
 startMillis = millis();
 lcd.init(); //initialize the lcd
 lcd.backlight(); //open the backlight
 Serial.begin(9600);
 pinMode(mySwitchPin, INPUT);
 pinMode (driverServoPin, OUTPUT);
 digitalWrite(mySwitchPin, HIGH);
 driverServo.attach(driverServoPin);
}

void loop() {
 driveForward();
 if (mySwitchDown()) {
 lastMillis = currentMillis;
 currentMillis = millis();
 deltaMillis = currentMillis - lastMillis;
 rpm = 60 / (float) deltaMillis * 1000;
 if (deltaMillis > 400) {
 hits++;
 Serial.print(" CurrentMillis: ");
 Serial.print(currentMillis);
 Serial.print(" LastMillis: ");
 Serial.print(lastMillis);
 Serial.print(" DeltaMillis: ");
 Serial.print(deltaMillis);
 Serial.print(" Hits: ");
 Serial.print(hits);
 Serial.print(" RPM: ");
 Serial.println(rpm);
 workLCD();
 }
 else {
 }
 }
 else {
 hitOn = false;
 }
}

void workLCD() {
 lcd.setCursor(0, 0); // set the cursor to column 0, line 1
 lcd.print("Hits: "); // Print a message to the LCD.
 lcd.print(hits); // Print a message to the LCD.
 lcd.setCursor(0, 1); // set the cursor to column 0, line 0
 lcd.print("Delta: "); // Print a message to the LCD.
 lcd.print(deltaMillis); // Print a message to the LCD.
 lcd.setCursor(0, 2); // set the cursor to column 0, line 0
 lcd.print("RPM: "); // Print a message to the LCD.
 lcd.print(rpm); // Print a message to the LCD.
 lcd.setCursor(0, 3); // set the cursor to column 0, line 2
 lcd.print("Signal: "); // Print a message to the LCD.
 lcd.print(signalLength); // Print a message to the LCD.
}


void driveForward() {  //This function defines the rotation speed
 sensorValue = analogRead(A0); //Read the value from the potentiometer
 signalLength = map(sensorValue, 20, 1023, 1495, 1800); //map it to servo signal length
 driverServo.writeMicroseconds(signalLength); //run the servo at the given signal length
 delay(10);
}


boolean mySwitchDown() {
 mySwitchDown = digitalRead(mySwitchPin);
 if (LOW == mySwitchDown) {
 hitOn = true;
 return true;
 }
 else {
 return false;
 }
}
