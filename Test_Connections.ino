/*
 * Glacier System - Breadboard Component Testing
 * Test each component individually before full integration
 * 
 * Author: Glacier Project Team
 * Date: August 6, 2025
 */

// Pin Definitions (same as main project)
#define WATER_SENSOR_PIN 34      // Water Level Sensor (Analog Input)
#define SS_PIN 21                // RFID SDA Pin
#define RST_PIN 22               // RFID Reset Pin
#define PATHWAY_LEDS_PIN 15      // Pathway LEDs (Blinking Group)
#define SAFE_ZONE_LEDS_PIN 2     // Safe Zone LEDs (Static Group)
#define YELLOW_LED_PIN 4         // River Pole Yellow LED (Level 1)
#define RED_LED_PIN 5            // River Pole Red LED (Level 2)
#define SERVO1_PIN 25            // Servo 1 - Small Canal Gate
#define SERVO2_PIN 26            // Servo 2 - Modular River Gate
#define SERVO3_PIN 27            // Servo 3 - Underground Gate
#define BUZZER_PIN 18            // Active Buzzer

// Test mode selection (uncomment ONE test at a time)
#define TEST_LEDS           // Test all LED connections
// #define TEST_BUZZER         // Test buzzer functionality
// #define TEST_SERVOS         // Test servo motor control
// #define TEST_WATER_SENSOR   // Test water sensor readings
// #define TEST_ALL_COMPONENTS // Test everything together

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("🔧 GLACIER SYSTEM - BREADBOARD TESTING");
  Serial.println("=====================================");
  
#ifdef TEST_LEDS
  testLEDs();
#endif

#ifdef TEST_BUZZER
  testBuzzer();
#endif

#ifdef TEST_SERVOS
  testServos();
#endif

#ifdef TEST_WATER_SENSOR
  testWaterSensor();
#endif

#ifdef TEST_ALL_COMPONENTS
  testAllComponents();
#endif
}

void loop() {
#ifdef TEST_LEDS
  // Blink pattern for LED test
  static unsigned long lastBlink = 0;
  static bool ledState = false;
  
  if (millis() - lastBlink > 1000) {
    ledState = !ledState;
    digitalWrite(PATHWAY_LEDS_PIN, ledState);
    digitalWrite(SAFE_ZONE_LEDS_PIN, ledState);
    digitalWrite(YELLOW_LED_PIN, ledState);
    digitalWrite(RED_LED_PIN, ledState);
    
    Serial.println("LEDs: " + String(ledState ? "ON" : "OFF"));
    lastBlink = millis();
  }
#endif

#ifdef TEST_WATER_SENSOR
  // Continuous water sensor reading
  int rawValue = analogRead(WATER_SENSOR_PIN);
  Serial.println("Water Sensor Raw: " + String(rawValue) + 
                " | Voltage: " + String((rawValue * 3.3) / 4095.0, 2) + "V");
  delay(500);
#endif

#ifdef TEST_ALL_COMPONENTS
  runComponentCycle();
#endif
}

void testLEDs() {
  Serial.println("🚥 Testing LED Connections...");
  
  // Initialize LED pins
  pinMode(PATHWAY_LEDS_PIN, OUTPUT);
  pinMode(SAFE_ZONE_LEDS_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  
  Serial.println("✅ LED pins initialized");
  Serial.println("Watch for blinking pattern on all LEDs...");
  
  // Test each LED individually
  Serial.println("Testing individual LEDs:");
  
  Serial.println("- Pathway LED (GPIO 15)");
  digitalWrite(PATHWAY_LEDS_PIN, HIGH);
  delay(1000);
  digitalWrite(PATHWAY_LEDS_PIN, LOW);
  
  Serial.println("- Safe Zone LED (GPIO 2)");
  digitalWrite(SAFE_ZONE_LEDS_PIN, HIGH);
  delay(1000);
  digitalWrite(SAFE_ZONE_LEDS_PIN, LOW);
  
  Serial.println("- Yellow LED (GPIO 4)");
  digitalWrite(YELLOW_LED_PIN, HIGH);
  delay(1000);
  digitalWrite(YELLOW_LED_PIN, LOW);
  
  Serial.println("- Red LED (GPIO 5)");
  digitalWrite(RED_LED_PIN, HIGH);
  delay(1000);
  digitalWrite(RED_LED_PIN, LOW);
  
  Serial.println("Individual LED test complete!");
  Serial.println("Now starting continuous blink pattern...");
}

void testBuzzer() {
  Serial.println("🔊 Testing Buzzer Connection...");
  
  pinMode(BUZZER_PIN, OUTPUT);
  
  Serial.println("Testing buzzer patterns:");
  
  // Test 1: Single beep
  Serial.println("- Single beep");
  digitalWrite(BUZZER_PIN, HIGH);
  delay(200);
  digitalWrite(BUZZER_PIN, LOW);
  delay(1000);
  
  // Test 2: Three beeps
  Serial.println("- Three beeps");
  for (int i = 0; i < 3; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(150);
    digitalWrite(BUZZER_PIN, LOW);
    delay(150);
  }
  delay(1000);
  
  // Test 3: Rapid beeps (like glacier warning)
  Serial.println("- Rapid beeps (glacier simulation)");
  for (int i = 0; i < 5; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
    delay(100);
  }
  
  Serial.println("✅ Buzzer test complete!");
}

void testServos() {
  Serial.println("🔧 Testing Servo Motor Control...");
  
  // Initialize servo PWM
  ledcAttach(SERVO1_PIN, 50, 16);
  ledcAttach(SERVO2_PIN, 50, 16);
  ledcAttach(SERVO3_PIN, 50, 16);
  
  Serial.println("✅ Servo PWM initialized");
  
  // Test servo positions
  Serial.println("Testing servo positions:");
  
  Serial.println("- Moving to 0° (closed)");
  setServoAngle(SERVO1_PIN, 0);
  setServoAngle(SERVO2_PIN, 0);
  setServoAngle(SERVO3_PIN, 0);
  delay(2000);
  
  Serial.println("- Moving to 90° (half-open)");
  setServoAngle(SERVO1_PIN, 90);
  setServoAngle(SERVO2_PIN, 90);
  setServoAngle(SERVO3_PIN, 90);
  delay(2000);
  
  Serial.println("- Moving to 180° (full-open)");
  setServoAngle(SERVO1_PIN, 180);
  setServoAngle(SERVO2_PIN, 180);
  setServoAngle(SERVO3_PIN, 180);
  delay(2000);
  
  Serial.println("- Returning to 0° (closed)");
  setServoAngle(SERVO1_PIN, 0);
  setServoAngle(SERVO2_PIN, 0);
  setServoAngle(SERVO3_PIN, 0);
  delay(1000);
  
  Serial.println("✅ Servo test complete!");
}

void testWaterSensor() {
  Serial.println("💧 Testing Water Sensor...");
  
  pinMode(WATER_SENSOR_PIN, INPUT);
  
  Serial.println("✅ Water sensor pin initialized");
  Serial.println("Reading continuous values...");
  Serial.println("Touch the sensor or bring water near it to see changes!");
  Serial.println("Values should be:");
  Serial.println("  - Air: ~1800-2200 (no water)");
  Serial.println("  - Touch: 2500+ (finger contact)");
  Serial.println("  - Water: Variable based on water level");
  Serial.println("");
}

void testAllComponents() {
  Serial.println("🎯 Testing All Components Together...");
  
  // Initialize everything
  pinMode(PATHWAY_LEDS_PIN, OUTPUT);
  pinMode(SAFE_ZONE_LEDS_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(WATER_SENSOR_PIN, INPUT);
  
  ledcAttach(SERVO1_PIN, 50, 16);
  ledcAttach(SERVO2_PIN, 50, 16);
  ledcAttach(SERVO3_PIN, 50, 16);
  
  Serial.println("✅ All components initialized");
  Serial.println("Starting component cycle test...");
}

void runComponentCycle() {
  static unsigned long lastCycle = 0;
  static int cycleStep = 0;
  
  if (millis() - lastCycle > 3000) { // Change every 3 seconds
    
    switch (cycleStep) {
      case 0:
        Serial.println("🔵 NORMAL STATE");
        turnOffAllLEDs();
        setAllServos(0);
        break;
        
      case 1:
        Serial.println("🟡 LEVEL 1 ALERT");
        digitalWrite(YELLOW_LED_PIN, HIGH);
        digitalWrite(SAFE_ZONE_LEDS_PIN, HIGH);
        setAllServos(90);
        singleBeep();
        break;
        
      case 2:
        Serial.println("🔴 LEVEL 2 CRITICAL");
        digitalWrite(YELLOW_LED_PIN, HIGH);
        digitalWrite(RED_LED_PIN, HIGH);
        digitalWrite(SAFE_ZONE_LEDS_PIN, HIGH);
        setAllServos(180);
        multipleBeeps(3);
        break;
        
      case 3:
        Serial.println("🔥 GLACIER BURST WARNING");
        digitalWrite(YELLOW_LED_PIN, HIGH);
        digitalWrite(RED_LED_PIN, HIGH);
        digitalWrite(SAFE_ZONE_LEDS_PIN, HIGH);
        digitalWrite(PATHWAY_LEDS_PIN, HIGH);
        multipleBeeps(5);
        break;
    }
    
    cycleStep = (cycleStep + 1) % 4;
    lastCycle = millis();
  }
}

// Helper functions
void setServoAngle(int pin, int angle) {
  int dutyCycle = map(angle, 0, 180, 1638, 8192);
  ledcWrite(pin, dutyCycle);
}

void setAllServos(int angle) {
  setServoAngle(SERVO1_PIN, angle);
  setServoAngle(SERVO2_PIN, angle);
  setServoAngle(SERVO3_PIN, angle);
}

void turnOffAllLEDs() {
  digitalWrite(PATHWAY_LEDS_PIN, LOW);
  digitalWrite(SAFE_ZONE_LEDS_PIN, LOW);
  digitalWrite(YELLOW_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
}

void singleBeep() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(200);
  digitalWrite(BUZZER_PIN, LOW);
}

void multipleBeeps(int count) {
  for (int i = 0; i < count; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
    delay(100);
  }
}
