#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Initialize the LCD (I2C address: 0x27, 16 columns, 2 rows)
LiquidCrystal_I2C lcd(0x27, 16, 2); // Address: 0x27, columns: 16, rows: 2

// Define IR sensor pins
const int irSensor1 = 2; // Parking space 1
const int irSensor2 = 3; // Parking space 2
const int irSensor3 = 4; // Parking space 3
const int gateSensor = 5; // IR sensor for detecting a car at the gate

// Servo motor for the gate
Servo gateServo;
const int servoPin = 6; // Pin connected to the servo

void setup() {
  Serial.begin(9600);

  // Initialize IR sensor pins as inputs
  pinMode(irSensor1, INPUT);
  pinMode(irSensor2, INPUT);
  pinMode(irSensor3, INPUT);
  pinMode(gateSensor, INPUT);

  // Attach the servo to the designated pin
  gateServo.attach(servoPin);
  gateServo.write(0); // Initial position (gate closed)

  // Initialize the LCD
  lcd.begin(); // No arguments for this library version
  lcd.backlight();

  // Display initial message
  lcd.setCursor(0, 0);
  lcd.print("Car Parking");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Read the state of the parking space IR sensors
  bool spot1 = digitalRead(irSensor1); // Parking space 1 status
  bool spot2 = digitalRead(irSensor2); // Parking space 2 status
  bool spot3 = digitalRead(irSensor3); // Parking space 3 status
  bool carAtGate = digitalRead(gateSensor); // Check for a car at the gate

  // Display the parking spot statuses on the LCD
  lcd.setCursor(0, 0);
  lcd.print("PS1:");
  lcd.print(spot1 == HIGH ? "FREE" : "OCCP");

  lcd.setCursor(8, 0);
  lcd.print("PS2:");
  lcd.print(spot2 == HIGH ? "FREE" : "OCCP");

  lcd.setCursor(0, 1);
  lcd.print("PS3:");
  lcd.print(spot3 == HIGH ? "FREE" : "OCCP");

  // Check if all parking spots are full
  bool allSpotsFull = (spot1 == LOW && spot2 == LOW && spot3 == LOW);

  // Gate control logic (ONLY controlled by gateSensor)
  if (carAtGate == LOW) { 
    if (allSpotsFull) {
      // Gate remains closed if all spots are full
      lcd.setCursor(10, 1);
      lcd.print("Gate:Closed");
      gateServo.write(0); // Keep the gate closed (servo at 0°)
    } else {
      // Open the gate if there's at least one free parking spot
      lcd.setCursor(10, 1);
      lcd.print("Gate:Open ");
      gateServo.write(90); // Open the gate (servo at 90°)
      delay(3000);         // Keep the gate open for 3 seconds
      //gateServo.write(0);  // Close the gate (servo back to 0°)
    }
  } else {
    // Ensure the gate remains closed when no car is detected at the gate
    lcd.setCursor(10, 1);
    lcd.print("Gate:Idle  ");
    gateServo.write(0); // Ensure the gate remains closed
  }

  delay(500); // Short delay to update readings
}
