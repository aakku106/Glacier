<!-- @format -->

# 💧 Glacier System - Demonstration Water Setup Guide

**Updated Concept:** Minimal Water, Maximum Impact! 🎯

---

## 🔄 **New Demonstration Logic**

### **Original Problem:**

- Old thresholds: Level 1 = 1500, Level 2 = 2500
- Required too much water to trigger alerts
- Water overflow made demonstrations messy
- Hard to control water levels precisely

### **✅ New Solution:**

- **Level 1 Threshold:** 300 (just a little water)
- **Level 2 Threshold:** 600 (slightly more water)
- **Normal State:** No water in prototype river
- **Visual Enhancement:** Hot glue + colors for realistic water simulation

---

## 💧 **Water Level Testing Guide**

### **Step 1: Baseline Test**

1. **No Water:** System should show `Calibrated: 0`
2. **Raw readings:** ~1800-2200 (this is normal noise)
3. **Status:** "System Normal - Monitoring Sensors"

### **Step 2: Level 1 Test**

1. **Add small amount of water** (just enough to touch sensor)
2. **Expected reading:** ~300+ calibrated value
3. **System response:**
   - 🟡 Yellow LED turns ON
   - 🔧 Servo 1 & 2 open to 90° (half position)
   - 🔊 Single beep sound
   - 📱 Dashboard shows "LEVEL 1 ALERT"

### **Step 3: Level 2 Test**

1. **Add slightly more water** (increase water level)
2. **Expected reading:** ~600+ calibrated value
3. **System response:**
   - 🔴 Red LED turns ON (+ Yellow still on)
   - 🔧 ALL servos open to 180° (full position)
   - 🔊 Multiple rapid beeps (5 beeps)
   - 📱 Dashboard shows "CRITICAL LEVEL 2 ALERT"

---

## 🎨 **Visual Water Simulation Tips**

### **Hot Glue Water Effect:**

1. **Base Layer:** Clear hot glue for water texture
2. **Color Addition:**
   - Blue food coloring for normal water
   - Add brown/mud color for "flood water"
   - White foam for turbulent water effect

### **LED Integration:**

- Place **blue LEDs under clear hot glue** for water glow effect
- **RGB strips** can change color based on alert level:
  - Blue = Normal water
  - Yellow = Level 1 warning
  - Red = Level 2 critical

### **Layered Effect:**

```
Top Layer: Colored hot glue (flood water)
Middle: Clear hot glue (normal water)
Bottom: LED strips for lighting
Base: Prototype river channel
```

---

## 🧪 **Calibration Process**

The system automatically calibrates when it starts:

### **Automatic Calibration:**

1. **Takes 50 readings** over 1 second
2. **Establishes baseline** (~1800-2200 typically)
3. **Filters noise** (ignores changes <100 from baseline)
4. **Uses absolute values** for easier triggering

### **Manual Testing:**

```
Normal (No Water):     Calibrated = 0
Touch Sensor:          Raw jumps to 2500+, Calibrated stays 0
Small Water:           Calibrated = 300+ → Level 1
More Water:            Calibrated = 600+ → Level 2
```

---

## 📊 **Dashboard Monitoring**

### **What to Watch:**

- **Raw Signal:** Shows actual sensor values (includes noise)
- **Calibrated Reading:** Filtered value used for alerts
- **Baseline:** Reference point for noise filtering
- **Alert Status:** Color-coded warnings

### **Expected Values:**

```
No Water:     Raw=1800-2200, Calibrated=0, Status=Normal
Level 1:      Raw=Variable,  Calibrated=300+, Status=Warning
Level 2:      Raw=Variable,  Calibrated=600+, Status=Critical
Touch ESP32:  Raw=2500+,     Calibrated=0, Status=Normal (filtered!)
```

---

## 🎬 **Demonstration Script**

### **1. System Introduction (30 seconds)**

> "This is our Glacier Flood Detection System. In normal conditions, there's no water in our prototype river, representing a dry riverbed."

### **2. Level 1 Demo (45 seconds)**

> "When glacier melting begins, water starts flowing into the river..."
>
> **[Add small amount of water]**
>
> "The system detects rising water levels and activates Level 1 warnings - you can see the yellow LED and gates opening to half position for controlled water release."

### **3. Level 2 Demo (45 seconds)**

> "If glacier melting accelerates, water levels rise critically..."
>
> **[Add more water]**
>
> "Now we have a Level 2 critical alert - all lights activate, all gates open fully for maximum flood prevention, and urgent audio alerts sound."

### **4. RFID Glacier Simulation (30 seconds)**

> "We can also simulate dangerous glacier heat conditions safely using RFID..."
>
> **[Present RFID tag]**
>
> "This triggers our glacier burst warning - all lights glow representing the emergency condition that would precede massive flooding."

### **5. Visual Enhancement Explanation (30 seconds)**

> "The hot glue and colored effects simulate realistic water flow patterns, while our sensors provide precise digital monitoring that would work in real glacier environments."

**Total Demo Time: ~3 minutes**

---

## 🔧 **Troubleshooting New Thresholds**

### **Level 1 Not Triggering:**

- Check if calibrated value reaches 300+
- Ensure water makes contact with sensor
- Verify sensor wiring on GPIO 34

### **Level 2 Not Triggering:**

- Add more water to reach 600+ calibrated
- Check dashboard for real-time readings
- May need to adjust sensor position

### **False Triggers:**

- System should filter out hand proximity
- Check baseline calibration on startup
- Raw values may jump, but calibrated should stay 0

### **No Response:**

- Verify buzzer moved to GPIO 33
- Check all LED connections
- Confirm servo power supply (5V external)

---

## 🎯 **Success Criteria**

✅ **Level 1 triggers with minimal water** (~300 calibrated)  
✅ **Level 2 triggers with slightly more water** (~600 calibrated)  
✅ **No false alerts from touching ESP32**  
✅ **Visual water effects enhance demonstration**  
✅ **All components respond correctly to alerts**  
✅ **Dashboard provides clear real-time feedback**

**Result: Professional demonstration with minimal water and maximum visual impact!** 🚀
