#!/bin/bash
echo "OpenCoopControl Flashing Tool"
echo "============================"
echo

# Detect operating system
if [[ "$OSTYPE" == "darwin"* ]]; then
    # macOS
    echo "Detected macOS system"
    echo "Scanning for available ports..."
    
    # Create array of available ports
    ports=()
    
    # Find available serial ports
    mac_ports=$(ls /dev/cu.* 2>/dev/null)
    if [ -z "$mac_ports" ]; then
        echo "No serial ports found. Make sure your ESP32 is connected."
        echo "You might need to install the driver: https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers"
        exit 1
    fi
    
    # Add each port to the array
    while read -r port; do
        if [[ "$port" != *"Bluetooth"* ]]; then
            ports+=("$port")
        fi
    done <<< "$mac_ports"
    
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    # Linux
    echo "Detected Linux system"
    echo "Scanning for available ports..."
    
    # Create array of available ports
    ports=()
    
    # Common Linux ports for ESP32
    for port in /dev/ttyUSB* /dev/ttyACM*; do
        if [ -e "$port" ]; then
            ports+=("$port")
        fi
    done
    
    if [ ${#ports[@]} -eq 0 ]; then
        echo "No serial ports found. Make sure your ESP32 is connected."
        echo "You might need to add your user to the 'dialout' group:"
        echo "sudo usermod -a -G dialout \$USER"
        echo "Then log out and log back in."
        exit 1
    fi
else
    # Default to /dev/ttyUSB0 for other systems
    ports=("/dev/ttyUSB0")
    echo "NOTE: For Windows users, run the .bat script instead!"
fi

# If multiple ports found, let user select
if [ ${#ports[@]} -gt 1 ]; then
    echo
    echo "Multiple serial ports found. Please select the correct port:"
    for i in "${!ports[@]}"; do
        echo "  $((i+1)). ${ports[$i]}"
    done
    echo
    
    # Prompt user to select a port
    selected=0
    while [ $selected -lt 1 ] || [ $selected -gt ${#ports[@]} ]; do
        read -p "Enter port number (1-${#ports[@]}): " selected
        if ! [[ "$selected" =~ ^[0-9]+$ ]]; then
            selected=0
        fi
    done
    
    PORT="${ports[$((selected-1))]}"
elif [ ${#ports[@]} -eq 1 ]; then
    PORT="${ports[0]}"
    echo "Found port: $PORT"
else
    echo "No ports found. Please check your connection."
    exit 1
fi

echo "Using port: $PORT"
echo

# Binary files
FIRMWARE="opencoopcontrol_firmware.bin"
SPIFFS="opencoopcontrol_spiffs.bin"
BOOTLOADER="bootloader.bin"
PARTITIONS="partitions.bin"

# Check if files exist
if [ ! -f "$FIRMWARE" ]; then
    echo "Error: Firmware file ($FIRMWARE) not found in current directory!"
    exit 1
fi

if [ ! -f "$BOOTLOADER" ]; then
    echo "Error: Bootloader file ($BOOTLOADER) not found in current directory!"
    exit 1
fi

if [ ! -f "$PARTITIONS" ]; then
    echo "Error: Partitions file ($PARTITIONS) not found in current directory!"
    exit 1
fi

# SPIFFS is optional
SPIFFS_EXISTS=true
if [ ! -f "$SPIFFS" ]; then
    echo "Note: SPIFFS file ($SPIFFS) not found, will skip filesystem installation."
    SPIFFS_EXISTS=false
fi

echo "Select flashing mode:"
echo "1. Update firmware only (preserves settings)"
echo "2. Full installation (erases all settings)"
echo

read -p "Enter option (1/2): " MODE

if [ "$MODE" = "1" ]; then
    echo
    echo "Updating firmware only - settings will be preserved..."
    echo
    
    echo "Flashing firmware..."
    esptool.py --chip esp32 --port "$PORT" --baud 921600 write_flash -z 0x10000 "$FIRMWARE"
    if [ $? -ne 0 ]; then
        echo "Error flashing firmware. Please try again."
        exit 1
    fi
    
    echo
    echo "Firmware update complete!"

elif [ "$MODE" = "2" ]; then
    echo
    echo "Performing full installation - ALL SETTINGS WILL BE ERASED!"
    echo
    
    echo "Erasing flash..."
    esptool.py --chip esp32 --port "$PORT" --baud 921600 erase_flash
    if [ $? -ne 0 ]; then
        echo "Error erasing flash. Please try again."
        exit 1
    fi
    
    echo "Flashing bootloader and partition table..."
    esptool.py --chip esp32 --port "$PORT" --baud 921600 write_flash 0x1000 "$BOOTLOADER" 0x8000 "$PARTITIONS"
    if [ $? -ne 0 ]; then
        echo "Error flashing bootloader or partitions. Please try again."
        exit 1
    fi
    
    echo "Flashing firmware..."
    esptool.py --chip esp32 --port "$PORT" --baud 921600 write_flash 0x10000 "$FIRMWARE"
    if [ $? -ne 0 ]; then
        echo "Error flashing firmware. Please try again."
        exit 1
    fi
    
    if [ "$SPIFFS_EXISTS" = true ]; then
        echo "Flashing filesystem..."
        esptool.py --chip esp32 --port "$PORT" --baud 921600 write_flash 0x310000 "$SPIFFS"
        if [ $? -ne 0 ]; then
            echo "Error flashing filesystem. Please try again."
            exit 1
        fi
    else
        echo "Skipping filesystem flashing (SPIFFS file not found)."
    fi
    
    echo
    echo "Full installation complete! Device will start in setup mode."
else
    echo "Invalid option selected. Please run the script again."
    exit 1
fi

echo
echo "If you need to change the port in the future, run this script again and select a different port."
echo
read -p "Press Enter to exit..."