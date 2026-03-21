# WIFI Switch User / Assembly Guide

This project is designed to control a device over Wi-Fi using the included web app.

![WIFI switch assembled board](https://github.com/2omethingBaD/WIFI-Remote/blob/main/Pics/20260318_032044.jpg?raw=true)

---

## 📋 Table of Contents
- [Assembly](#assembly)
- [Powering the Device](#powering-the-device)
- [Connecting to Wi-Fi](#connecting-to-wi-fi)
- [Using the Web App](#using-the-web-app)
- [Tech Notes](#tech-notes)

---

## 🧰 Assembly

<details>
<summary><strong>Connecting Your Device or Relay</strong></summary>

Before powering on the system, connect either:

- a **low-power device** that can be driven directly from the microcontroller  
  **or**
- the included **3.3V relay**, if your device requires more power than the microcontroller pin should supply directly (3V)

The screw terminal is labeled as follows:

- **-** → connected to **GND**
- **IN** → connected to the **GPIO control pin**
- **+** → connected to **3.3V**, intended only for powering the relay module if used

Make sure your wires are firmly secured in the screw terminal before continuing.

![Screw terminal overview](https://github.com/2omethingBaD/WIFI-Remote/blob/main/Pics/20260318_032626.jpg?raw=true)
![Relay and terminal wiring example](https://github.com/2omethingBaD/WIFI-Remote/blob/main/Pics/20260318_032057.jpg?raw=true)
![Close-up of terminal connection](https://github.com/2omethingBaD/WIFI-Remote/blob/main/Pics/20260318_032343.jpg?raw=true)

</details>

---

## 🔋 Powering the Device

<details>
<summary><strong>Power Input and Safety</strong></summary>

Once your device is connected, power the system by plugging the provided **buckle-to-USB-C adapter** into the microcontroller.

This adapter includes a **buck converter**, which steps the output of a **9V battery** down to a safer **5V supply** for the microcontroller.

You may also power the board through the microcontroller’s **USB-C port** with any regulated **5V source**.

> **Important:** Do **not** exceed **5V** when powering the microcontroller through USB-C.

![USB-C power connection](https://github.com/2omethingBaD/WIFI-Remote/blob/main/Pics/20260318_032636.jpg?raw=true)

</details>

---

## 📶 Connecting to Wi-Fi

<details>
<summary><strong>Joining the Device Network</strong></summary>

Once powered, the device has **no indicator LEDs**, so the easiest way to confirm it is running is to open your phone or computer’s Wi-Fi settings.

By default, the device broadcasts a Wi-Fi network named:

- **SSID:** `MY_Wifi_Remote`
- **Password:** `12345678`

If you do not see the network, make sure your battery is charged and the board is receiving power properly.

![Wi-Fi network on phone](https://github.com/2omethingBaD/WIFI-Remote/blob/main/Pics/Screenshot_20260311_235254_One%20UI%20Home.jpg?raw=true)

</details>

---

## 🌐 Using the Web App

<details>
<summary><strong>Opening the Controller</strong></summary>

After connecting to the device’s Wi-Fi network, open your browser and go to:

**`192.168.1.1`**

You can also access the page by scanning the provided QR code.

![QR code for web app](https://github.com/2omethingBaD/WIFI-Remote/blob/main/Pics/Screenshot%202026-03-18%20152705.png?raw=true)

</details>

<details>
<summary><strong>Choosing Switch Behavior</strong></summary>

Inside the web app, you can choose between two switch modes:

- **Momentary Switch**  
  The output stays on **only while the button is being held down**

- **Latching Switch**  
  The output remains **on or off until changed again**, even if you disconnect from the system

This lets the same hardware behave either like a pushbutton or like a toggle switch depending on your project.

![Momentary mode screen](https://github.com/2omethingBaD/WIFI-Remote/blob/main/Pics/Screenshot_20260318_031517_Chrome.jpg?raw=true)
![Latching mode screen](https://github.com/2omethingBaD/WIFI-Remote/blob/main/Pics/Screenshot_20260318_031529_Chrome.jpg?raw=true)

</details>

---

## 🧠 Tech Notes

<details>
<summary><strong>What the parts are doing</strong></summary>

### ESP32 Wi-Fi Access Point
This project uses the microcontroller in **Access Point (AP) mode**, which means it creates its own Wi-Fi network instead of joining your home router. That is why you connect directly to the board before opening the web app.

### Static Local IP
The board is configured with a fixed local IP address so the controller page always loads from the same address: **192.168.1.1**.

### Buck Converter
A **buck converter** is a switching regulator that efficiently steps a higher DC voltage down to a lower one. In this project, it allows a 9V battery source to be used more safely with a 5V-powered board.

### Relay Basics
A **relay** is an electrically controlled switch. A low-power signal from the microcontroller can control a separate load through the relay, which is useful when the attached device should not be powered directly from a GPIO pin.

### Important Relay Reminder
Always stay within the **relay’s voltage and current ratings**. The relay protects the microcontroller from switching the load directly, but it still has electrical limits of its own.

</details>

---

💡 *Created by [SomethingBAD Studios](https://github.com/2omethingBaD) — “BAD ideas, genius built.”*
