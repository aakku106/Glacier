<!-- @format -->

# 🔌 Glacier Flood Detection System - Breadboard Wiring Guide

**Project:** Glacier Flood Detection & Prevention System  
**Version:** 2.1  
**Date:** August 6, 2025  
**ESP32 Code:** Try1.ino

---

## 📋 **Component List & Pin Assignments**

| Component           | Quantity | ESP32 GPIO Pin   | Power Requirements |
| ------------------- | -------- | ---------------- | ------------------ |
| ESP32 DevKit        | 1        | -                | 5V USB or External |
| Water Level Sensor  | 1        | GPIO 34 (Analog) | 3.3V               |
| MFRC522 RFID Reader | 1        | SPI Pins         | 3.3V               |
| Active Buzzer       | 1        | GPIO 18          | 3.3V/5V            |
| Servo Motor 1       | 1        | GPIO 25 (PWM)    | 5V, 1-2A           |
| Servo Motor 2       | 1        | GPIO 26 (PWM)    | 5V, 1-2A           |
| Servo Motor 3       | 1        | GPIO 27 (PWM)    | 5V, 1-2A           |
| Pathway LEDs        | 1+       | GPIO 15          | 3.3V + Resistor    |
| Safe Zone LEDs      | 1+       | GPIO 2           | 3.3V + Resistor    |
| Yellow Alert LED    | 1        | GPIO 4           | 3.3V + Resistor    |
| Red Alert LED       | 1        | GPIO 5           | 3.3V + Resistor    |
| RFID Tag            | 1        | -                | Passive            |

---

## 🔧 **Detailed Wiring Connections**

### **ESP32 Power Connections**

```
ESP32 VIN  → 5V Power Supply (if using external power)
ESP32 3.3V → 3.3V Rail on Breadboard
ESP32 GND  → Ground Rail on Breadboard (connect to power supply ground)
ESP32 EN   → (Leave unconnected for normal operation)
```

---

### **💧 Water Level Sensor (GPIO 34)**

```
Water Sensor Connections:
┌─────────────────┐
│ Water Sensor    │
├─────────────────┤
│ VCC → ESP32 3.3V│
│ GND → ESP32 GND │
│ SIG → GPIO 34   │
└─────────────────┘

Breadboard Wiring:
ESP32 GPIO 34 ──────► Water Sensor Signal Pin
ESP32 3.3V ─────────► Water Sensor VCC
ESP32 GND ──────────► Water Sensor GND
```

**📝 Notes:**

- GPIO 34 is input-only and ADC1_CH6
- Sensor will read ~1800-2200 when no water present
- **Level 1 Alert:** ~300 calibrated value (just a little water)
- **Level 2 Alert:** ~600 calibrated value (slightly more water)
- Touching sensor will show 2500+ (this is filtered out by calibration)
- **Demonstration friendly:** Easy to trigger with minimal water!
- Touching sensor will show 2500+ (this is filtered out by calibration)

---

### **📡 MFRC522 RFID Reader (SPI Interface)**

```
RFID Reader Connections:
┌─────────────────────┐
│ MFRC522 RFID Reader │
├─────────────────────┤
│ VCC → ESP32 3.3V    │
│ GND → ESP32 GND     │
│ RST → GPIO 22       │
│ SDA → GPIO 21       │
│ MOSI → GPIO 23      │
│ MISO → GPIO 19      │
│ SCK → GPIO 18       │
└─────────────────────┘

⚠️ WAIT! GPIO 18 CONFLICT DETECTED!
```

**🚨 CRITICAL WIRING ISSUE FOUND!**

Both the **BUZZER** and **RFID SCK** are assigned to **GPIO 18**! This will cause conflicts.

**🔧 CORRECTED PIN ASSIGNMENTS:**

```
MFRC522 RFID Reader (Corrected):
┌─────────────────────┐
│ MFRC522 RFID Reader │
├─────────────────────┤
│ VCC → ESP32 3.3V    │
│ GND → ESP32 GND     │
│ RST → GPIO 22       │
│ SDA → GPIO 21       │
│ MOSI → GPIO 23      │
│ MISO → GPIO 19      │
│ SCK → GPIO 18       │ ← Keep RFID on GPIO 18
└─────────────────────┘

Active Buzzer (Moved):
┌─────────────────┐
│ Active Buzzer   │
├─────────────────┤
│ Positive → GPIO 33  │ ← MOVED FROM GPIO 18
│ Negative → GND      │
└─────────────────┘
```

---

### **🚨 LED Connections (with Current Limiting Resistors)**

#### **Pathway LEDs (GPIO 15)**

```
ESP32 GPIO 15 ──→ 220Ω Resistor ──→ LED Anode (+)
                                      │
                                   LED Cathode (-) ──→ ESP32 GND
```

#### **Safe Zone LEDs (GPIO 2)**

```
ESP32 GPIO 2 ──→ 220Ω Resistor ──→ LED Anode (+)
                                     │
                                  LED Cathode (-) ──→ ESP32 GND
```

#### **Yellow Alert LED (GPIO 4)**

```
ESP32 GPIO 4 ──→ 220Ω Resistor ──→ Yellow LED Anode (+)
                                     │
                                  LED Cathode (-) ──→ ESP32 GND
```

#### **Red Alert LED (GPIO 5)**

```
ESP32 GPIO 5 ──→ 220Ω Resistor ──→ Red LED Anode (+)
                                     │
                                  LED Cathode (-) ──→ ESP32 GND
```

**💡 LED Notes:**

- Use 220Ω resistors to limit current
- Long leg of LED = Anode (positive)
- Short leg of LED = Cathode (negative)
- Multiple LEDs can be connected in parallel for each pin

---

### **🔧 Servo Motor Connections**

#### **Servo 1 - Small Canal Gate (GPIO 25)**

```
┌──────────────┐
│ Servo Motor 1│
├──────────────┤
│ Signal → GPIO 25  │
│ Power  → 5V       │ ← External 5V supply recommended
│ Ground → GND      │
└──────────────┘
```

#### **Servo 2 - Modular River Gate (GPIO 26)**

```
┌──────────────┐
│ Servo Motor 2│
├──────────────┤
│ Signal → GPIO 26  │
│ Power  → 5V       │
│ Ground → GND      │
└──────────────┘
```

#### **Servo 3 - Underground Gate (GPIO 27)**

```
┌──────────────┐
│ Servo Motor 3│
├──────────────┤
│ Signal → GPIO 27  │
│ Power  → 5V       │
│ Ground → GND      │
└──────────────┘
```

**⚡ Servo Power Requirements:**

- Each servo: 5V, 1-2A peak current
- **DO NOT** power servos from ESP32 3.3V pin
- Use external 5V power supply (2-3A capacity)
- Connect all grounds together (common ground)

---

### **🔊 Active Buzzer (GPIO 33 - CORRECTED)**

```
┌─────────────────┐
│ Active Buzzer   │
├─────────────────┤
│ Positive → GPIO 33  │ ← CHANGED FROM GPIO 18
│ Negative → GND      │
└─────────────────┘

Simple Connection:
ESP32 GPIO 33 ──────► Buzzer Positive (+)
ESP32 GND ───────────► Buzzer Negative (-)
```

---

## 🔧 **CODE CHANGES REQUIRED**

The current code has **GPIO 18** assigned to both RFID and Buzzer. You need to update the buzzer pin:

**In Try1.ino, change line:**

```cpp
#define BUZZER_PIN 18            // Active Buzzer
```

**To:**

```cpp
#define BUZZER_PIN 33            // Active Buzzer (MOVED FROM GPIO 18)
```

---

## 🏗️ **Breadboard Layout Suggestion**

```
     ESP32 DevKit
    ┌─────────────────┐
    │  ┌───────────┐  │
    │  │    USB    │  │
    │  └───────────┘  │
    │                 │
    │ 3V3 ●●●●●●● VIN │ ← Power Rails
    │ GND ●●●●●●● GND │
    │                 │
    │ IO4 ●●●●●●● IO2 │ ← LEDs (Yellow, Safe Zone)
    │ IO5 ●●●●●●● IO15│ ← LEDs (Red, Pathway)
    │ IO18●●●●●●● IO19│ ← RFID (SCK, MISO)
    │ IO21●●●●●●● IO22│ ← RFID (SDA, RST)
    │ IO23●●●●●●● IO25│ ← RFID MOSI, Servo 1
    │ IO26●●●●●●● IO27│ ← Servo 2, Servo 3
    │ IO32●●●●●●● IO33│ ← Available, Buzzer
    │ IO34●●●●●●● IO35│ ← Water Sensor, Available
    └─────────────────┘
```

**Breadboard Zones:**

- **Left Side:** LEDs with resistors
- **Right Side:** RFID module
- **Bottom:** Servo connections (need external power)
- **Top:** Water sensor and buzzer

---

## ⚡ **Power Supply Requirements**

### **Option 1: USB Power Only (Limited)**

```
USB 5V (500mA) → ESP32 VIN
│
├─→ ESP32 3.3V → RFID, Sensors, LEDs
└─→ ESP32 5V → Light servos (SG90 only, may be unreliable)
```

### **Option 2: External Power (Recommended)**

```
External 5V/3A Power Supply
│
├─→ ESP32 VIN (5V)
├─→ Servo Power Rail (5V/2A)
└─→ ESP32 GND (Common Ground)
```

---

## 🧪 **Testing Sequence**

### **Step 1: Basic Power Test**

1. Connect only ESP32 to power
2. Upload test code
3. Check serial monitor for boot messages

### **Step 2: LED Test**

1. Add LEDs with resistors
2. Test each LED individually
3. Verify no shorts or overheating

### **Step 3: Sensor Test**

1. Connect water sensor
2. Monitor analog readings
3. Test calibration system

### **Step 4: RFID Test**

1. Connect RFID module
2. Test tag detection
3. Verify glacier tag learning

### **Step 5: Buzzer Test**

1. Connect buzzer to GPIO 33
2. Test different beep patterns
3. Check audio output

### **Step 6: Servo Test (Critical)**

1. **Use external 5V power**
2. Test each servo individually
3. Verify smooth movement 0°-180°

### **Step 7: Full System Test**

1. All components connected
2. Load main Try1.ino code
3. Test complete alert sequences

---

## 🚨 **Common Issues & Troubleshooting**

### **Power Issues**

- **Servos not moving:** Need external 5V power supply
- **ESP32 resetting:** Insufficient current, add external power
- **LEDs dim:** Check resistor values and connections

### **Connection Issues**

- **RFID not detected:** Check SPI wiring, especially GPIO 18 conflict
- **Water sensor unstable:** Verify analog pin GPIO 34
- **Buzzer silent:** Update code to GPIO 33

### **Code Issues**

- **Compilation error:** Update BUZZER_PIN to GPIO 33
- **False water alerts:** Calibration system will filter noise
- **WiFi not connecting:** Check network credentials

---

## 📐 **Wire Management Tips**

1. **Use different colored wires:**

   - Red: 5V power
   - Black: Ground
   - Blue: 3.3V
   - Yellow: Signal wires
   - Green: Digital outputs

2. **Keep wires short** to reduce interference

3. **Bundle power wires** separately from signal wires

4. **Use breadboard power rails** for clean connections

5. **Label connections** for easy troubleshooting

---

## ✅ **Pre-Flight Checklist**

- [ ] ESP32 connected to power (USB or external 5V)
- [ ] All grounds connected to common ground rail
- [ ] Water sensor on GPIO 34 with 3.3V power
- [ ] RFID module on SPI pins (check GPIO 18 for SCK)
- [ ] Buzzer moved to GPIO 33 (update code!)
- [ ] All LEDs have 220Ω current limiting resistors
- [ ] Servos connected to GPIO 25, 26, 27 with external 5V power
- [ ] No loose connections or shorts
- [ ] Code updated with correct BUZZER_PIN = 33
- [ ] Serial monitor ready at 115200 baud

---

**🚀 Ready to test! Connect everything step by step and verify each component before moving to the next!**
