Sure! Here’s your README in English with the serial commands section added:

````markdown
# Shortcut Deck

Shortcut Deck is an Arduino project that manages programmable button configurations, simulating keyboard key presses via USB HID. It is designed to create a customizable keypad with configurable key combinations.

## Features

- Manage multiple button configurations
- Simulate single key presses and combinations (e.g., Enter, letters, etc.)
- Serial communication for configuration commands and feedback
- Modular and easy to extend

## Requirements

- Arduino Micro or any USB HID compatible device
- Arduino libraries:
  - Keyboard

## Usage

1. Define button configurations in the code with desired key mappings.
2. Upload the firmware to your Arduino Micro.
3. Connect the Arduino to your PC via USB.
4. The device will act as a HID keyboard, sending configured key presses when buttons are pressed.

## Serial Commands

You can control the device by sending commands through the serial port (e.g., `/dev/ttyACM0`):

- **Switch configuration:**
  ```bash
  echo "SWITCH_CFG=2" > /dev/ttyACM0
````

* **Add configuration:**

  ```bash
  echo 'ADD_CFG={"btn_1":"c", "btn_2":"h"}' > /dev/ttyACM0
  ```

* **Remove configuration:**

  ```bash
  echo "RM_CFG=2" > /dev/ttyACM0
  ```

* **Show entire configuration:**

  ```bash
  echo "SHOW_CFG" > /dev/ttyACM0
  ```


## License

This project is licensed under the MIT License.
