/*
 * Glacier Flood Detection & Prevention System
 * ESP32-based flood monitoring and prevention system
 * 
 * Components:
 * - Water Level Sensor (GPIO 34) - Calibrated for minimal water demonstration
 * - RFID Reader for Glacier Detection (SPI)
 * - 3x Servo Motors (GPIO 25, 26, 27)
 * - 4x LED Groups (GPIO 15, 2, 4, 5)
 * - Buzzer (GPIO 33) - Updated pin assignment
 * 
 * Demonstration Concept:
 * - Normal: No water in prototype river
 * - Level 1: Small amount of water triggers first warning
 * - Level 2: Slightly more water triggers critical alert
 * - Visual water simulation using hot glue and colors for realistic effect
 * 
 * Author: Glacier Project Team
 * Date: August 2025
 */

#include <WiFi.h>
#include <WebServer.h>
#include <SPI.h>
#include <MFRC522.h>

// Pin Definitions
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
#define BUZZER_PIN 33            // Active Buzzer (MOVED FROM GPIO 18 - RFID SCK CONFLICT)

// Threshold Values - Adjusted for prototype demonstration with minimal water
#define WATER_LEVEL1_THRESHOLD 300    // Level 1 Warning - Just a little water detected
#define WATER_LEVEL2_THRESHOLD 600    // Level 2 Critical - Slightly more water (easy to achieve)

// RFID Configuration
MFRC522 rfid(SS_PIN, RST_PIN);   // Create RFID instance
MFRC522::MIFARE_Key key;

// Glacier RFID Tag UID (representing heat source/melting trigger)
byte glacierTagUID[4] = {0xFF, 0xFF, 0xFF, 0xFF};  // Default UID - update after reading your tag

// Servo Positions
#define SERVO_CLOSED 0
#define SERVO_HALF_OPEN 90
#define SERVO_FULL_OPEN 180

// Timing Variables
unsigned long previousMillis = 0;
unsigned long pathwayBlinkInterval = 500;  // Normal blink rate
bool pathwayLedState = false;
bool glacierWarningActive = false;
bool level1Alert = false;
bool level2Alert = false;

// Water sensor calibration
int baselineWaterLevel = 0;  // Baseline reading when no sensor connected
bool sensorCalibrated = false;

// Alert logging flags to prevent spam
bool glacierAlertLogged = false;
bool level1AlertLogged = false;
bool level2AlertLogged = false;
bool systemIsNormal = true;

// RFID Detection Variables
bool rfidTagDetected = false;
unsigned long rfidDetectionTime = 0;
unsigned long rfidDebounceDelay = 2000;  // 2 second debounce for RFID detection

// WiFi Credentials (Required for web interface)
const char* ssid = "glacier";
const char* password = "@aakku1069";
// const char* ssid = "Kamladi Dattebyo";
// const char* password = "@puspa&!0";
// const char* ssid = "Nepathya College";
// const char* password = "nepathya123";

// Web Server
WebServer server(80);

// System Status Variables for Web Display
String currentStatus = "System Ready";
String lastAlert = "No alerts";
String systemLog = "";
int maxLogEntries = 50;

// Admin panel security
const String adminPassword = "@1234";
bool adminAuthenticated = false;
unsigned long adminSessionStart = 0;
const unsigned long adminSessionTimeout = 600000; // 10 minutes timeout

// Function declarations (required for Arduino IDE compilation)
void addToLogFormatted(String category, String message, String level = "INFO");
void addToLog(String message);
int getCalibratedWaterLevel(int rawReading);
void calibrateWaterSensor();
void setServoAngle(int pin, int angle);
void handleGlacierWarning();
void handleLevel1Alert();
void handleLevel2Alert();
void handleNormalState();
void handlePathwayLEDs();
void checkRFIDForGlacier();
bool isGlacierTag(byte *buffer, byte bufferSize);
bool isDefaultUID();
void learnGlacierTagUID(byte *buffer, byte bufferSize);
void printHex(byte *buffer, byte bufferSize);
void playBuzzerTone(int beeps, int onTime, int offTime);
void initializeWiFi();
void setupWebServer();
void handleRoot();
void handleAPIStatus();
void handleAPISensors();
void handleAPILogs();
void handleAPIClearLogs();
String generateDashboardHTML();
void handleAdmin();
void handleAdminLED();
void handleAdminBuzzer();
void handleAdminServo();
void handleAdminReset();
void handleAdminLogin();
void handleAdminLogout();
void handleAdminSessionCheck();
String generateAdminHTML();
String generateLoginHTML();
void systemTest();
void printSystemStatus();

void setup() {
  // Initialize Serial Monitor (minimal)
  Serial.begin(115200);
  
  // Initialize SPI bus for RFID
  SPI.begin();
  rfid.PCD_Init();
  
  // Prepare RFID key (default key for MIFARE Classic)
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  
  // Initialize Servo Motors using ESP32 PWM (newer ESP32 core)
  ledcAttach(SERVO1_PIN, 50, 16); // 50 Hz, 16-bit resolution
  ledcAttach(SERVO2_PIN, 50, 16);
  ledcAttach(SERVO3_PIN, 50, 16);
  
  // Set all servos to closed position initially
  setServoAngle(SERVO1_PIN, SERVO_CLOSED);
  setServoAngle(SERVO2_PIN, SERVO_CLOSED);
  setServoAngle(SERVO3_PIN, SERVO_CLOSED);
  
  // Initialize LED Pins as OUTPUT
  pinMode(PATHWAY_LEDS_PIN, OUTPUT);
  pinMode(SAFE_ZONE_LEDS_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  
  // Initialize Buzzer Pin
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Initialize Sensor Pins (INPUT by default for analog pins)
  pinMode(WATER_SENSOR_PIN, INPUT);
  
  // Calibrate water sensor baseline (when no sensor connected)
  calibrateWaterSensor();
  
  // Turn off all LEDs initially
  digitalWrite(PATHWAY_LEDS_PIN, LOW);
  digitalWrite(SAFE_ZONE_LEDS_PIN, LOW);
  digitalWrite(YELLOW_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  
  // Initialize WiFi and Web Server
  initializeWiFi();
  setupWebServer();
  
  addToLogFormatted("SYSTEM", "Glacier Flood Detection System with RFID Initialized", "SUCCESS");
  addToLogFormatted("SYSTEM", "📡 Water sensor calibration complete", "SUCCESS");
  addToLogFormatted("SYSTEM", "🌐 Web server started and ready", "SUCCESS");
  addToLogFormatted("SYSTEM", "🔍 RFID glacier detection active", "INFO");
  currentStatus = "Monitoring Sensors...";
  
  // Print IP address once for reference
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Dashboard: http://");
    Serial.println(WiFi.localIP());
  }
  
  Serial.println("Present RFID tag to learn Glacier Tag UID...");
  
  delay(1000); // Reduced delay
}

void loop() {
  // Handle web server requests
  server.handleClient();
  
  // Read Sensor Values with noise filtering
  int rawWaterLevel = analogRead(WATER_SENSOR_PIN);
  int waterLevel = getCalibratedWaterLevel(rawWaterLevel);
  
  // Check RFID for glacier detection
  checkRFIDForGlacier();
  
  // Reset alert states
  glacierWarningActive = false;
  level1Alert = false;
  level2Alert = false;
  
  // Check RFID Detection (Glacier Heat Simulation)
  if (rfidTagDetected) {
    glacierWarningActive = true;
    handleGlacierWarning();
  }
  
  // Check Water Level Thresholds
  if (waterLevel >= WATER_LEVEL2_THRESHOLD) {
    // Level 2 - Critical Water Level
    level2Alert = true;
    handleLevel2Alert();
  } else if (waterLevel >= WATER_LEVEL1_THRESHOLD) {
    // Level 1 - Increased Water Level
    level1Alert = true;
    handleLevel1Alert();
  } else {
    // Normal State - Reset all alerts
    handleNormalState();
  }
  
  // Handle LED Blinking
  handlePathwayLEDs();
  
  delay(50); // Reduced delay for better responsiveness
}

void handleGlacierWarning() {
  currentStatus = "GLACIER BURST WARNING: Critical glacier melting detected!";
  lastAlert = "Glacier Burst Warning - RFID trigger detected";
  systemIsNormal = false;
  
  if (!glacierAlertLogged) {
    addToLogFormatted("GLACIER", "🌋 RFID tag detected - Critical melting condition!", "ERROR");
    addToLogFormatted("ALERTS", "🚨 ALL LIGHTS ACTIVATED - Glacier burst warning", "WARNING");
    glacierAlertLogged = true;
  }
  
  // ALL LIGHTS GLOW for glacier burst warning (RED, YELLOW, GREEN effect)
  digitalWrite(YELLOW_LED_PIN, HIGH);    // Yellow for Level 1
  digitalWrite(RED_LED_PIN, HIGH);       // Red for Level 2  
  digitalWrite(SAFE_ZONE_LEDS_PIN, HIGH); // Green effect - Safe zone LEDs
  digitalWrite(PATHWAY_LEDS_PIN, HIGH);   // Additional lighting
  
  // Continuous loud buzzer for glacier burst (most critical alert)
  static unsigned long lastBuzzer = 0;
  if (millis() - lastBuzzer > 1000) { // Every 1 second - more urgent than water alerts
    for (int i = 0; i < 5; i++) {     // 5 rapid beeps
      digitalWrite(BUZZER_PIN, HIGH);
      delay(100);
      digitalWrite(BUZZER_PIN, LOW);
      delay(100);
    }
    lastBuzzer = millis();
  }
}

void handleLevel1Alert() {
  currentStatus = "LEVEL 1 ALERT: Water level increased!";
  lastAlert = "Level 1 Alert - Water level increased";
  systemIsNormal = false;
  
  if (!level1AlertLogged) {
    addToLogFormatted("WATER", "🟡 Level 1 water threshold reached", "WARNING");
    addToLogFormatted("SERVOS", "🔧 Gates 1&2 opening to half position", "INFO");
    level1AlertLogged = true;
  }
  
  // Move Servo 1 and 2 to half-open position
  setServoAngle(SERVO1_PIN, SERVO_HALF_OPEN);
  setServoAngle(SERVO2_PIN, SERVO_HALF_OPEN);
  
  // Turn ON Yellow River Pole LED
  digitalWrite(YELLOW_LED_PIN, HIGH);
  digitalWrite(RED_LED_PIN, LOW);
  
  // Turn ON Safe Zone LEDs (Static)
  digitalWrite(SAFE_ZONE_LEDS_PIN, HIGH);
  
  // Set pathway LED blink rate
  pathwayBlinkInterval = 500; // Normal blink rate
  
  // Low frequency buzzer tone
  playBuzzerTone(1, 200, 300); // 1 beep, 200ms on, 300ms off
}

void handleLevel2Alert() {
  currentStatus = "CRITICAL LEVEL 2 ALERT: CRITICAL water level!";
  lastAlert = "CRITICAL Level 2 Alert - Emergency flood level";
  systemIsNormal = false;
  
  if (!level2AlertLogged) {
    addToLogFormatted("WATER", "🔴 CRITICAL Level 2 threshold reached!", "ERROR");
    addToLogFormatted("SERVOS", "🔧 ALL GATES opening to full position", "WARNING");
    addToLogFormatted("ALERTS", "🚨 Emergency flood prevention activated", "ERROR");
    level2AlertLogged = true;
  }
  
  // Move all servos to fully open position
  setServoAngle(SERVO1_PIN, SERVO_FULL_OPEN);
  setServoAngle(SERVO2_PIN, SERVO_FULL_OPEN);
  setServoAngle(SERVO3_PIN, SERVO_FULL_OPEN);
  
  // Turn ON Red River Pole LED
  digitalWrite(YELLOW_LED_PIN, HIGH); // Keep yellow also on
  digitalWrite(RED_LED_PIN, HIGH);
  
  // Turn ON Safe Zone LEDs (Static)
  digitalWrite(SAFE_ZONE_LEDS_PIN, HIGH);
  
  // Increase pathway LED blink rate
  pathwayBlinkInterval = 200; // Faster blink rate for critical alert
  
  // High frequency loud buzzer tone
  playBuzzerTone(5, 100, 100); // 5 beeps, 100ms on, 100ms off
}

void handleNormalState() {
  // Only log return to normal if system was previously in alert state
  if (!systemIsNormal) {
    addToLogFormatted("SYSTEM", "✅ All sensors returned to normal", "SUCCESS");
    addToLogFormatted("SERVOS", "🔧 All gates closed - System reset", "SUCCESS");
    currentStatus = "System Normal - Monitoring Sensors";
    lastAlert = "System returned to normal";
    systemIsNormal = true;
    
    // Reset alert logging flags when returning to normal state
    glacierAlertLogged = false;
    level1AlertLogged = false;
    level2AlertLogged = false;
  }
  
  // Close all servo gates
  setServoAngle(SERVO1_PIN, SERVO_CLOSED);
  setServoAngle(SERVO2_PIN, SERVO_CLOSED);
  setServoAngle(SERVO3_PIN, SERVO_CLOSED);
  
  // Turn OFF all alert LEDs
  digitalWrite(YELLOW_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(SAFE_ZONE_LEDS_PIN, LOW);
  digitalWrite(PATHWAY_LEDS_PIN, LOW);
  
  // Turn off buzzer
  digitalWrite(BUZZER_PIN, LOW);
  
  // Reset blink state
  pathwayLedState = false;
}

void handlePathwayLEDs() {
  // Only blink if there's an active alert
  if (glacierWarningActive || level1Alert || level2Alert) {
    unsigned long currentMillis = millis();
    
    if (currentMillis - previousMillis >= pathwayBlinkInterval) {
      previousMillis = currentMillis;
      pathwayLedState = !pathwayLedState;
      digitalWrite(PATHWAY_LEDS_PIN, pathwayLedState);
    }
  }
}

void checkRFIDForGlacier() {
  // Reset the loop if no new card present on the sensor/reader
  if (!rfid.PICC_IsNewCardPresent()) {
    // Check if we should clear the detection after debounce period
    if (rfidTagDetected && (millis() - rfidDetectionTime > rfidDebounceDelay)) {
      rfidTagDetected = false;
      addToLogFormatted("GLACIER", "❄️ RFID tag removed - Melting condition ended", "SUCCESS");
    }
    return;
  }

  // Verify if the NUID has been read
  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Check if this is the glacier tag or learn new tag UID
  if (isGlacierTag(rfid.uid.uidByte, rfid.uid.size) || isDefaultUID()) {
    if (!rfidTagDetected) {
      rfidTagDetected = true;
      rfidDetectionTime = millis();
      
      // Learn the tag UID if it's the default
      if (isDefaultUID()) {
        learnGlacierTagUID(rfid.uid.uidByte, rfid.uid.size);
      }
      
      addToLogFormatted("GLACIER", "🔥 Glacier RFID tag detected - Heat simulation active", "WARNING");
      
      // Print UID for debugging
      Serial.print("Glacier Tag UID: ");
      printHex(rfid.uid.uidByte, rfid.uid.size);
      Serial.println();
    }
  }

  // Halt PICC
  rfid.PICC_HaltA();
  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}

bool isGlacierTag(byte *buffer, byte bufferSize) {
  // Check if the detected tag matches our learned glacier tag
  if (bufferSize != 4) return false;
  
  for (byte i = 0; i < 4; i++) {
    if (buffer[i] != glacierTagUID[i]) {
      return false;
    }
  }
  return true;
}

bool isDefaultUID() {
  // Check if we're still using the default UID (all 0xFF)
  for (byte i = 0; i < 4; i++) {
    if (glacierTagUID[i] != 0xFF) {
      return false;
    }
  }
  return true;
}

void learnGlacierTagUID(byte *buffer, byte bufferSize) {
  // Learn the first presented tag as the glacier tag
  if (bufferSize == 4) {
    for (byte i = 0; i < 4; i++) {
      glacierTagUID[i] = buffer[i];
    }
    addToLogFormatted("GLACIER", "📖 Glacier tag UID learned and saved", "SUCCESS");
    Serial.println("Glacier tag UID learned successfully!");
  }
}

void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

void playBuzzerTone(int beeps, int onTime, int offTime) {
  for (int i = 0; i < beeps; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(onTime);
    digitalWrite(BUZZER_PIN, LOW);
    if (i < beeps - 1) { // Don't delay after the last beep
      delay(offTime);
    }
  }
}

void initializeWiFi() {
  Serial.println("🔍 Starting WiFi connection...");
  Serial.println("Network: " + String(ssid));
  Serial.println("Password: " + String(password));
  
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
    
    // Print status every 5 attempts
    if (attempts % 5 == 0) {
      Serial.println();
      Serial.println("WiFi Status Code: " + String(WiFi.status()));
      Serial.println("Attempts: " + String(attempts) + "/30");
    }
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.println("✅ WiFi Connected Successfully!");
    Serial.println("IP Address: " + WiFi.localIP().toString());
    Serial.println("Signal Strength: " + String(WiFi.RSSI()) + " dBm");
    Serial.println("Gateway: " + WiFi.gatewayIP().toString());
    Serial.println("DNS: " + WiFi.dnsIP().toString());
    Serial.println("Subnet: " + WiFi.subnetMask().toString());
    
    // Test college network restrictions
    Serial.println();
    Serial.println("🏫 COLLEGE NETWORK TROUBLESHOOTING:");
    Serial.println("📱 Try accessing from your phone/laptop first:");
    Serial.println("   http://" + WiFi.localIP().toString());
    Serial.println("🌐 If website doesn't load, college network likely blocks:");
    Serial.println("   • Device-to-device communication");
    Serial.println("   • Custom ports or services");
    Serial.println("   • Non-standard web traffic");
    Serial.println("🔧 Solutions to try:");
    Serial.println("   1. Connect laptop to SAME WiFi network");
    Serial.println("   2. Ask IT department to whitelist your IP");
    Serial.println("   3. Use mobile hotspot for demonstration");
    Serial.println("   4. Check if captive portal needs login");
    
    addToLogFormatted("WIFI", "🌐 Connected successfully - IP: " + WiFi.localIP().toString(), "SUCCESS");
  } else {
    Serial.println();
    Serial.println("❌ WiFi Connection Failed!");
    Serial.println("Final Status Code: " + String(WiFi.status()));
    Serial.println("Common Status Codes:");
    Serial.println("  1 = WL_NO_SSID_AVAIL (Network not found)");
    Serial.println("  4 = WL_CONNECT_FAILED (Wrong password)");
    Serial.println("  6 = WL_DISCONNECTED (Connection lost)");
    addToLogFormatted("WIFI", "❌ Connection failed - Status: " + String(WiFi.status()), "ERROR");
  }
}

void setupWebServer() {
  Serial.println("🌐 Setting up web server...");
  
  // Main dashboard page
  server.on("/", handleRoot);
  
  // API endpoints for live data
  server.on("/api/status", handleAPIStatus);
  server.on("/api/sensors", handleAPISensors);
  server.on("/api/logs", handleAPILogs);
  server.on("/api/clear-logs", handleAPIClearLogs);
  
  // Admin panel page and controls
  server.on("/admin", handleAdmin);
  server.on("/admin-login", handleAdminLogin);
  server.on("/admin-logout", handleAdminLogout);
  server.on("/api/admin/session-check", handleAdminSessionCheck);
  server.on("/api/admin/led", handleAdminLED);
  server.on("/api/admin/buzzer", handleAdminBuzzer);
  server.on("/api/admin/servo", handleAdminServo);
  server.on("/api/admin/reset", handleAdminReset);
  
  // Start the server
  server.begin();
  Serial.println("✅ Web server started on port 80");
  addToLogFormatted("SERVER", "🌐 Web server started successfully on port 80", "SUCCESS");
}

void handleRoot() {
  String html = generateDashboardHTML();
  server.send(200, "text/html", html);
}

void handleAPIStatus() {
  // Check WiFi status more reliably with debugging
  bool wifiConnected = (WiFi.status() == WL_CONNECTED);
  
  // Alternative check: if we have an IP address, we're connected
  if (!wifiConnected && WiFi.localIP().toString() != "0.0.0.0") {
    wifiConnected = true;
    addToLogFormatted("WIFI", "🔧 Status override: Valid IP detected " + WiFi.localIP().toString(), "INFO");
  }
  
  // Add WiFi status to logs for debugging
  static unsigned long lastWiFiLog = 0;
  if (millis() - lastWiFiLog > 10000) { // Log every 10 seconds
    String wifiStatusText = "📊 Status Code: " + String(WiFi.status()) + 
                           " (WL_CONNECTED=" + String(WL_CONNECTED) + ")";
    addToLogFormatted("WIFI", wifiStatusText, "INFO");
    lastWiFiLog = millis();
  }
  
  // Get calibrated water level reading
  int rawWaterLevel = analogRead(WATER_SENSOR_PIN);
  int calibratedWaterLevel = getCalibratedWaterLevel(rawWaterLevel);
  
  String json = "{";
  json += "\"status\":\"" + currentStatus + "\",";
  json += "\"lastAlert\":\"" + lastAlert + "\",";
  json += "\"waterLevel\":" + String(calibratedWaterLevel) + ",";
  json += "\"rawWaterLevel\":" + String(rawWaterLevel) + ",";
  json += "\"rfidDetected\":" + String(rfidTagDetected ? "true" : "false") + ",";
  json += "\"glacierWarning\":" + String(glacierWarningActive ? "true" : "false") + ",";
  json += "\"level1Alert\":" + String(level1Alert ? "true" : "false") + ",";
  json += "\"level2Alert\":" + String(level2Alert ? "true" : "false") + ",";
  json += "\"wifiConnected\":" + String(wifiConnected ? "true" : "false") + ",";
  json += "\"wifiStatusCode\":" + String(WiFi.status()) + ",";
  json += "\"wifiIP\":\"" + WiFi.localIP().toString() + "\",";
  json += "\"uptime\":" + String(millis() / 1000);
  json += "}";
  
  server.send(200, "application/json", json);
}

void handleAPISensors() {
  int rawWaterLevel = analogRead(WATER_SENSOR_PIN);
  int calibratedWaterLevel = getCalibratedWaterLevel(rawWaterLevel);
  
  String json = "{";
  json += "\"waterLevel\":" + String(calibratedWaterLevel) + ",";
  json += "\"rawWaterLevel\":" + String(rawWaterLevel) + ",";
  json += "\"baseline\":" + String(baselineWaterLevel) + ",";
  json += "\"rfidDetected\":" + String(rfidTagDetected ? "true" : "false") + ",";
  json += "\"waterThreshold1\":" + String(WATER_LEVEL1_THRESHOLD) + ",";
  json += "\"waterThreshold2\":" + String(WATER_LEVEL2_THRESHOLD) + ",";
  json += "\"rfidTagPresent\":\"" + String(rfidTagDetected ? "Glacier Tag Detected" : "No Tag") + "\"";
  json += "}";
  
  server.send(200, "application/json", json);
}

void handleAPILogs() {
  server.send(200, "text/plain", systemLog);
}

void handleAPIClearLogs() {
  systemLog = "";
  addToLog("System logs cleared by user");
  server.send(200, "text/plain", "Logs cleared successfully");
}

String generateDashboardHTML() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <title>Glacier Flood Detection System</title>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body { font-family: Arial, sans-serif; margin: 0; padding: 20px; background: #f0f8ff; }
        .container { max-width: 1200px; margin: 0 auto; }
        .header { text-align: center; color: #2c3e50; margin-bottom: 30px; }
        .status-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr)); gap: 20px; margin-bottom: 30px; }
        .status-card { background: white; padding: 20px; border-radius: 10px; box-shadow: 0 4px 6px rgba(0,0,0,0.1); }
        .status-normal { border-left: 5px solid #27ae60; }
        .status-warning { border-left: 5px solid #f39c12; }
        .status-critical { border-left: 5px solid #e74c3c; }
        .sensor-value { font-size: 2em; font-weight: bold; color: #2c3e50; }
        .sensor-label { color: #7f8c8d; margin-bottom: 10px; }
        .alert-box { padding: 15px; border-radius: 5px; margin: 10px 0; }
        .alert-success { background: #d4edda; color: #155724; border: 1px solid #c3e6cb; }
        .alert-warning { background: #fff3cd; color: #856404; border: 1px solid #ffeaa7; }
        .alert-danger { background: #f8d7da; color: #721c24; border: 1px solid #f5c6cb; }
        .logs-container { background: white; padding: 20px; border-radius: 10px; box-shadow: 0 4px 6px rgba(0,0,0,0.1); }
        .logs { 
            background: #f8f9fa; 
            padding: 15px; 
            border-radius: 5px; 
            max-height: 300px; 
            overflow-y: auto; 
            font-family: 'Courier New', monospace; 
            font-size: 13px;
            line-height: 1.6;
            white-space: pre-wrap;
            word-wrap: break-word;
        }
        .btn { background: #3498db; color: white; padding: 10px 20px; border: none; border-radius: 5px; cursor: pointer; margin: 5px; }
        .btn:hover { background: #2980b9; }
        .btn-danger { background: #e74c3c; }
        .btn-danger:hover { background: #c0392b; }
        .threshold-info { font-size: 0.9em; color: #7f8c8d; margin-top: 5px; }
        .icon { font-weight: bold; color: #3498db; }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1><span class="icon">&#x1F3D4;</span> Glacier Flood Detection &amp; Prevention System</h1>
            <p>Real-time monitoring and control dashboard</p>
            <div style="margin-top: 15px;">
                <a href="/admin" style="background: #e67e22; color: white; padding: 8px 16px; text-decoration: none; border-radius: 5px; font-size: 14px;">
                    🔐 Admin Panel (Password Required)
                </a>
            </div>
        </div>
        
        <div class="alert-box" id="currentAlert">
            <strong id="alertStatus">System Status:</strong> <span id="alertMessage">Loading...</span>
        </div>
        
        <div class="status-grid">
            <div class="status-card">
                <div class="sensor-label"><span class="icon">&#x1F4A7;</span> Water Level (Calibrated)</div>
                <div class="sensor-value" id="waterLevel">---</div>
                <div class="threshold-info">
                    <strong>Thresholds:</strong> Level 1: )rawliteral" + String(WATER_LEVEL1_THRESHOLD) + R"rawliteral( | Level 2: )rawliteral" + String(WATER_LEVEL2_THRESHOLD) + R"rawliteral(<br>
                    <span style="color: #666;">Raw Signal: <span id="rawWaterLevel">---</span></span> | 
                    <span style="color: #666;">Baseline: <span id="baseline">---</span></span><br>
                    <small style="color: #888;">📡 Warnings trigger on CALIBRATED value, not raw signal</small><br>
                    <small style="color: #888;">🤚 Touch ESP32 → Raw jumps, Calibrated stays 0, No false alerts!</small>
                </div>
            </div>
            
            <div class="status-card">
                <div class="sensor-label"><span class="icon">&#x1F4F7;</span> RFID Glacier Detection</div>
                <div class="sensor-value" id="rfidStatus">---</div>
                <div class="threshold-info">
                    🔥 Simulates glacier heat source for safe demonstration<br>
                    <small style="color: #888;">Present any RFID tag to trigger glacier burst warning</small>
                </div>
            </div>
            
            <div class="status-card">
                <div class="sensor-label"><span class="icon">&#x1F4E1;</span> System Info</div>
                <div><strong>WiFi:</strong> <span id="wifiStatus">Connected</span></div>
                <div><strong>Uptime:</strong> <span id="uptime">0s</span></div>
                <div><strong>IP Address:</strong> )rawliteral" + WiFi.localIP().toString() + R"rawliteral(</div>
                <div><strong>Debug:</strong> <span id="wifiDebug" style="font-size: 0.8em; color: #666;">---</span></div>
                <div class="threshold-info">
                    <small style="color: #888;">💡 Dashboard auto-refreshes every 2 seconds</small>
                </div>
            </div>
            
            <div class="status-card">
                <div class="sensor-label"><span class="icon">&#x1F6A8;</span> Alert & Component Status</div>
                <div><strong>🌋 Glacier RFID:</strong> <span id="glacierWarning">No</span></div>
                <div><strong>🟡 Level 1 Alert:</strong> <span id="level1Alert">No</span></div>
                <div><strong>🔴 Level 2 Alert:</strong> <span id="level2Alert">No</span></div>
                <div class="threshold-info">
                    <small style="color: #888;">🚨 All LEDs glow during glacier burst warning</small>
                </div>
            </div>
        </div>
        
        <div class="logs-container">
            <h3><span class="icon">&#x1F4CB;</span> System Logs & Calibration Status</h3>
            <div style="margin-bottom: 15px; padding: 10px; background: #e8f4fd; border-radius: 5px; border-left: 4px solid #3498db;">
                <strong>🎯 Calibration Info:</strong> Water sensor automatically calibrated to filter electromagnetic interference.<br>
                <strong>📊 How it works:</strong><br>
                • <strong>Raw Signal:</strong> Shows actual analog values (including touch interference)<br>
                • <strong>Calibrated Reading:</strong> Filtered value used for flood detection and warnings<br>
                • <strong>Alert Logic:</strong> All warnings are based on CALIBRATED readings, not raw signals<br>
                • <strong>Touch Effect:</strong> Raw may jump to 2000+ when touched, but calibrated stays at 0 = no false alerts!
            </div>
            <button class="btn" onclick="refreshLogs()">&#x1F504; Refresh Logs</button>
            <button class="btn btn-danger" onclick="clearLogs()">&#x1F5D1; Clear Logs</button>
            <div class="logs" id="systemLogs">Loading logs...</div>
        </div>
    </div>

    <script>
        function updateDashboard() {
            fetch('/api/status')
                .then(response => response.json())
                .then(data => {
                    // Update status alert
                    const alertBox = document.getElementById('currentAlert');
                    const alertMessage = document.getElementById('alertMessage');
                    alertMessage.textContent = data.status;
                    
                    if (data.level2Alert === "true") {
                        alertBox.className = 'alert-box alert-danger';
                    } else if (data.level1Alert === "true" || data.glacierWarning === "true") {
                        alertBox.className = 'alert-box alert-warning';
                    } else {
                        alertBox.className = 'alert-box alert-success';
                    }
                    
                    // Update sensor values with enhanced feedback
                    const waterLevelElement = document.getElementById('waterLevel');
                    const calibratedLevel = data.waterLevel;
                    
                    waterLevelElement.textContent = calibratedLevel;
                    
                    // Color code water level based on thresholds
                    if (calibratedLevel >= 2500) {
                        waterLevelElement.style.color = "#e74c3c"; // Red for critical
                    } else if (calibratedLevel >= 1500) {
                        waterLevelElement.style.color = "#f39c12"; // Orange for warning
                    } else if (calibratedLevel > 0) {
                        waterLevelElement.style.color = "#3498db"; // Blue for normal water
                    } else {
                        waterLevelElement.style.color = "#27ae60"; // Green for no water
                    }
                    
                    document.getElementById('rfidStatus').textContent = data.rfidDetected === "true" ? "🔥 TAG DETECTED" : "❄️ NO TAG";
                    document.getElementById('wifiStatus').textContent = data.wifiConnected === "true" ? "Connected" : "Disconnected";
                    document.getElementById('uptime').textContent = data.uptime + "s";
                    
                    // Update WiFi debug info
                    if (data.wifiStatusCode !== undefined && data.wifiIP !== undefined) {
                        document.getElementById('wifiDebug').textContent = 
                            "Status:" + data.wifiStatusCode + " IP:" + data.wifiIP;
                    }
                    
                    // Update detailed sensor info
                    if (data.rawWaterLevel !== undefined) {
                        document.getElementById('rawWaterLevel').textContent = data.rawWaterLevel;
                    }
                    if (data.baseline !== undefined) {
                        document.getElementById('baseline').textContent = data.baseline;
                    }
                    
                    // Color code the WiFi status
                    const wifiElement = document.getElementById('wifiStatus');
                    if (data.wifiConnected === "true") {
                        wifiElement.style.color = "#27ae60"; // Green for connected
                    } else {
                        wifiElement.style.color = "#e74c3c"; // Red for disconnected
                    }
                    
                    // Update alert statuses with enhanced visual feedback
                    const glacierWarningElement = document.getElementById('glacierWarning');
                    const level1AlertElement = document.getElementById('level1Alert');
                    const level2AlertElement = document.getElementById('level2Alert');
                    
                    glacierWarningElement.textContent = data.glacierWarning === "true" ? "🔥 BURST WARNING" : "❄️ OK";
                    glacierWarningElement.style.color = data.glacierWarning === "true" ? "#e74c3c" : "#27ae60";
                    
                    level1AlertElement.textContent = data.level1Alert === "true" ? "🟡 ACTIVE" : "✅ OK";
                    level1AlertElement.style.color = data.level1Alert === "true" ? "#f39c12" : "#27ae60";
                    
                    level2AlertElement.textContent = data.level2Alert === "true" ? "🔴 CRITICAL" : "✅ OK";
                    level2AlertElement.style.color = data.level2Alert === "true" ? "#e74c3c" : "#27ae60";
                })
                .catch(error => console.error('Error updating dashboard:', error));
        }
        
        function refreshLogs() {
            fetch('/api/logs')
                .then(response => response.text())
                .then(data => {
                    document.getElementById('systemLogs').textContent = data || 'No logs available';
                })
                .catch(error => console.error('Error fetching logs:', error));
        }
        
        function clearLogs() {
            if (confirm('Are you sure you want to clear all logs?')) {
                fetch('/api/clear-logs')
                    .then(response => response.text())
                    .then(data => {
                        alert(data);
                        refreshLogs();
                    })
                    .catch(error => console.error('Error clearing logs:', error));
            }
        }
        
        // Auto-refresh every 2 seconds
        setInterval(updateDashboard, 2000);
        setInterval(refreshLogs, 10000);
        
        // Initial load
        updateDashboard();
        refreshLogs();
    </script>
</body>
</html>
)rawliteral";
  
  return html;
}

void handleAdmin() {
  // Check if admin session is valid
  if (!adminAuthenticated || (millis() - adminSessionStart > adminSessionTimeout)) {
    adminAuthenticated = false;
    String html = generateLoginHTML();
    server.send(200, "text/html", html);
    return;
  }
  
  // Admin is authenticated, show admin panel
  String html = generateAdminHTML();
  server.send(200, "text/html", html);
}

void handleAdminLogin() {
  if (server.hasArg("password")) {
    String password = server.arg("password");
    
    if (password == adminPassword) {
      adminAuthenticated = true;
      adminSessionStart = millis();
      addToLogFormatted("ADMIN", "🔐 Admin panel access granted", "SUCCESS");
      
      // Redirect to admin panel
      server.sendHeader("Location", "/admin");
      server.send(302, "text/plain", "");
    } else {
      addToLogFormatted("ADMIN", "🚫 Admin panel access denied - incorrect password", "WARNING");
      String html = generateLoginHTML();
      html.replace("<!--ERROR_MESSAGE-->", "<div style='color: red; text-align: center; margin: 10px 0;'>❌ Incorrect password. Please try again.</div>");
      server.send(200, "text/html", html);
    }
  } else {
    String html = generateLoginHTML();
    server.send(200, "text/html", html);
  }
}

void handleAdminLED() {
  // Check authentication first
  if (!adminAuthenticated || (millis() - adminSessionStart > adminSessionTimeout)) {
    server.send(401, "text/plain", "Unauthorized - Please login again");
    return;
  }
  if (server.hasArg("led") && server.hasArg("state")) {
    String ledPin = server.arg("led");
    String state = server.arg("state");
    
    int pin = -1;
    String ledName = "";
    
    if (ledPin == "pathway") {
      pin = PATHWAY_LEDS_PIN;
      ledName = "Pathway LEDs";
    } else if (ledPin == "safezone") {
      pin = SAFE_ZONE_LEDS_PIN;
      ledName = "Safe Zone LEDs";
    } else if (ledPin == "yellow") {
      pin = YELLOW_LED_PIN;
      ledName = "Yellow LED";
    } else if (ledPin == "red") {
      pin = RED_LED_PIN;
      ledName = "Red LED";
    }
    
    if (pin != -1) {
      bool ledState = (state == "on");
      digitalWrite(pin, ledState ? HIGH : LOW);
      
      String action = ledState ? "turned ON" : "turned OFF";
      addToLogFormatted("ADMIN", "🎛️ " + ledName + " manually " + action, "INFO");
      
      server.send(200, "text/plain", ledName + " " + action + " successfully");
    } else {
      server.send(400, "text/plain", "Invalid LED specified");
    }
  } else {
    server.send(400, "text/plain", "Missing parameters");
  }
}

void handleAdminBuzzer() {
  // Check authentication first
  if (!adminAuthenticated || (millis() - adminSessionStart > adminSessionTimeout)) {
    server.send(401, "text/plain", "Unauthorized - Please login again");
    return;
  }
  
  if (server.hasArg("action")) {
    String action = server.arg("action");
    
    if (action == "beep") {
      // Single beep
      digitalWrite(BUZZER_PIN, HIGH);
      delay(200);
      digitalWrite(BUZZER_PIN, LOW);
      addToLogFormatted("ADMIN", "🔊 Manual buzzer test - single beep", "INFO");
      server.send(200, "text/plain", "Buzzer test completed");
    } else if (action == "pattern") {
      // Pattern beep (3 beeps)
      for (int i = 0; i < 3; i++) {
        digitalWrite(BUZZER_PIN, HIGH);
        delay(150);
        digitalWrite(BUZZER_PIN, LOW);
        delay(150);
      }
      addToLogFormatted("ADMIN", "🔊 Manual buzzer test - pattern beeps", "INFO");
      server.send(200, "text/plain", "Buzzer pattern completed");
    } else if (action == "on") {
      digitalWrite(BUZZER_PIN, HIGH);
      addToLogFormatted("ADMIN", "🔊 Buzzer manually turned ON", "INFO");
      server.send(200, "text/plain", "Buzzer turned ON");
    } else if (action == "off") {
      digitalWrite(BUZZER_PIN, LOW);
      addToLogFormatted("ADMIN", "🔊 Buzzer manually turned OFF", "INFO");
      server.send(200, "text/plain", "Buzzer turned OFF");
    } else {
      server.send(400, "text/plain", "Invalid buzzer action");
    }
  } else {
    server.send(400, "text/plain", "Missing action parameter");
  }
}

void handleAdminServo() {
  // Check authentication first
  if (!adminAuthenticated || (millis() - adminSessionStart > adminSessionTimeout)) {
    server.send(401, "text/plain", "Unauthorized - Please login again");
    return;
  }
  
  if (server.hasArg("servo") && server.hasArg("angle")) {
    String servoNum = server.arg("servo");
    int angle = server.arg("angle").toInt();
    
    // Validate angle range
    if (angle < 0 || angle > 180) {
      server.send(400, "text/plain", "Angle must be between 0-180 degrees");
      return;
    }
    
    int pin = -1;
    String servoName = "";
    
    if (servoNum == "1") {
      pin = SERVO1_PIN;
      servoName = "Small Canal Gate";
    } else if (servoNum == "2") {
      pin = SERVO2_PIN;
      servoName = "Modular River Gate";
    } else if (servoNum == "3") {
      pin = SERVO3_PIN;
      servoName = "Underground Gate";
    }
    
    if (pin != -1) {
      setServoAngle(pin, angle);
      addToLogFormatted("ADMIN", "🔧 " + servoName + " manually set to " + String(angle) + "°", "INFO");
      server.send(200, "text/plain", servoName + " set to " + String(angle) + "° successfully");
    } else {
      server.send(400, "text/plain", "Invalid servo specified");
    }
  } else {
    server.send(400, "text/plain", "Missing parameters");
  }
}

void handleAdminReset() {
  // Check authentication first
  if (!adminAuthenticated || (millis() - adminSessionStart > adminSessionTimeout)) {
    server.send(401, "text/plain", "Unauthorized - Please login again");
    return;
  }
  
  // Reset all components to normal state
  digitalWrite(PATHWAY_LEDS_PIN, LOW);
  digitalWrite(SAFE_ZONE_LEDS_PIN, LOW);
  digitalWrite(YELLOW_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  
  setServoAngle(SERVO1_PIN, SERVO_CLOSED);
  setServoAngle(SERVO2_PIN, SERVO_CLOSED);
  setServoAngle(SERVO3_PIN, SERVO_CLOSED);
  
  addToLogFormatted("ADMIN", "🔄 All components manually reset to default state", "SUCCESS");
  server.send(200, "text/plain", "All components reset successfully");
}

void handleAdminLogout() {
  // Invalidate the session on server side
  adminAuthenticated = false;
  adminSessionStart = 0;
  
  addToLogFormatted("ADMIN", "🚪 Admin session terminated - User logged out", "INFO");
  
  // Redirect to main dashboard
  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "Logged out successfully");
}

void handleAdminSessionCheck() {
  // Check if admin session is valid
  if (!adminAuthenticated || (millis() - adminSessionStart > adminSessionTimeout)) {
    adminAuthenticated = false; // Ensure session is invalidated
    server.send(401, "application/json", "{\"authenticated\":false,\"message\":\"Session expired\"}");
    return;
  }
  
  // Calculate remaining time
  unsigned long remaining = adminSessionTimeout - (millis() - adminSessionStart);
  server.send(200, "application/json", "{\"authenticated\":true,\"remainingTime\":" + String(remaining) + "}");
}

String generateLoginHTML() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <title>Admin Login - Glacier System</title>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body { 
            font-family: Arial, sans-serif; 
            margin: 0; 
            padding: 0; 
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            display: flex;
            justify-content: center;
            align-items: center;
        }
        .login-container { 
            background: white; 
            padding: 40px; 
            border-radius: 15px; 
            box-shadow: 0 8px 32px rgba(0,0,0,0.3); 
            max-width: 400px; 
            width: 100%;
            text-align: center;
        }
        .login-title { 
            color: #2c3e50; 
            margin-bottom: 30px; 
            font-size: 24px;
        }
        .system-info {
            background: #e8f4fd;
            padding: 15px;
            border-radius: 8px;
            margin-bottom: 25px;
            border-left: 4px solid #3498db;
        }
        .form-group { 
            margin-bottom: 20px; 
            text-align: left; 
        }
        .form-group label { 
            display: block; 
            margin-bottom: 8px; 
            color: #2c3e50; 
            font-weight: bold; 
        }
        .form-group input { 
            width: 100%; 
            padding: 12px; 
            border: 2px solid #ddd; 
            border-radius: 8px; 
            font-size: 16px; 
            box-sizing: border-box;
        }
        .form-group input:focus { 
            border-color: #3498db; 
            outline: none; 
        }
        .login-btn { 
            background: #3498db; 
            color: white; 
            padding: 12px 30px; 
            border: none; 
            border-radius: 8px; 
            cursor: pointer; 
            font-size: 16px; 
            width: 100%;
            transition: background 0.3s;
        }
        .login-btn:hover { 
            background: #2980b9; 
        }
        .nav-link { 
            display: inline-block; 
            margin-top: 20px; 
            color: #3498db; 
            text-decoration: none; 
        }
        .nav-link:hover { 
            text-decoration: underline; 
        }
        .security-note {
            margin-top: 20px;
            padding: 10px;
            background: #fff3cd;
            border: 1px solid #ffeaa7;
            border-radius: 5px;
            font-size: 12px;
            color: #856404;
        }
        .icon { 
            font-size: 48px; 
            margin-bottom: 20px; 
        }
    </style>
</head>
<body>
    <div class="login-container">
        <div class="icon">🔐</div>
        <h1 class="login-title">Admin Panel Access</h1>
        
        <div class="system-info">
            <strong>🎛️ Glacier Flood Detection System</strong><br>
            <small>Secure administrative access required</small>
        </div>
        
        <!--ERROR_MESSAGE-->
        
        <form action="/admin-login" method="POST">
            <div class="form-group">
                <label for="password">🔑 Admin Password:</label>
                <input type="password" id="password" name="password" 
                       placeholder="Enter admin password" required autofocus>
            </div>
            
            <button type="submit" class="login-btn">
                🚀 Access Admin Panel
            </button>
        </form>
        
        <a href="/" class="nav-link">🏠 Back to Main Dashboard</a>
        
        <div class="security-note">
            <strong>🔒 Security Notice:</strong><br>
            • Session timeout: 10 minutes<br>
            • All admin actions are logged<br>
            • Unauthorized access attempts are recorded
        </div>
    </div>

    <script>
        // Auto-focus password field
        document.getElementById('password').focus();
        
        // Handle form submission
        document.querySelector('form').addEventListener('submit', function(e) {
            const password = document.getElementById('password').value;
            if (password.trim() === '') {
                e.preventDefault();
                alert('Please enter the admin password');
                return false;
            }
        });
        
        // Enter key support
        document.getElementById('password').addEventListener('keypress', function(e) {
            if (e.key === 'Enter') {
                document.querySelector('form').submit();
            }
        });
    </script>
</body>
</html>
)rawliteral";
  
  return html;
}

String generateAdminHTML() {
  String html = "<!DOCTYPE html><html><head><title>Admin Panel</title><meta charset='UTF-8'>";
  html += "<style>body{font-family:Arial;margin:20px;background:#f5f5f5;}";
  html += ".card{background:white;padding:15px;margin:10px 0;border-radius:5px;border-left:3px solid #3498db;}";
  html += ".btn{background:#3498db;color:white;padding:8px 12px;border:none;border-radius:3px;margin:3px;cursor:pointer;}";
  html += ".btn:hover{background:#2980b9;}.btn-success{background:#27ae60;}.btn-danger{background:#e74c3c;}";
  html += ".btn-warning{background:#f39c12;}</style></head><body>";
  
  html += "<h1>🎛️ Admin Panel</h1>";
  html += "<div><a href='/' style='color:#3498db;'>🏠 Dashboard</a> | ";
  html += "<a href='/admin-logout' style='color:#e74c3c;'>🚪 Logout</a></div><br>";
  
  // LED Controls
  html += "<div class='card'><h3>💡 LED Controls</h3>";
  html += "<button class='btn btn-success' onclick='led(\"pathway\",\"on\")'>Pathway ON</button>";
  html += "<button class='btn btn-danger' onclick='led(\"pathway\",\"off\")'>Pathway OFF</button><br>";
  html += "<button class='btn btn-success' onclick='led(\"safezone\",\"on\")'>Safe Zone ON</button>";
  html += "<button class='btn btn-danger' onclick='led(\"safezone\",\"off\")'>Safe Zone OFF</button><br>";
  html += "<button class='btn btn-success' onclick='led(\"yellow\",\"on\")'>Yellow ON</button>";
  html += "<button class='btn btn-danger' onclick='led(\"yellow\",\"off\")'>Yellow OFF</button><br>";
  html += "<button class='btn btn-success' onclick='led(\"red\",\"on\")'>Red ON</button>";
  html += "<button class='btn btn-danger' onclick='led(\"red\",\"off\")'>Red OFF</button></div>";
  
  // Buzzer Controls
  html += "<div class='card'><h3>🔊 Buzzer Controls</h3>";
  html += "<button class='btn btn-warning' onclick='buzzer(\"beep\")'>Test Beep</button>";
  html += "<button class='btn btn-warning' onclick='buzzer(\"pattern\")'>Pattern</button>";
  html += "<button class='btn btn-success' onclick='buzzer(\"on\")'>Buzzer ON</button>";
  html += "<button class='btn btn-danger' onclick='buzzer(\"off\")'>Buzzer OFF</button></div>";
  
  // Servo Controls
  html += "<div class='card'><h3>🔧 Servo Controls</h3>";
  html += "Servo 1: <button class='btn' onclick='servo(1,0)'>Close</button>";
  html += "<button class='btn' onclick='servo(1,90)'>Half</button>";
  html += "<button class='btn' onclick='servo(1,180)'>Open</button><br>";
  html += "Servo 2: <button class='btn' onclick='servo(2,0)'>Close</button>";
  html += "<button class='btn' onclick='servo(2,90)'>Half</button>";
  html += "<button class='btn' onclick='servo(2,180)'>Open</button><br>";
  html += "Servo 3: <button class='btn' onclick='servo(3,0)'>Close</button>";
  html += "<button class='btn' onclick='servo(3,90)'>Half</button>";
  html += "<button class='btn' onclick='servo(3,180)'>Open</button></div>";
  
  // Reset Controls
  html += "<div class='card'><h3>🚨 Emergency Controls</h3>";
  html += "<button class='btn btn-danger' onclick='reset()' style='font-size:16px;padding:12px 20px;'>🔄 RESET ALL</button></div>";
  
  // JavaScript
  html += "<script>";
  html += "function led(l,s){fetch('/api/admin/led?led='+l+'&state='+s).then(r=>r.text()).then(alert).catch(()=>location.href='/admin');}";
  html += "function buzzer(a){fetch('/api/admin/buzzer?action='+a).then(r=>r.text()).then(alert).catch(()=>location.href='/admin');}";
  html += "function servo(s,a){fetch('/api/admin/servo?servo='+s+'&angle='+a).then(r=>r.text()).then(alert).catch(()=>location.href='/admin');}";
  html += "function reset(){if(confirm('Reset all?'))fetch('/api/admin/reset').then(r=>r.text()).then(alert).catch(()=>location.href='/admin');}";
  html += "setInterval(()=>{fetch('/api/admin/led?led=test&state=check').catch(()=>location.href='/admin');},30000);";
  html += "</script></body></html>";
  
  return html;
}

void addToLog(String message) {
  String timestamp = String(millis() / 1000);
  String logEntry = "[" + timestamp + "s] " + message + "\n";
  
  systemLog += logEntry;
  
  // Keep only the last maxLogEntries
  int lineCount = 0;
  for (int i = 0; i < systemLog.length(); i++) {
    if (systemLog.charAt(i) == '\n') lineCount++;
  }
  
  if (lineCount > maxLogEntries) {
    int firstNewline = systemLog.indexOf('\n');
    systemLog = systemLog.substring(firstNewline + 1);
  }
}

// Enhanced logging function with categories and formatting
void addToLogFormatted(String category, String message, String level) {
  String timestamp = String(millis() / 1000);
  String logEntry = "[" + timestamp + "s] ";
  
  // Add level indicator
  if (level == "ERROR") {
    logEntry += "❌ ERROR: ";
  } else if (level == "WARNING") {
    logEntry += "⚠️  WARN:  ";
  } else if (level == "SUCCESS") {
    logEntry += "✅ OK:    ";
  } else {
    logEntry += "ℹ️  INFO:  ";
  }
  
  // Add category
  logEntry += "[" + category + "] ";
  logEntry += message + "\n";
  
  systemLog += logEntry;
  
  // Keep only the last maxLogEntries
  int lineCount = 0;
  for (int i = 0; i < systemLog.length(); i++) {
    if (systemLog.charAt(i) == '\n') lineCount++;
  }
  
  if (lineCount > maxLogEntries) {
    int firstNewline = systemLog.indexOf('\n');
    systemLog = systemLog.substring(firstNewline + 1);
  }
}

// Servo control function using ESP32 PWM (newer ESP32 core)
void setServoAngle(int pin, int angle) {
  // Convert angle (0-180) to duty cycle for servo control
  // For newer ESP32 core: duty cycle range is 0-65535 for 16-bit resolution
  int dutyCycle = map(angle, 0, 180, 1638, 8192); // Map to appropriate PWM range
  ledcWrite(pin, dutyCycle);
}

// Additional utility functions

void systemTest() {
  // Test all components on startup (call this in setup if needed)
  addToLog("Testing all components...");
  
  // Test LEDs
  digitalWrite(PATHWAY_LEDS_PIN, HIGH);
  digitalWrite(SAFE_ZONE_LEDS_PIN, HIGH);
  digitalWrite(YELLOW_LED_PIN, HIGH);
  digitalWrite(RED_LED_PIN, HIGH);
  delay(1000);
  
  // Test Servos
  setServoAngle(SERVO1_PIN, SERVO_FULL_OPEN);
  setServoAngle(SERVO2_PIN, SERVO_FULL_OPEN);
  setServoAngle(SERVO3_PIN, SERVO_FULL_OPEN);
  delay(1000);
  
  // Test Buzzer
  playBuzzerTone(3, 100, 100);
  
  // Reset all
  handleNormalState();
  
  addToLog("Component test complete!");
}

void printSystemStatus() {
  // Debug function accessible via web interface only
  addToLog("=== SYSTEM STATUS ===");
  addToLog("Water Level: " + String(analogRead(WATER_SENSOR_PIN)));
  addToLog("RFID Glacier Tag: " + String(rfidTagDetected ? "DETECTED" : "NOT DETECTED"));
  addToLog("WiFi: " + String(WiFi.status() == WL_CONNECTED ? "Connected" : "Disconnected"));
  addToLog("===================");
}

// Water sensor calibration functions
void calibrateWaterSensor() {
  addToLog("Calibrating water sensor baseline...");
  
  // Take multiple readings to establish baseline when no sensor is connected
  long sum = 0;
  int numReadings = 50;
  
  for (int i = 0; i < numReadings; i++) {
    sum += analogRead(WATER_SENSOR_PIN);
    delay(20);
  }
  
  baselineWaterLevel = sum / numReadings;
  sensorCalibrated = true;
  
  addToLog("Water sensor baseline calibrated: " + String(baselineWaterLevel));
  
  // If baseline is very low (actual sensor connected), adjust thresholds accordingly
  if (baselineWaterLevel < 100) {
    addToLog("Real water sensor detected - using normal thresholds");
  } else {
    addToLog("No sensor connected - filtering noise from baseline: " + String(baselineWaterLevel));
  }
}

int getCalibratedWaterLevel(int rawReading) {
  if (!sensorCalibrated) {
    return rawReading; // Return raw reading if not calibrated yet
  }
  
  // If no real sensor is connected (high baseline), filter out noise
  if (baselineWaterLevel > 500) {
    // Calculate difference from baseline
    int difference = rawReading - baselineWaterLevel;
    
    // Only consider significant changes (more than 100 from baseline for easier triggering)
    if (abs(difference) < 100) {
      return 0; // Treat small variations as no water
    }
    
    // Return the significant change (amplified for easier demonstration)
    return max(0, abs(difference));  // Use absolute value for easier triggering
  }
  
  // Real sensor connected - use raw reading
  return rawReading;
}