<!-- @format -->
<div align="center">
   
   ![](https://komarev.com/ghpvc/?username=aakku&abbreviated=true&base=0&label=Reppo+VIEWS&style=for-the-badge)
   
</div>

# 🏔️ Glacier Flood Detection & Prevention System

<div align="center">

![Glacier System](https://img.shields.io/badge/🏆%20WINNER-District%20Level%20Robotics%20%26%20IoT%20Competition-gold?style=for-the-badge)

![ESP32](https://img.shields.io/badge/ESP32-Development%20Board-blue?style=flat-square&logo=espressif)
![Arduino](https://img.shields.io/badge/Arduino-IDE-teal?style=flat-square&logo=arduino)
![IoT](https://img.shields.io/badge/IoT-Internet%20of%20Things-green?style=flat-square)
![RFID](https://img.shields.io/badge/RFID-RC522-orange?style=flat-square)
![WiFi](https://img.shields.io/badge/WiFi-Connected-blue?style=flat-square)

**🥇 DISTRICT LEVEL CHAMPIONS**  
**Robotics & IoT Competition 2025**

</div>

---

## 🏆 **Competition Achievement**

> **🥇 FIRST PLACE WINNER**  
> **District Level Robotics & IoT Competition**  
> **Organized by:** [Cornor Technology](https://www.cornortech.com)  
> **Category:** Disaster Prevention & IoT Innovation  
> **Date:** August 2025  
> **Achievement:** Gold Medal for Technical Innovation

Our **Glacier Flood Detection System** secured **1st place** at the district level competition, representing the cutting-edge of IoT disaster prevention technology!

---

## 🎯 **Project Overview**

The **Glacier Flood Detection System** is an intelligent early warning system designed to protect communities from sudden glacier flood events. Using advanced IoT sensors and real-time monitoring, our system provides critical flood warnings and automated flood gate controls.

### 🚨 **The Problem We Solved**

- **Primary Focus:** Sudden catastrophic floods threatening communities
- **Secondary Challenge:** Glacier melting causes rapid water level changes
- Communities near water bodies lack early warning systems
- Traditional flood detection is too slow for emergency response
- Manual flood gate operation is dangerous during disasters
- **Remote Areas:** Landslides block rescue access in hilly regions of Nepal
- **Communication Issues:** WiFi unreliable in mountainous terrain

### 💡 **Our Complete Solution**

A comprehensive disaster management system that:

- ✅ **Detects floods instantly** with calibrated water level sensors
- ✅ **Monitors glacier conditions** using RFID simulation (backup method after hardware issues)
- ✅ **Warns communities immediately** via web dashboard and audio/visual alerts
- ✅ **Controls flood gates automatically** using 3 servo motors
- ✅ **Provides real-time data** through WiFi IoT connectivity
- ✅ **Enables remote rescue operations** with dual-connectivity RC cars
- ✅ **Ensures reliable communication** with WiFi + Bluetooth backup systems

---

## 🛠️ **System Components**

| Component               | Purpose                | Power Source        | GPIO Pin             | Technology                     |
| ----------------------- | ---------------------- | ------------------- | -------------------- | ------------------------------ |
| **ESP32 Controller**    | Main processing unit   | 5V External Supply  | -                    | WiFi + Bluetooth enabled       |
| **Arduino Uno**         | Dedicated power supply | 5V External Supply  | -                    | 5V power distribution hub      |
| **Water Level Sensor**  | Flood detection        | ESP32 (3.3V)        | GPIO 34              | Analog sensor with calibration |
| **RFID Reader (RC522)** | Glacier simulation     | ESP32 (3.3V)        | SPI (21,22,23,19,18) | 13.56MHz MIFARE reader         |
| **3x Servo Motors**     | Automated flood gates  | Arduino Uno (5V)    | GPIO 25,26,27        | 180° precision control         |
| **4x LED Groups**       | Visual warning system  | ESP32 (3.3V)        | GPIO 15,2,4,5        | Pathway, Safe Zone, Alert LEDs |
| **Active Buzzer**       | Audio alerts           | ESP32 (3.3V)        | GPIO 33              | Multi-pattern warning tones    |
| **RC Car (WiFi)**       | ESP8266 Processor      | Independent Battery | WiFi Module          | Long-range rescue vehicle      |
| **RC Car (Bluetooth)**  | Arduino Uno + BT       | Independent Battery | Bluetooth Module     | Reliable short-range backup    |
| **Web Dashboard**       | Real-time monitoring   | HTTP/WiFi           | -                    | Responsive IoT interface       |

---

## 🎮 **Key Features**

### 💧 **Smart Flood Detection (Primary Focus)**

- **Real-Time Monitoring:** Continuous water level sensing with instant alerts
- **Minimal Water Detection:** Level 1 alert at 300 units, Level 2 critical at 600 units
- **Noise Filtering:** Advanced algorithms ignore false readings from interference
- **Emergency Response:** Automated gate control and community warnings

### 🌋 **Glacier Monitoring (Explained to Judges)**

- **Backup Method:** RFID simulation included in technical presentation
- **Competition Adaptation:** Hardware issues led to primary flood focus
- **Educational Value:** Glacier explanation demonstrated system versatility

### ⚡ **Smart Power Management**

- **Dual Power Strategy:** Arduino Uno provides stable 5V, ESP32 handles 3.3V components
- **Load Distribution:** Reduced ESP32 stress by separating power responsibilities
- **System Reliability:** Independent power sources prevent component interference
- **Component Optimization:** Right voltage for each component type

### 🚗 **Remote Rescue Vehicles**

- **WiFi RC Car (ESP8266):** Global control from anywhere in the world
- **Bluetooth RC Car (Arduino Uno):** Reliable short-range backup for hilly regions
- **Separate Processors:** Independent systems ensure backup redundancy
- **Emergency Failover:** If one system fails, the other remains operational
- **Nepal-Specific Design:** Addresses WiFi connectivity issues in mountainous areas
- **Rescue Operations:** Remove landslides, rescue people, reduce life damage
- **Dual Connectivity:** Complete system redundancy for disaster response reliability

### 🔧 **Automated Response**

- **Level 1 Flood:** Gates half-open (90°), yellow LED, single beep
- **Level 2 Critical:** All gates full-open (180°), red+yellow LEDs, rapid beeps
- **Emergency Protocol:** Immediate community alerts and gate automation
- **Remote Rescue:** Deploy RC cars for landslide clearance and people rescue

### 🌐 **IoT Dashboard & Communication**

- **Real-Time:** Live sensor readings every 2 seconds
- **Secure Admin:** Password-protected manual controls (@1234)
- **Remote Access:** Monitor from any phone/computer on WiFi
- **System Logs:** Complete activity history with timestamps
- **Dual Connectivity:** WiFi primary, Bluetooth backup for Nepal's terrain

---

## 📁 **Documentation**

We've created comprehensive documentation for different audiences:

### 📋 **Quick Start:**

- **[docs/DOCUMENTATION_INDEX.md](docs/DOCUMENTATION_INDEX.md)** - Choose the right guide for you

### 👥 **For Everyone (Non-Technical):**

- **[docs/SIMPLE_USER_GUIDE.md](docs/SIMPLE_USER_GUIDE.md)** - Project overview in simple terms

### 👨‍💻 **For Technical People:**

- **[docs/ENGINEERING_DOCUMENTATION.md](docs/ENGINEERING_DOCUMENTATION.md)** - Complete technical specifications
- **[docs/BREADBOARD_WIRING_GUIDE.md](docs/BREADBOARD_WIRING_GUIDE.md)** - Hardware setup and connections
- **[docs/DEMONSTRATION_WATER_GUIDE.md](docs/DEMONSTRATION_WATER_GUIDE.md)** - Testing and demo procedures

### 💻 **Source Code:**

- **[src/main.ino](src/main.ino)** - Main ESP32 Arduino code (1400+ lines)
- **[examples/Test_Connections.ino](examples/Test_Connections.ino)** - Component testing utilities

---

## 🚀 **Quick Setup Guide**

### **1. Hardware Assembly:**

```bash
# Follow the detailed wiring guide
📖 See: docs/BREADBOARD_WIRING_GUIDE.md
```

### **2. Software Installation:**

```bash
# Clone this repository
git clone https://github.com/aakku106/Glacier.git

# Open src/main.ino in Arduino IDE
# Install libraries: WiFi, SPI, MFRC522
# Update WiFi credentials in code
# Upload to ESP32
```

### **3. Testing:**

```bash
# Use component tester first
📖 See: examples/Test_Connections.ino

# Then follow demo procedures
📖 See: docs/DEMONSTRATION_WATER_GUIDE.md
```

### **4. Access Dashboard:**

```bash
# Check Serial Monitor for IP address
# Open browser: http://ESP32_IP_ADDRESS
# Admin Panel: http://ESP32_IP_ADDRESS/admin (Password: @1234)
```

---

## 🎯 **Competition Highlights**

### 🏅 **What Made Us Win**

1. **Real-World Flood Solution:** Primary focus on immediate disaster prevention needs
2. **Smart Power Engineering:** Arduino Uno + ESP32 dual-power architecture
3. **Emergency Redundancy:** Separate processors (ESP8266 + Arduino Uno) for RC vehicle backup
4. **Last-Minute Optimization:** Updated thresholds (Level2: 600→900) and servo positions for hardware compatibility
5. **Professional IoT Implementation:** Complete web dashboard with admin controls
6. **Demonstration Excellence:** Reliable flood detection with optimized water levels
7. **Technical Sophistication:** Advanced sensor calibration and configurable RFID setup
8. **Nepal-Specific Innovation:** Dual-connectivity addressing local terrain challenges
9. **Complete Disaster Management:** Flood detection + dual rescue vehicle integration
10. **Competition Adaptability:** Successfully adjusted system parameters for demo reliability

### 🎪 **Live Demo Capabilities**

- **Primary Demo:** Progressive flood alerts (Level 1 → Level 2) with minimal water
- **Web Dashboard:** Real-time monitoring accessible from judges' devices
- **Admin Controls:** Manual component testing during presentation
- **RC Vehicle Demo:** WiFi and Bluetooth controlled rescue cars
- **Power System:** Demonstrated stable dual-power architecture
- **Glacier Explanation:** Technical presentation covered glacier monitoring concepts

---

## 👥 **Meet Team Glacier**

### **🏔️ The Champions**

| **Team Member**     | **Role**                                   | **Contact & Links**                                                                                                                |
| ------------------- | ------------------------------------------ | ---------------------------------------------------------------------------------------------------------------------------------- |
| **Adarasha Gaiher** | Lead Developer & System Architect          | [@aakku106](https://github.com/aakku106) • [Website](https://adarashagaihre.com.np) • [Email](mailto:adarasha.gaihre106@gmail.com) |
| **[Team Member 2]** | Hardware Engineer & Integration Specialist | [@teammate](https://github.com/teammate)                                                                                           |

**Specializations:**

- **ESP32 Programming** - IoT connectivity, web dashboard, sensor integration
- **Arduino Power Systems** - Dual-controller architecture, load distribution
- **Hardware Design** - Circuit assembly, component testing, power management
- **System Testing** - Calibration procedures, demo optimization, troubleshooting
- **Remote Vehicle Control** - WiFi/Bluetooth RC cars for disaster rescue operations
- **Nepal Terrain Adaptation** - Communication redundancy for mountainous regions

---

## 📊 **Technical Specifications**

```cpp
// Core System Performance
Water Thresholds: Level1=300, Level2=900 (competition-optimized)
Servo Positions: Closed=0°, Half=30°, Full=70° (hardware-calibrated)
RFID Detection: Configurable UID (0xFF,0xFF,0xFF,0xFF default)
Power Architecture: Arduino Uno (5V) + ESP32 (3.3V) dual-system design
RC Communication: ESP8266 (WiFi global) + Arduino Uno (Bluetooth local)
Emergency Redundancy: Separate processors ensure system backup reliability
WiFi Dashboard: 2-second auto-refresh, mobile responsive
Boot Time: ~5 seconds to full operation
Memory Usage: 35KB RAM (efficient ESP32 utilization)
```

### 🔋 **Power Architecture**

- **Arduino Uno:** 5V dedicated supply for servo motors and high-power components
- **ESP32:** 3.3V optimized for sensors, WiFi, and control logic
- **Load Distribution:** Prevents ESP32 overload, ensures system stability
- **Component Separation:** Reduces electrical interference between power domains
- **Total System:** 5V @ 4A external supply with intelligent distribution

---

## 🌟 **Impact & Recognition**

### 🏆 **Competition Success**

- **🥇 District Level Winner** - Robotics & IoT Competition 2025
- **Technical Innovation Award** - Disaster prevention solution
- **Judges' Choice** - Most practical real-world application
- **Audience Favorite** - Best live demonstration

### 🌍 **Real-World Impact**

- **Community Protection:** Early flood warnings save lives in vulnerable regions
- **Cost-Effective:** $100-150 complete solution vs. $50,000+ traditional systems
- **Open Source:** Available for global disaster prevention initiatives
- **Scalable Design:** Adaptable to rivers, lakes, mountain regions worldwide
- **Nepal-Ready:** Designed for challenging terrain and connectivity issues
- **Rescue Integration:** RC vehicles provide immediate disaster response capability
- **Power Resilience:** Dual-controller architecture ensures system reliability

---

## 🔮 **Future Enhancements**

### 🚀 **Next Phase Development**

- **📱 Mobile App** - Direct smartphone notifications
- **🛰️ LoRaWAN** - Long-range communication for remote areas
- **🤖 AI Prediction** - Machine learning flood forecasting
- **☁️ Cloud Analytics** - Regional disaster management network
- **📊 Historical Data** - Trend analysis and climate research

### 🌐 **Global Deployment Vision**

- **Nepal/India:** Himalayan glacier communities
- **Pakistan:** Karakoram mountain villages
- **Canada/Alaska:** Arctic glacier monitoring
- **Switzerland:** Alpine flood prevention
- **Chile/Argentina:** Andes mountain protection

---

## 🤝 **Contributing**

We welcome contributions to improve glacier flood detection worldwide!

### 🔧 **Areas for Contribution:**

- **Hardware Optimization** - Component efficiency and cost reduction
- **Software Enhancement** - Additional sensor support and algorithms
- **Documentation** - Translation to local languages
- **Testing** - Real-world deployment and validation
- **Integration** - Compatibility with existing emergency systems

### 📧 **Contact:**

- **Project Lead:** Adarasha Gaiher - [adarasha.gaihre106@gmail.com](mailto:adarasha.gaihre106@gmail.com)
- **GitHub:** [@aakku106](https://github.com/aakku106)
- **Website:** [adarashagaihre.com.np](https://adarashagaihre.com.np)
- **Technical Questions:** Create GitHub issues
- **Collaboration:** Pull requests welcome

---

## 📜 **License**

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.

**Open Source Commitment:** We believe technology should be freely available for disaster prevention and community safety worldwide! 🌍

---

## 🙏 **Acknowledgments**

- **🏆 Competition Organizers** - [Cornor Technology](https://www.cornortech.com) for providing platform to showcase innovation
- **👨‍🏫 Judges & Mentors** - For recognizing technical excellence and real-world impact
- **🌐 Open Source Community** - ESP32, Arduino, and MFRC522 library developers
- **🏔️ Climate Researchers** - Inspiring our focus on glacier flood prevention
- **👥 Local Communities** - Motivating us to create accessible safety solutions

---

<div align="center">

**🏆 PROUD WINNERS - DISTRICT LEVEL ROBOTICS & IOT COMPETITION 2025 🏆**

![Competition Winner](https://img.shields.io/badge/🥇-DISTRICT%20LEVEL%20CHAMPIONS-gold?style=for-the-badge)

**Representing innovation in IoT disaster prevention technology!**

⭐ **Star this repository to support glacier flood prevention worldwide!** ⭐

[📖 Read Documentation](docs/DOCUMENTATION_INDEX.md) | [🔧 Build Guide](docs/BREADBOARD_WIRING_GUIDE.md) | [👥 Simple Guide](docs/SIMPLE_USER_GUIDE.md)

</div>
````
