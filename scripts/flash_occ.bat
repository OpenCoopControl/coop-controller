@echo off
echo OpenCoopControl Flashing Tool
echo ============================
echo.

set FIRMWARE=opencoopcontrol_firmware.bin
set SPIFFS=opencoopcontrol_spiffs.bin
set BOOTLOADER=bootloader.bin
set PARTITIONS=partitions.bin

REM Check if files exist
if not exist "%FIRMWARE%" (
    echo Error: Firmware file (%FIRMWARE%) not found in current directory!
    goto :exit
)

if not exist "%BOOTLOADER%" (
    echo Error: Bootloader file (%BOOTLOADER%) not found in current directory!
    goto :exit
)

if not exist "%PARTITIONS%" (
    echo Error: Partitions file (%PARTITIONS%) not found in current directory!
    goto :exit
)

set SPIFFS_EXISTS=true
if not exist "%SPIFFS%" (
    echo Note: SPIFFS file (%SPIFFS%) not found, will skip filesystem installation.
    set SPIFFS_EXISTS=false
)

REM List available COM ports
echo Scanning for available COM ports...
echo.
powershell -command "Get-WmiObject Win32_PnPEntity | Where-Object { $_.Name -match 'COM\d+' } | ForEach-Object { $_.Name } | Sort-Object"
echo.

set /p PORT="Enter COM port (e.g., COM3): "

REM Validate COM port format
set PORT=%PORT: =%
if "%PORT:~0,3%" neq "COM" (
    set PORT=COM%PORT%
)

echo Using port: %PORT%
echo.

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
    esptool.py --chip esp32 --port %PORT% --baud 115200 write_flash -z 0x10000 %FIRMWARE%
    if %ERRORLEVEL% neq 0 (
        echo Error flashing firmware. Please try again.
        goto :exit
    )
    
    echo.
    echo Firmware update complete!
) else if "%MODE%"=="2" (
    echo.
    echo Performing full installation - ALL SETTINGS WILL BE ERASED!
    echo.
    
    echo Erasing flash...
    esptool.py --chip esp32 --port %PORT% --baud 115200 erase_flash
    if %ERRORLEVEL% neq 0 (
        echo Error erasing flash. Please try again.
        goto :exit
    )
    
    echo Flashing bootloader and partition table...
    esptool.py --chip esp32 --port %PORT% --baud 115200 write_flash 0x1000 %BOOTLOADER% 0x8000 %PARTITIONS%
    if %ERRORLEVEL% neq 0 (
        echo Error flashing bootloader or partitions. Please try again.
        goto :exit
    )
    
    echo Flashing firmware...
    esptool.py --chip esp32 --port %PORT% --baud 115200 write_flash 0x10000 %FIRMWARE%
    if %ERRORLEVEL% neq 0 (
        echo Error flashing firmware. Please try again.
        goto :exit
    )
    
    if "%SPIFFS_EXISTS%"=="true" (
        echo Flashing filesystem...
        esptool.py --chip esp32 --port %PORT% --baud 115200 write_flash 0x310000 %SPIFFS%
        if %ERRORLEVEL% neq 0 (
            echo Error flashing filesystem. Please try again.
            goto :exit
        )
    ) else (
        echo Skipping filesystem flashing (SPIFFS file not found).
    )
    
    echo.
    echo Full installation complete! Device will start in setup mode.
) else (
    echo Invalid option selected. Please run the script again.
    goto :exit
)

echo.

:exit
echo.
pause