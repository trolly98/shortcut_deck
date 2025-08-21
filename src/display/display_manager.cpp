#include "display_manager.hpp"

#include "i2c_20x4_lcd/I2C20x4_lcd.hpp"
#include "i2c_096_oled/i2c_096_oled.hpp"

DisplayManager::DisplayManager():
    _display(new I2C096Oled())
{
}

DisplayManager::~DisplayManager()
{
    delete _display;
}

bool DisplayManager::init()
{
    if (_display)
    {
        return _display->init();
    }
    return false;
}

const Display* DisplayManager::display() const
{
    return _display;
}
