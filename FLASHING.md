# Flashing OpenCoopControl to your ESP32

This guide will help you flash the OpenCoopControl firmware to your ESP32 device.

## Prerequisites

1. An ESP32 development board
2. USB cable to connect the board to your computer
3. Python installed on your computer

## Installation Methods

### Method 1: Using Release Assets (Recommended for most users)

1. Go to the [Releases page](https://github.com/OpenCoopControl/coop-controller/releases) of the repository
2. Download the latest release assets:
   - For Windows: `flash_opencoopcontrol_vX.Y.Z.bat`
   - For Linux/Mac: `flash_opencoopcontrol_vX.Y.Z.sh`
   - Binary files: 
     * `opencoopcontrol_vX.Y.Z_firmware.bin` (Main firmware)
     * `opencoopcontrol_vX.Y.Z_spiffs.bin` (Filesystem)
     * `bootloader.bin` (Required for full installation)
     * `partitions.bin` (Required for full installation)

3. Install esptool:
   ```bash
   pip install esptool
   ```

4. Connect your ESP32 to your computer

5. Run the flashing script:
   - On Windows: Double-click the .bat file and follow the prompts
   - On Linux/Mac: 
     ```bash
     chmod +x flash_opencoopcontrol_vX.Y.Z.sh
     ./flash_opencoopcontrol_vX.Y.Z.sh
     ```

6. Choose your flashing mode:
   - **Option 1: Update firmware only** - This will keep your WiFi settings and configurations
   - **Option 2: Full installation** - This will erase all settings and return the device to factory state

### Method 2: Building from Source (For developers)

If you want to build the firmware yourself:

1. Install PlatformIO:
   ```bash
   pip install platformio
   ```

2. Clone the repository:
   ```bash
   git clone https://github.com/OpenCoopControl/coop-controller.git
   cd coop-controller
   ```

3. Build and upload:
   ```bash
   platformio run --target upload
   ```

4. Upload the file system (contains the web interface):
   ```bash
   platformio run --target uploadfs
   ```

## Troubleshooting

### Common Issues

1. **Permission denied on Linux/Mac**:
   ```bash
   sudo chmod a+rw /dev/ttyUSB0  # Replace with your port!
   ```

2. **Can't find COM port on Windows**:
   - Check Device Manager to identify the correct COM port
   - Edit the .bat file to update the port number

3. **Connection issues**:
   - Make sure the ESP32 is in download mode by holding the BOOT button while connecting or resetting
   - Try a different USB cable or port

4. **Flash failed**:
   - Try a slower baud rate by editing the flash script to use `--baud 115200` instead of 921600

5. **Unable to connect to WiFi after flashing**:
   - If you chose "Update firmware only" but are having issues, try the "Full installation" option instead
   - After a full installation, the device will create a WiFi AP named "OpenCoop-[ID]"

### LED Status Indicators

- When the ESP32 is properly flashed, the built-in LED will blink once during startup
- If the LED stays on, it indicates the device is in setup mode

## After Flashing

### First-time Setup or After Full Installation

1. The ESP32 will create a WiFi access point named "OpenCoop-[ID]"
2. Connect to this WiFi network with your phone or computer
3. Open a web browser and navigate to http://192.168.4.1
4. Follow the setup wizard to complete configuration

### After Firmware Update Only

1. The device will reconnect to your WiFi network automatically
2. Access the web interface using the device's IP address on your network

For more information, see the [README.md](README.md) file.