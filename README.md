# MediBox - Smart Medicine Storage System

## Overview

MediBox is an intelligent medicine storage system built on the ESP32 platform. It provides automated medicine compartment control based on environmental conditions, medicine reminders with alarms, and real-time monitoring through MQTT communication and a Node-RED dashboard.

The system monitors ambient light, temperature, and humidity to automatically adjust the medicine compartment opening angle via a servo motor, ensuring optimal storage conditions for medications.

## Features

### Core Functionality
- 🏥 **Automated Medicine Compartment Control**: Servo-controlled opening based on light intensity
- ⏰ **Medicine Reminder System**: Configurable alarms with snooze functionality
- 🌡️ **Environmental Monitoring**: Temperature and humidity sensing with DHT22
- 💡 **Light Intensity Monitoring**: LDR-based light sensing for optimal storage
- 📱 **Remote Control**: MQTT-based communication for remote monitoring and control
- 🖥️ **OLED Display**: Real-time information display on SSD1306 OLED
- 🔊 **Audio/Visual Alerts**: Buzzer and LED indicators for warnings and alarms

## Hardware Requirements

### Components
- **ESP32 DevKit C V4** - Main microcontroller
- **SSD1306 OLED Display (128x64)** - Information display
- **DHT22 Sensor** - Temperature and humidity monitoring
- **LDR (Light Dependent Resistor)** - Light intensity sensing
- **SG90 Servo Motor** - Medicine compartment control
- **Buzzer** - Audio alerts
- **LED** - Visual indicators
- **Push Buttons (4x)** - User interface
  - MENU (Blue) - Pin 33
  - UP (Yellow) - Pin 33  
  - DOWN (Green) - Pin 35
  - CANCEL (Red) - Pin 34
  - OK (Center) - Pin 32
- **Resistors** - Pull-up/pull-down for buttons and LED current limiting
- **Breadboards** - Component mounting and connections

### Pin Configuration
```cpp
// Sensor Pins
#define DHTPIN 12        // DHT22 Temperature/Humidity
#define LDR_PIN 36       // Light Dependent Resistor
#define SERVO_PIN 25     // Servo Motor Control

// Interface Pins
#define BUZZER 5         // Buzzer for alarms
#define LED1 15          // Status LED

// Button Pins
#define PB_UP 33         // Up button
#define PB_DOWN 35       // Down button  
#define PB_OK 32         // OK/Select button
#define PB_CANCEL 34     // Cancel button

// Display Pins (I2C)
// SDA - Pin 21
// SCL - Pin 22
```

### Hardware Architecture
```
ESP32 DevKit C V4
├── I2C Bus (SDA: 21, SCL: 22)
│   └── SSD1306 OLED Display
├── Digital Pins
│   ├── Pin 12: DHT22 (Temperature/Humidity)
│   ├── Pin 25: Servo Motor
│   ├── Pin 5:  Buzzer
│   ├── Pin 15: Status LED
│   ├── Pin 32: OK Button
│   ├── Pin 33: UP Button  
│   ├── Pin 34: CANCEL Button
│   └── Pin 35: DOWN Button
├── Analog Pin
│   └── Pin 36: LDR (Light Sensor)
└── Power & Communication
    ├── 3.3V/5V: Component Power
    ├── GND: Common Ground
    └── WiFi: Wireless Communication
```

## Installation & Setup

### Prerequisites
- [PlatformIO](https://platformio.org/) or Arduino IDE
- [Node-RED](https://nodered.org/) (for dashboard)
- MQTT Broker access (uses test.mosquitto.org by default)

### Software Setup

1. **Clone the Repository**
   ```bash
   git clone <repository-url>
   cd MediBox
   ```

2. **Install Dependencies**
   
   The required libraries are defined in `platformio.ini`:
   ```ini
   lib_deps = 
       beegee-tokyo/DHT sensor library for ESPx@^1.19.0
       adafruit/Adafruit GFX Library@^1.11.11
       adafruit/Adafruit SSD1306@^2.5.13
       adafruit/DHT sensor library@^1.4.6
       knolleary/PubSubClient@^2.8
       madhephaestus/ESP32Servo@^3.0.6
   ```

3. **Configure WiFi**
   
   Update `src/wifi_control.cpp` with your WiFi credentials:
   ```cpp
   const char *default_ssid = "Your_WiFi_SSID";
   const char *default_password = "Your_WiFi_Password";
   ```

4. **Build and Upload**
   ```bash
   pio run --target upload
   ```

### Hardware Setup

1. **Assemble the Circuit** according to the wiring diagram in `diagram.json`
2. **Mount Components** on breadboards as shown in the circuit design
3. **Connect ESP32** to development machine via USB
4. **Power On** and verify all connections

### Node-RED Dashboard Setup

1. **Import Flow** from `NODE_RED.json`
2. **Install Required Nodes**:
   - node-red-dashboard
   - node-red-contrib-mqtt-broker
3. **Configure MQTT Broker** connection to match ESP32 settings
4. **Deploy Flow** and access dashboard at `http://localhost:1880/ui`

## Usage


### Menu Options

- **Set Alarm**: Configure medicine reminder times
- **View Alarms**: Display current alarm settings  
- **Delete Alarm**: Remove existing alarms
- **Settings**: Configure system parameters

### Alarm System

1. **Setting Alarms**
   - Navigate to "Set Alarm" in menu
   - Select alarm slot (1 or 2)
   - Set hours and minutes using UP/DOWN buttons
   - Confirm with OK button

2. **Alarm Triggers**
   - Displays "MEDICINE TIME" message
   - Plays melodic alarm sequence
   - LED indicator activates
   - **CANCEL**: Stop alarm permanently
   - **OK**: Snooze for 1 minute

### Environmental Monitoring

- **Temperature Range**: Optimal 24-32°C
- **Humidity Range**: Optimal 65-85%
- **Light Control**: Automatic servo adjustment based on ambient light
- **Warnings**: Visual and audio alerts for out-of-range conditions

## MQTT Topics

### Published Topics (ESP32 → Broker)
- `ENTC-LIGHT`: Light intensity values (0.0-1.0 normalized)

### Subscribed Topics (Broker → ESP32)
- `ENTC-INTERVAL`: Sampling/data intervals
  - Format: `s<value>` for sampling interval (seconds)
  - Format: `d<value>` for data interval (seconds)
- `ENTC-ANGLE`: Minimum servo angle (0-120 degrees)
- `ENTC-CONTROL`: Controlling factor for servo response (0.0-1.0)
- `ENTC-TEMP1`: Target medicine temperature (10-40°C)

### Message Formats

```json
// Light intensity (published)
"0.75"  // Normalized value (0.0 = dark, 1.0 = bright)

// Sampling interval (subscribed)
"s5"    // Set sampling interval to 5 seconds

// Data interval (subscribed)  
"d30"   // Set data interval to 30 seconds

// Servo angle (subscribed)
"45"    // Set minimum servo angle to 45 degrees

// Controlling factor (subscribed)
"0.8"   // Set controlling factor to 0.8

// Temperature setting (subscribed)
"25"    // Set target temperature to 25°C
```

### Accessing Dashboard

1. **Local Access**: `http://localhost:1880/ui`
2. **Network Access**: `http://<node-red-server-ip>:1880/ui`

## Configuration

### System Parameters

#### Sampling Configuration
```cpp
int samplingInterval = 2000;    // 2 seconds (2000ms)
int dataInterval = 10000;       // 10 seconds (10000ms)  
int maxSamples = 24;           // Maximum samples per batch
```

#### Servo Configuration
```cpp
int minimumAngle = 30;          // Minimum servo angle (degrees)
float controllingFactor = 0.75; // Response sensitivity (0.0-1.0)
```

#### Environmental Thresholds
```cpp
// Temperature warnings (°C)
#define TEMP_MIN 24
#define TEMP_MAX 32

// Humidity warnings (%)
#define HUMID_MIN 65
#define HUMID_MAX 85
```

#### Alarm Configuration
```cpp
int n_alarms = 2;              // Number of available alarms
int alarm_hours[] = {20, 20};   // Default alarm hours
int alarm_minutes[] = {20, 20}; // Default alarm minutes
```

## Acknowledgments

- **ESP32 Community** for extensive documentation and examples
- **Adafruit** for excellent sensor libraries and tutorials
- **Node-RED Community** for dashboard and flow examples
- **Arduino Community** for platform support and resources