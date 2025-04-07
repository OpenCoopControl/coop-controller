# OpenCoopControl - ESP32 Chicken Coop Controller

This repository contains the ESP32 firmware for the OpenCoopControl automated chicken coop system. The controller manages coop door operations, feeding schedules, and environmental monitoring through various sensors.

## Features

- **Automated Door Control**: Schedule door opening/closing based on time
- **Automated Feeder**: Configure feeding times and durations
- **Environmental Monitoring**: Track temperature, light levels, and water levels
- **Web Interface**: Local web dashboard for monitoring and control
- **API Endpoints**: Control and monitor your coop through HTTP requests
- **Easy Setup**: WiFi Manager for easy configuration without reflashing
- **Cloud Integration**: Optional connection to OpenCoopControl cloud service

## Hardware Requirements

- ESP32 Development Board
- Servo Motor (for door control)
- DC Motor (for feeder)
- Temperature Sensor (analog)
- Light Sensor (photoresistor)
- Water Level Sensor
- Push Button (for manual control)

## Pin Configuration

| Component           | ESP32 Pin | Description                |
|---------------------|-----------|----------------------------|
| Temperature Sensor  | GPIO 32   | Analog input               |
| Light Sensor        | GPIO 33   | Analog input               |
| Door Servo          | GPIO 13   | PWM output                 |
| Feeder Motor        | GPIO 12   | Digital output             |
| Water Level Sensor  | GPIO 34   | Analog input               |
| Manual Door Button  | GPIO 14   | Digital input (pull-up)    |
| Status LED          | GPIO 2    | Digital output (built-in)  |

## Getting Started

### Installation

1. Clone this repository:
   ```
   git clone https://github.com/OpenCoopControl/coop-controller.git
   cd coop-controller
   ```

2. Open the project with PlatformIO in Visual Studio Code or your preferred IDE.

3. Build and upload to your ESP32 device:
   ```
   platformio run --target upload
   ```

4. Upload the filesystem (contains the web interface):
   ```
   platformio run --target uploadfs
   ```

### First-Time Setup

1. When first powered on, the device will create a WiFi access point named "OpenCoop-[ID]".

2. Connect to this WiFi network with your phone or computer.

3. Open a web browser and navigate to http://192.168.4.1

4. Follow the setup wizard to:
   - Connect to your home WiFi
   - Name your coop
   - Get your unique device ID for cloud registration

5. After setup, the controller will connect to your WiFi network and is ready to use.

## Web Interface

Once set up, you can access the controller's web interface by navigating to its IP address on your local network. The interface allows you to:

- View current temperature, light level, and water level
- Check door and feeder status
- Manually open/close the door
- Manually activate the feeder
- Configure automation settings

## API Endpoints

The controller provides the following HTTP endpoints for integration:

- `GET /api/state` - Get current sensor readings and device status
- `GET /api/settings` - Get current automation settings
- `POST /api/settings` - Update automation settings
- `POST /api/door/open` - Open the coop door
- `POST /api/door/close` - Close the coop door
- `POST /api/feeder/activate` - Activate the feeder (optional duration parameter)

## Cloud Integration

To connect your controller to the OpenCoopControl cloud service:

1. Create an account at [opencoopcontrol.org](https://opencoopcontrol.org) (coming soon)
2. Add your device using the unique device ID displayed during setup
3. Access your coop data from anywhere and set up advanced automations

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/my-new-feature`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin feature/my-new-feature`
5. Submit a pull request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- ESP32 Community
- Arduino Project
- All contributors and chicken enthusiasts!