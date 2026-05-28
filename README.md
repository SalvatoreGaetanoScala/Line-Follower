# 🤖 Line Follower Robot - Elegoo Uno R3

This repository contains the firmware and detailed documentation for building an autonomous **Line Follower Robot**. The project is based on the **Elegoo Uno R3** microcontroller, the **L298N** H-bridge motor driver, and a pair of infrared (IR) sensors for path tracking.

## 👤 Author
* **Student:** Scala Salvatore Gaetano
* **Student ID (Matricola):** 1000100183

---

## 🛠️ Project Components

1. **Brain and Logic:**
   * 1x **Elegoo Uno R3** Microcontroller (Arduino Uno compatible)
   * 1x **L298N** Motor Driver (H-Bridge module for motor power control)
2. **Sensors (The "Eyes"):**
   * 2x IR Line Tracking Sensors (Digital output `D0`, `A0` pin unused)
3. **Actuators and Movement:**
   * 2x DC Motors with gearboxes (classic yellow robotics motors)
   * 2x Drive wheels + 1x Caster wheel for stability
4. **Power Supply and Manual Control:**
   * 2x 9V Batteries:
     * *Battery 1:* Motor power supply (connected to the L298N driver).
     * *Battery 2:* Logic power supply (connected via DC jack to the Arduino).
   * 1x Toggle Switch/Button (Black switch to cut off motor power).
5. **Prototyping:**
   * 1x Breadboard (Used to distribute 5V and GND to the sensors).
   * Jumper Wires (Male-to-Male and Male-to-Female).
   * Robot Chassis for mechanical assembly.

---

## 🔌 Wiring Scheme

### 1. Motors & L298N Driver
* **Left Motor (SX):** Connected to the `OUT1` and `OUT2` terminals of the L298N driver.
* **Right Motor (DX):** Connected to the `OUT3` and `OUT4` terminals of the L298N driver.

### 2. Power Supply and Common Ground (GND)
* **Motor Line:** Positive pole (`+`) of Battery 1 $\rightarrow$ Switch $\rightarrow$ `12V` terminal of the L298N driver.
* **Ground Line:** Negative pole (`-`) of Battery 1 $\rightarrow$ `GND` terminal of the L298N driver.
* **Common Ground (CRUCIAL):** A wire must connect the `GND` terminal of the L298N driver to one of the Arduino `GND` pins to align the logic reference potentials.
* **Arduino Line:** Battery 2 powers the Arduino directly via the round black jack plug (or by connecting the red wire to `VIN` and black to `GND`).

### 3. Pin Mapping to Elegoo Uno R3
The following table summarizes the logic and control signal connections:

| Component | Module Pin | Arduino Pin | Pin Type | Function |
| :--- | :---: | :---: | :---: | :--- |
| **L298N Driver** | `ENA` | **5** | OUTPUT (PWM) | Speed control (throttle) Right motor |
| **L298N Driver** | `ENB` | **6** | OUTPUT (PWM) | Speed control (throttle) Left motor |
| **L298N Driver** | `IN1` | **8** | OUTPUT | Right motor direction |
| **L298N Driver** | `IN2` | **9** | OUTPUT | Right motor direction |
| **L298N Driver** | `IN3` | **10** | OUTPUT | Left motor direction |
| **L298N Driver** | `IN4` | **11** | OUTPUT | Left motor direction |
| **Left IR Sensor** | `D0` | **2** | INPUT | Digital reading of the left "eye" |
| **Right IR Sensor**| `D0` | **3** | INPUT | Digital reading of the right "eye" |

> ℹ️ *Note on Sensors:* The `VCC` and `GND` pins of both sensors must be connected to the red (`+`) and blue/black (`-`) rails of the breadboard, which is powered by the `5V` and `GND` pins of the Arduino.

---

## 🧠 Movement Logic (Track Algorithm)

The robot is designed as a **continuous correction system**. The goal is to move forward while keeping the black line exactly centered between the two sensors. The behavior follows 4 strictly defined logic scenarios:
