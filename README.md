# Boardoza HP303B Barometric Pressure and Temperature Sensor Breakout Board

The **Boardoza HP303B Breakout Board** is a compact and low-power digital **barometric pressure and temperature sensor** designed for applications requiring high accuracy and long-term stability. Based on a **capacitive pressure sensing principle**, the HP303B delivers precise measurements even under changing temperature conditions, making it suitable for demanding environmental sensing tasks.

Each HP303B sensor is **individually calibrated at the factory** and includes a **24-bit internal signal processor** that converts raw sensor data into accurate pressure and temperature values. With support for both **I²C and SPI communication**, an integrated **32-sample FIFO**, and optional **interrupt functionality**, this breakout board is ideal for **IoT devices, wearables, mobile electronics, weather stations, altitude measurement systems, and battery-powered sensor nodes**.

## [Click here to purchase!](https://www.ozdisan.com/p/arduino-sensorleri-ve-modulleri-613/boardoza-hp303b-module-1473941)

| Front Side | Back Side |
|:---:|:---:|
| ![HP303B Front](./assets/HP303B%20Front.png) | ![HP303B Back](./assets/HP303B%20Back.png) |

---

## Key Features

- **High-Accuracy Barometric Pressure Sensor:** Designed for precise pressure and altitude measurements across a wide operating range.
- **Integrated Temperature Measurement:** Provides accurate ambient temperature data alongside pressure readings.
- **Factory-Calibrated Sensor:** Each HP303B device is individually calibrated, ensuring reliable and consistent output.
- **Dual Communication Interface:** Supports both **I²C** and **SPI** interfaces for flexible system integration.
- **32-Sample FIFO Buffer:** Reduces host processor load by minimizing frequent data polling.
- **Low Power Consumption:** Optimized for battery-powered and low-energy applications such as wearables and IoT nodes.
- **Interrupt Support:** Optional interrupt signaling via the SDO pin for efficient event-driven designs.

---

## Technical Specifications

**Model:** HP303B  
**Manufacturer:** Boardoza   
**Manufacturer IC:** HOPERF  
**Functions:** Digital Barometric Pressure and Temperature Sensor  
**Supply Voltage:** 3.3 V – 5 V  
**Interface:** I²C, SPI  
**Pressure Measurement Range:** 300 hPa – 1200 hPa  
**Pressure Precision:** ±0.005 hPa (high precision mode)  
**Relative Accuracy:** ±0.06 hPa  
**Absolute Accuracy:** ±1 hPa  
**Temperature Accuracy:** ±0.5 °C  
**Pressure Temperature Sensitivity:** 0.5 Pa/K  
**Measurement Time:**  
- Typical: 27.6 ms (standard mode, 16x)  
- Minimum: 3.6 ms (low precision mode)  
  
**Average Current Consumption:**  
- Pressure Measurement: 1.7 µA @ 1 Hz  
- Temperature Measurement: 1.5 µA @ 1 Hz  
- Standby Mode: 0.5 µA

**FIFO Buffer Depth:** 32 samples  
**Operating Temperature:** -40 °C to +85 °C  
**Board Dimensions:** 20 mm x 20 mm

---

## Board Pinout

### **( J1 ) Main Connector**

| Pin Number | Pin Name | Description |
|:---:|:---:|---|
| 1 | 3V3-5V | Positive Power Supply |
| 2 | SDO | I²C: Address LSB / Interrupt<br>SPI: MISO / Interrupt |
| 3 | CSB | I²C: Not used (leave open)<br>SPI: Chip Select |
| 4 | SDA | I²C: Serial Data<br>SPI: MOSI (4-wire) / Data (3-wire) |
| 5 | SCL | I²C: Serial Clock<br>SPI: Serial Clock |
| 6 | GND | Ground |

### **Jumper Configuration**

| Jumper | Description |
|------|------------|
| JP1, JP2 | SPI mode: Open (default)<br>I²C mode: Short |
| JP3 | I²C Address 0x77: Open (default)<br>I²C Address 0x76: Short |

---

## Board Dimensions

<img src="./assets/HP303B Dimensions.png" alt="HP303B Board Dimensions" width="450"/>

---

## Step Files

[Boardoza HP303B.step](./assets/HP303B%20Step.step)

---

## Datasheet

[HP303B Datasheet](./assets/HP303B%20Datasheet.pdf)

---

## Version History

- V1.0.0 – Initial Release

---

## Support

- If you have any questions or need support, please contact **support@boardoza.com**

---

## **License**

This repository contains both hardware and software components:

### **Hardware Design**

[![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

All hardware design files are licensed under [Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg

### **Software/Firmware**

[![BSD-3-Clause][bsd-shield]][bsd]

All software and firmware are licensed under [BSD 3-Clause License][bsd].

[bsd]: https://opensource.org/licenses/BSD-3-Clause
[bsd-shield]: https://img.shields.io/badge/License-BSD%203--Clause-blue.svg
