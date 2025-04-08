@echo off
echo OpenCoopControl Development Environment Setup
echo ==============================================
echo.

REM Check for Python
echo Checking for Python...
python --version >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Python not found. Please install Python 3.x.
    echo Download from: https://www.python.org/downloads/
    exit /b 1
)

echo Found Python: 
python --version

REM Check for pip
echo Checking for pip...
pip --version >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: pip not found. Please ensure pip is installed with Python.
    exit /b 1
)

echo Found pip:
pip --version

REM Install PlatformIO
echo Installing/Upgrading PlatformIO...
pip install -U platformio

REM Install required Python packages
echo Installing required Python packages...
pip install -U esptool pyserial

REM Check for Git
echo Checking for Git...
git --version >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo WARNING: Git not found. It's recommended to install Git for version control.
    echo Download from: https://git-scm.com/download/win
) else (
    echo Found Git:
    git --version
)

REM Check for VS Code (optional)
echo Checking for Visual Studio Code...
where code >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo Visual Studio Code not found. While not required, it's recommended for development.
    echo Download from: https://code.visualstudio.com/
) else (
    echo Found VS Code
    
    REM Install PlatformIO VSCode extension
    echo Installing PlatformIO extension for VS Code...
    code --install-extension platformio.platformio-ide
)

echo.
echo Setup completed! You can now open the project in VS Code or use PlatformIO CLI.
echo.
echo Next steps:
echo 1. Clone the repository: git clone https://github.com/OpenCoopControl/coop-controller.git
echo 2. Open the folder in VS Code
echo 3. Build the project with PlatformIO
echo.
echo Happy coding!

pause