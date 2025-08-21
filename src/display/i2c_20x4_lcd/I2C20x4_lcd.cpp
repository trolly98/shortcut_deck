#include "I2C20x4_lcd.hpp"
#include <LiquidCrystal_I2C.h>

#define SDA_PIN 4
#define SCL_PIN 5

LiquidCrystal_I2C lcd(0x27, 20, 4);

I2C20x4LCD::I2C20x4LCD()
{
}

I2C20x4LCD::~I2C20x4LCD()
{
}

bool I2C20x4LCD::init()
{
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Hello, World!");
    return true;
}

void I2C20x4LCD::clear()
{
  lcd.clear();
}

void I2C20x4LCD::print(const String& text)
{
  lcd.print(text);
}

void I2C20x4LCD::show_logo()
{
    // Implement the logic to display the logo on the LCD
}

void I2C20x4LCD::print_configuration(const ButtonArray* config, 
                                     ButtonsConfiguration::index_t selected_config,
                                     ButtonsConfiguration::index_t config_count) const
{
    if (!config) return;

    lcd.clear();

    // Stampiamo la matrice 3x3
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            int idx = row * 3 + col;
            if (idx >= MAX_BTN_NUMBER) break;

            lcd.setCursor(col * 6, row);  // 6 spazi per colonna, sufficiente per valori tipo "BTN1"
            String val = config->buttons[idx]->key();
            if (val == "")
            {
                val = "-";
            }
            // Assicuriamoci che occupi 5 caratteri max per avere allineamento
            if (val.length() > 5) val = val.substring(0,5);
            lcd.print(val);
        }
    }

    // Riga inferiore per stato configurazione
    lcd.setCursor(0,3);
    lcd.print("Config: ");
    lcd.print(selected_config);
    lcd.print("/");
    lcd.print(config_count);
}


