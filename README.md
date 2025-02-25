```markdown
# Air Quality Monitoring Project with MQ-135 and MQ-136 Gas Sensors

## Overview
This project uses the **MQ-135** and **MQ-136** gas sensors to monitor air quality by detecting various gases such as CO, Alcohol, CO2, Toluene, NH4 (Ammonia), Acetone, and H2S (Hydrogen Sulfide). The sensors are connected to an Arduino UNO, which processes the sensor data and outputs the concentration of gases in parts per million (PPM).

The project is designed for educational purposes, environmental monitoring, or indoor air quality assessment.

---

## Features
- **Multi-gas Detection**: Detects multiple gases including CO, Alcohol, CO2, Toluene, NH4, Acetone, and H2S.
- **Calibration**: Automatically calibrates the MQ-135 sensor in clean air during startup.
- **Real-time Monitoring**: Outputs gas concentrations in real-time via the serial monitor.
- **Customizable**: Adjustable parameters for different environments and sensor configurations.
- **Simple Setup**: Easy-to-follow wiring and code structure.

---

## Hardware Components
1. **Arduino UNO** - Microcontroller board.
2. **MQ-135 Sensor** - Detects gases like CO, Alcohol, CO2, Toluene, NH4, and Acetone.
3. **MQ-136 Sensor** - Specialized for detecting Hydrogen Sulfide (H2S).
4. **Breadboard and Jumper Wires** - For connecting sensors to the Arduino.
5. **Power Supply** - Ensure a stable 5V supply for the sensors.

---

## Wiring Diagram
### MQ-135 Connections:
- **VCC** → 5V on Arduino
- **GND** → GND on Arduino
- **AOUT** → A0 on Arduino

### MQ-136 Connections:
- **VCC** → 5V on Arduino
- **GND** → GND on Arduino
- **AOUT** → A1 on Arduino
- **DOUT** → D2 on Arduino (optional for digital threshold detection)

---

## Software Setup
### Prerequisites
1. Install the Arduino IDE from [Arduino's official website](https://www.arduino.cc/en/software).
2. Install the **MQUnifiedsensor** library:
   - Open Arduino IDE.
   - Go to `Sketch` → `Include Library` → `Manage Libraries`.
   - Search for "MQUnifiedsensor" and install it.

### Code Upload
1. Copy the provided code into a new Arduino sketch.
2. Connect your Arduino UNO to your computer via USB.
3. Select the correct board and port under `Tools` → `Board` and `Tools` → `Port`.
4. Click the `Upload` button to upload the code to the Arduino.

---

## Calibration
### MQ-135 Calibration
- Place the sensor in clean air (preferably outdoors or in a well-ventilated area).
- Power on the Arduino and wait for the calibration process to complete (indicated by "Calibrating please wait." in the serial monitor).
- The calculated `R0` value will be displayed in the serial monitor. This value is critical for accurate readings.

### MQ-136 Calibration
- The MQ-136 sensor does not require pre-calibration but relies on the `calculateR0()` function to determine the baseline resistance in clean air.
- Ensure the sensor is exposed to clean air during the initial power-up for accurate results.

---

## Usage
1. Open the **Serial Monitor** in the Arduino IDE (`Tools` → `Serial Monitor`).
2. Set the baud rate to **9600**.
3. Observe the real-time gas concentration readings for both MQ-135 and MQ-136 sensors.

### Sample Output
```
** Values from MQ-135 ****
|    CO   |  Alcohol |   CO2  |  Toluen  |  NH4  |  Aceton  |
|   10.5   |   5.2   |   420.0   |   2.1   |   8.3   |   3.7   |

** Values from MQ-136 ****
|  Raw Value   |  Voltage |   Rs  |  Ohms, H2S Concentration 
   |   512   |   2.50   |   10000.0   |   1.2 ppm
```

---

## Notes
1. **Sensor Warm-up Time**: Both MQ-135 and MQ-136 sensors require a warm-up time of approximately 2-3 minutes before providing stable readings.
2. **Environmental Factors**: Humidity, temperature, and other environmental factors can affect sensor accuracy. Use in controlled conditions for best results.
3. **Calibration Adjustments**: If readings seem inaccurate, recalibrate the sensors in clean air or adjust the sensitivity curve coefficients in the code.

---

## Troubleshooting
1. **Infinite or Zero R0 Value**:
   - Check wiring connections.
   - Ensure the sensor is powered correctly.
   - Verify that the sensor is exposed to clean air during calibration.
2. **Noisy Readings**:
   - Add a capacitor (e.g., 10µF) between VCC and GND of the sensor to reduce noise.
   - Increase the delay between readings for more stable output.

---

## License
This project is open-source and available under the MIT License. Feel free to modify and use it for personal or educational purposes.

---

## Acknowledgments
- **MQUnifiedsensor Library**: https://github.com/miguel5612/MQSensorsLib
- **Gas Sensor Datasheets**:
  - MQ-135: https://www.olimex.com/Products/Components/Sensors/SNS-MQ135/resources/SNS-MQ135.pdf
  - MQ-136: https://www.sparkfun.com/datasheets/Sensors/Biometric/MQ-136.pdf

---

## Contact
For questions or feedback, feel free to reach out:
- Email: sunny@ajiroghene.dev
- GitHub: [Ajioz](https://github.com/ajioz)
```