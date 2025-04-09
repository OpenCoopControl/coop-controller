@echo off
echo OpenCoopControl Flashing Tool
echo ============================
echo.

set PORT=COM3
set FIRMWARE=opencoopcontrol_firmware.bin
set SPIFFS=opencoopcontrol_spiffs.bin

echo Select flashing mode:
echo 1. Update firmware only (preserves settings)
echo 2. Full installation (erases all settings)
echo.

set /p MODE="Enter option (1/2): "

if "%MODE%"=="1" (
    echo.
    echo Updating firmware only - settings will be preserved...
    echo.
    
    echo Flashing firmware...
    esptool.py --chip esp32 --port %PORT% --baud 921600 write_flash -z 0x10000 %FIRMWARE%
    
    echo.
    echo Firmware update complete!
) else if "%MODE%"=="2" (
    echo.
    echo Performing full installation - ALL SETTINGS WILL BE ERASED!
    echo.
    
    echo Erasing flash...
    esptool.py --chip esp32 --port %PORT% --baud 921600 erase_flash
    
    echo Flashing bootloader and partition table...
    esptool.py --chip esp32 --port %PORT% --baud 921600 write_flash 0x1000 bootloader.bin 0x8000 partitions.bin
    
    echo Flashing firmware...
    esptool.py --chip esp32 --port %PORT% --baud 921600 write_flash 0x10000 %FIRMWARE%
    
    echo Flashing filesystem...
    esptool.py --chip esp32 --port %PORT% --baud 921600 write_flash 0x310000 %SPIFFS%
    
    echo.
    echo Full installation complete! Device will start in setup mode.
) else (
    echo Invalid option selected. Please run the script again.
    exit /b 1
)

echo.
echo Note: If you need to change the COM port, edit this file and change:
echo   set PORT=COM3
echo to your correct port number.
echo.

pause