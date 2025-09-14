# Shortcut Deck

Shortcut Deck is an Arduino SDK based project that manages programmable button configurations, simulating keyboard key presses via USB HID. It is designed to create a customizable keypad with configurable key combinations.

## Features

- Manage multiple button configurations
- Simulate single key presses and combinations (e.g., Enter, letters, etc.)
- Serial communication for configuration commands and feedback
- Modular and easy to extend

## Requirements

- USB HID compatible device (I use this site to verify it: https://tttapa.github.io/Control-Surface/Doxygen/d8/d4a/md_pages_MIDI-over-USB.html)
- Arduino libraries:
  - Keyboard
- Optional: 0.96" OLED display

## Tested Boards

- [Waveshare RP2350-Plus Development Board](https://www.waveshare.com/product/rp2350-plus.htm)

## Usage

1. Define button configurations in the code with desired key mappings.
2. Upload the firmware to your board.
3. Connect the board to your PC via USB.
4. The device will act as a HID keyboard, sending configured key presses when buttons are pressed.

## Configuration

Shortcut Deck can be configured using the included **`configurator.html`** tool:  

1. Connect the board to your PC.  
2. Open `configurator.html` in your browser.  
3. Fill in the fields with the desired shortcut configuration.  
4. Click **ADD CONFIG** to send the configuration to the board.  

The board will store the configuration and use it immediately for the programmed buttons.

## License

This project is licensed under the MIT License.
