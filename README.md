# ESP8266 Wi-Fi Manager Dashboard

This project provides a simple web interface for the ESP8266, designed as a boilerplate to help you quickly get started with your IoT projects.
Allowing the user to connect it to the ESP and configure what network to connect to.

## Features

- **Scan and Connect**: Scan for available Wi-Fi networks and connect the ESP8266 to your chosen network.
- **Access Point Mode**: Configure the ESP8266 to create its own Wi-Fi Access Point for direct connection.
- **Auto-Reconnect**: Save Wi-Fi credentials to EEPROM, allowing the device to automatically reconnect to the last used network after reboot.
- **Responsive Web Interface**: Built with HTML, CSS, and JavaScript, the interface is lightweight, user-friendly, and works across devices.

## Usage

Once configured, you can access the ESP8266 either through your own Wi-Fi network or directly via its Access Point, providing flexibility for a variety of IoT applications.

## Note

This project is intended as a **boilerplate** to help you quickly set up Wi-Fi management for your ESP8266 and build upon it for your own IoT projects.

## How to start

1. **Download Arduino IDE 2 via either**

- [Microsoft Store](https://apps.microsoft.com/detail/xpddtbj80f8pc9)
- [Official Website](https://docs.arduino.cc/software/ide-v2/tutorials/getting-started/ide-v2-downloading-and-installing/)

2. **Configure the IDE**

- Arduino IDE, go to `File` > `Preferences` and add this URL to the `Additional Boards Manager URLs`: `https://arduino.esp8266.com/stable/package_esp8266com_index.json`
- Now go to `Tools` > `Board` > `Boards Manager`, search esp8266, and install ESP8266 Boards
- Select your board at `Tools` > `Board`
- Plug in your board and select its COM port at `Tools` > `Port`
- Optional: To reset/override previous settings, select `Tools` > `Erase Flash` > `All Flash Contents`
- You will need [Arduino ESP8266 filesystem uploader tool](https://github.com/esp8266/arduino-esp8266fs-plugin) to load sketch data into the ESP. [Tutorial](https://randomnerdtutorials.com/arduino-ide-2-install-esp8266-littlefs/) (follow the instructions for Arduino IDE 2)
- Install the `ArduinoJson` library via `Sketch` > `Include Library` > `Manage Libraries...` and search for `ArduinoJson`. Install the version 7.
- Run the `webMinifier` script to minify the HTML, CSS, and JS files. This will create a `data` folder with the minified files inside the `esp-dashboard` directory.
- Upload the `data` folder to the ESP8266 using the ESP8266 filesystem uploader tool. as follows:
  - Select `Ctrl` + `Shift` + `P`, then type `LittleFS to Pico/ESP8266/ESP32` and select it.
  - Note: Make sure to close the serial monitor before uploading the files. If you don't, you will get an error message.

## Default settings

- **IP Address:** `192.168.4.1`
- **SSID:** `ESP-Access-Point`
- **Password:** `123456789`
- **Channel:** `6`
- **Hidden:** `false`

## File Structure

```bash
📁 web-interface        # Contains the source HTML, CSS, and JS files for the web interface
📁 esp-dashboard        # Contains the ESP8266 sketch and the web interface files
│   📁 data
│       📁 web          # Contains the minified HTML, CSS, and JS files for the web interface
📁 utils                # Contains utility scripts for minifying HTML, CSS, and JS files
    📄 webMinifier.js   # Script to minify HTML, CSS, and JS files

```
