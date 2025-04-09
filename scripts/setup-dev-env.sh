echo "OpenCoopControl Development Environment Setup"
echo "=============================================="
echo

echo "Checking for Python..."
if command -v python3 &> /dev/null; then
    PYTHON_CMD="python3"
elif command -v python &> /dev/null; then
    PYTHON_CMD="python"
else
    echo "ERROR: Python not found. Please install Python 3.x."
    exit 1
fi

PYTHON_VERSION=$($PYTHON_CMD --version)
echo "Found $PYTHON_VERSION"

echo "Checking for pip..."
if command -v pip3 &> /dev/null; then
    PIP_CMD="pip3"
elif command -v pip &> /dev/null; then
    PIP_CMD="pip"
else
    echo "ERROR: pip not found. Please install pip."
    exit 1
fi

echo "Found pip: $($PIP_CMD --version)"

echo "Installing/Upgrading PlatformIO..."
$PIP_CMD install -U platformio

echo "Installing required Python packages..."
$PIP_CMD install -U esptool pyserial

echo "Checking for Git..."
if ! command -v git &> /dev/null; then
    echo "WARNING: Git not found. It's recommended to install Git for version control."
else
    echo "Found Git: $(git --version)"
fi

echo "Checking for Visual Studio Code..."
if command -v code &> /dev/null; then
    echo "Found VS Code: $(code --version | head -n 1)"
    
    echo "Installing PlatformIO extension for VS Code..."
    code --install-extension platformio.platformio-ide
else
    echo "Visual Studio Code not found. While not required, it's recommended for development."
    echo "Download from: https://code.visualstudio.com/"
fi

if command -v git &> /dev/null; then
    echo "Setting up git hooks..."
    HOOKS_DIR=".git/hooks"
    
    if [ -d "$HOOKS_DIR" ]; then
    PRE_COMMIT="$HOOKS_DIR/pre-commit"
    
    cat > "$PRE_COMMIT" << 'EOF'
#!/bin/bash
# Pre-commit hook to ensure code builds correctly

echo "Running PlatformIO check before commit..."
pio run -e esp32dev 2>&1 > /dev/null

if [ $? -ne 0 ]; then
    echo "ERROR: Build failed. Please fix the build errors before committing."
    exit 1
fi

echo "Build check passed."
exit 0
EOF
    
    chmod +x "$PRE_COMMIT"
    echo "Git pre-commit hook installed."
fi