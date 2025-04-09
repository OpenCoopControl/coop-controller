FROM python:3.9-slim

LABEL maintainer="OpenCoopControl Team" \
      description="Development environment for OpenCoopControl ESP32 firmware"

# Install dependencies
RUN apt-get update && apt-get install -y \
    git \
    make \
    gcc \
    g++ \
    curl \
    udev \
    zip \
    unzip \
    libusb-1.0-0 \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Install PlatformIO
RUN pip install --no-cache-dir platformio

# Install additional tools
RUN pip install --no-cache-dir \
    esptool \
    pyserial

# Set working directory
WORKDIR /opencoopcontrol

# Create .platformio directory to store libraries and toolchains
RUN mkdir -p /root/.platformio

# Download ESP32 platform and common libraries ahead of time
RUN platformio platform install espressif32

# Setup udev rules for ESP32 (helps with USB detection when mounted)
RUN curl -fsSL https://raw.githubusercontent.com/platformio/platformio-core/master/scripts/99-platformio-udev.rules > /etc/udev/rules.d/99-platformio-udev.rules

# Add a script to initialize the environment
RUN echo '#!/bin/bash\ncd /opencoopcontrol\nexec "$@"' > /entrypoint.sh && chmod +x /entrypoint.sh

ENTRYPOINT ["/entrypoint.sh"]
CMD ["bash"]