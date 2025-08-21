#include "i2c_096_oled.hpp"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1   // reset non collegato
#define SCREEN_ADDRESS 0x3C // tipico per gli OLED 0.96"

#define SDA_PIN 4
#define SCL_PIN 5

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

I2C096Oled::I2C096Oled()
{
}

I2C096Oled::~I2C096Oled()
{
}

bool I2C096Oled::init()
{
    Wire.setSDA(SDA_PIN);
    Wire.setSCL(SCL_PIN);
    Wire.begin();
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) 
    {
        Serial.println("SSD1306 allocation failed");
        return false;
    }

    display.clearDisplay();
    display.setTextSize(1);      // dimensione testo
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(F("Hello, World!"));
    display.display();           // aggiorna lo schermo

    this->show_logo();
    delay(2000);
    return true;
}

void I2C096Oled::clear()
{
  display.clearDisplay();
  display.display();
}

void I2C096Oled::print(const String& text)
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(text);
  display.display();
}

void I2C096Oled::show_logo()
{
    display.clearDisplay();
    display.drawBitmap(
        (SCREEN_WIDTH - LOGO_WIDTH) / 2,
        (SCREEN_HEIGHT - LOGO_HEIGHT) / 2,
        epd_bitmap_logo_little, LOGO_WIDTH, LOGO_HEIGHT, SSD1306_WHITE
    );
    display.display();
}

void I2C096Oled::print_configuration(const ButtonArray* config, 
                                     ButtonsConfiguration::index_t selected_config,
                                     ButtonsConfiguration::index_t config_count) const
{
    if (!config) return;

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);

    display.setTextSize(1);  // caratteri 6x8 px
const int footerH = 8;             
const int usableH = SCREEN_HEIGHT - footerH; 
const int usableW = SCREEN_WIDTH;      // usiamo tutta la larghezza
const int cellCols = 3;                
const int cellRows = 3;                
const int cellW = usableW / cellCols;  
const int cellH = usableH / cellRows;  
const int topMargin = 1;               // margine minimo dall'alto

for (int row = 0; row < cellRows; row++) {
    for (int col = 0; col < cellCols; col++) {
        int idx = row * cellCols + col;
        if (idx >= MAX_BTN_NUMBER) break;

        String val = config->buttons[idx]->key();
        if (val == "") val = "-";

        // suddividiamo il testo in massimo 3 righe da 5 caratteri ciascuna
        String lines[3] = {"", "", ""};
        int lineCount = 0;
        while (val.length() > 0 && lineCount < 3) {
            lines[lineCount] = val.substring(0, 5); 
            val = val.substring(5);
            lineCount++;
        }

        // offset verticale per centrare il testo nella cella
        int totalTextHeight = lineCount * 8; // 8 px per riga
        int yStart = row * cellH + (cellH - totalTextHeight)/2 + topMargin;

        for (int l = 0; l < lineCount; l++) {
            int lineLen = lines[l].length();
            int lineWidth = lineLen * 6; // 6 px per carattere
            int x = col * cellW + (cellW - lineWidth)/2; // centrato orizzontalmente
            int y = yStart + l * 8;            
            display.setCursor(x, y);
            display.print(lines[l]);
        }
    }
}

// Footer sulla riga finale
display.setTextSize(1);  
display.setCursor(0, SCREEN_HEIGHT - footerH);
display.print(F("Cfg "));
display.print(selected_config);
display.print("/");
display.print(config_count);

display.display();
}


