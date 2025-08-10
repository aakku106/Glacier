<!-- @format -->

# Glacier Flood Detection System - Hardware Guide

## 🔧 Required Components

### Main Components

- **ESP32 DevKit** - Main microcontroller with WiFi
- **RFID-RC522 Module** - For glacier heat simulation
- **Servo Motor (SG90)** - For automated gate control
- **Water Level Sensor** - For flood detection
- **Buzzer** - Audio alerts
- **Jumper Wires** - For connections
- **Breadboard** - For prototyping

### Optional Components

- **LED Indicators** - Visual status indicators
- **Resistors** - For circuit protection
- **Capacitors** - For power filtering

## 📋 Pin Configuration

### ESP32 to RFID-RC522

```
ESP32    →    RFID-RC522
3.3V     →    3.3V
GND      →    GND
GPIO21   →    SDA
GPIO18   →    SCK
GPIO23   →    MOSI
GPIO19   →    MISO
GPIO22   →    RST
```

### ESP32 to Other Components

```
ESP32    →    Component
GPIO13   →    Servo Signal
GPIO12   →    Water Sensor Signal
GPIO14   →    Buzzer Signal
5V       →    Servo Power (Red)
GND      →    Servo Ground (Brown)
```

## 🔌 Wiring Instructions

1. **Power Connections**: Connect 3.3V and GND rails on breadboard
2. **RFID Module**: Follow pin configuration above
3. **Servo Motor**: Connect to GPIO13 with proper power supply
4. **Water Sensor**: Connect signal to GPIO12
5. **Buzzer**: Connect to GPIO14 with current limiting resistor

## ⚡ Power Requirements

- **ESP32**: 3.3V (500mA recommended)
- **Servo Motor**: 5V (1A peak current)
- **RFID Module**: 3.3V (50mA)
- **Water Sensor**: 3.3V (10mA)

## 🛠️ Assembly Tips

1. Use breadboard for prototyping
2. Secure all connections
3. Test each component individually
4. Use multimeter to verify connections
5. Keep wires organized and labeled

## 📐 PCB Design Files

_PCB design files will be added in future updates_

## 🔍 Troubleshooting

### Common Issues

- **RFID not reading**: Check SPI connections and power
- **Servo not moving**: Verify power supply and signal wire
- **Water sensor not responding**: Check analog pin connection
- **WiFi connection issues**: Verify network credentials

### Debug Steps

1. Check all power connections
2. Verify pin assignments match code
3. Test components individually
4. Use serial monitor for debugging
5. Check for loose connections
