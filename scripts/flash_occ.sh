#!/bin/bash
echo "OpenCoopControl Flashing Tool"
echo "============================"
echo

PORT="/dev/ttyUSB0"
FIRMWARE="opencoopcontrol_firmware.bin"
SPIFFS="opencoopcontrol_spiffs.bin"

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
    esptool.py --chip esp32 --port $PORT --baud 921600 write_flash -z 0x10000 $FIRMWARE
    
    echo
    echo "Firmware update complete!"
elif [ "$MODE" = "2" ]; then
    echo
    echo "Performing full installation - ALL SETTINGS WILL BE ERASED!"
    echo
    
    echo "Erasing flash..."
    esptool.py --chip esp32 --port $PORT --baud 921600 erase_flash
    
    echo "Flashing bootloader and partition table..."
    esptool.py --chip esp32 --port $PORT --baud 921600 write_flash 0x1000 bootloader.bin 0x8000 partitions.bin
    
    echo "Flashing firmware..."
    esptool.py --chip esp32 --port $PORT --baud 921600 write_flash 0x10000 $FIRMWARE
    
    echo "Flashing filesystem..."
    esptool.py --chip esp32 --port $PORT --baud 921600 write_flash 0x310000 $SPIFFS
    
    echo
    echo "Full installation complete! Device will start in setup mode."
else
    echo "Invalid option selected. Please run the script again."
    exit 1
fi

echo
echo "Note: If you need to change the port, edit this file and change:"
echo "  PORT=\"/dev/ttyUSB0\""
echo "to your correct port."
echo

read -p "Press Enter to exit..."