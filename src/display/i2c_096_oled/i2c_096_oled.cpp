#include "i2c_096_oled.hpp"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#include "../../info.hpp"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define FOOTER_HEIGHT 8
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

bool I2C096Oled::init() const
{
    Serial.println("Initializing I2C 0.96\" OLED display...");
    Serial.print("SDA pin: "); Serial.println(SDA_PIN);
    Serial.print("SCL pin: "); Serial.println(SCL_PIN);
    Wire.setSDA(SDA_PIN);
    Wire.setSCL(SCL_PIN);
    Serial.println("Starting I2C...");
    Serial.println("Calling Wire.begin()...");
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

void I2C096Oled::clear() const
{
  display.clearDisplay();
  display.display();
}

void I2C096Oled::print(const String& text) const
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(text);
  display.display();
}

void I2C096Oled::show_logo() const
{
    display.clearDisplay();
    display.drawBitmap(
        (SCREEN_WIDTH - LOGO_WIDTH) / 2,
        (SCREEN_HEIGHT - LOGO_HEIGHT) / 2,
        epd_bitmap_logo_little, LOGO_WIDTH, LOGO_HEIGHT, SSD1306_WHITE
    );
    display.display();
}

void I2C096Oled::show_info() const
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    // Titolo centrato
    const char* title = PROJECT;
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(title, 0, 0, &x1, &y1, &w, &h);
    display.setCursor((SCREEN_WIDTH - w) / 2, 0);
    display.println(title);

    // Versione + Maintainer
    display.setCursor(0, 12);
    display.print(F("Ver: "));
    display.println(VERSION);
    display.setCursor(0, 22);
    display.print(F("By : "));
    display.println(MAINTAINER);

    // Uptime
    unsigned long uptime_s = millis() / 1000;
    unsigned long sec = uptime_s % 60;
    unsigned long min = (uptime_s / 60) % 60;
    unsigned long hr  = uptime_s / 3600;
    char uptime_buf[16];
    sprintf(uptime_buf, "%02lu:%02lu:%02lu", hr, min, sec);
    display.setCursor(0, 34);
    display.print(F("Up : "));
    display.println(uptime_buf);

    // Footer: GitHub url croppato se troppo lungo
    String url = GITHUB_URL;
    int maxCharsPerLine = SCREEN_WIDTH / 6;
    String line1 = url.substring(0, (url.length() < maxCharsPerLine) ? url.length() : maxCharsPerLine);
    String line2;
    if (url.length() > maxCharsPerLine) {
        int endIndex = (url.length() < 2 * maxCharsPerLine) ? url.length() : 2 * maxCharsPerLine;
        line2 = url.substring(maxCharsPerLine, endIndex);
        if (url.length() > 2 * maxCharsPerLine) {
            line2 = line2.substring(0, line2.length() - 2) + "..";
        }
    }
    display.setCursor(0, SCREEN_HEIGHT - 2 * FOOTER_HEIGHT);
    display.print(line1);
    display.setCursor(0, SCREEN_HEIGHT - FOOTER_HEIGHT);
    display.print(line2);

    display.display();
}

void I2C096Oled::print_configuration(const ButtonArray* config, 
                                     ButtonsConfiguration::index_t selected_config,
                                     ButtonsConfiguration::index_t config_count) const
{
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);

    //this->_print_footer(config, selected_config, config_count);
    
    this->_print_config_info(config, selected_config, config_count);
    delay(1000);
    display.clearDisplay();

    if (!config) 
    {
        display.setTextSize(2);
        String msg = "No config";
        int16_t x1, y1;
        uint16_t w, h;
        display.getTextBounds(msg, 0, 0, &x1, &y1, &w, &h);
        int x = (SCREEN_WIDTH - w) / 2;
        int y = (SCREEN_HEIGHT - FOOTER_HEIGHT - h) / 2;
        display.setCursor(x, y);
        display.print(msg);
        display.display();
        return;
    }

    // display.setTextSize(1);  // caratteri 6x8 px          
    // const int usableH = SCREEN_HEIGHT; // - FOOTER_HEIGHT; 
    // const int usableW = SCREEN_WIDTH;      // usiamo tutta la larghezza
    // const int cellCols = 3;                
    // const int cellRows = 3;                
    // const int cellW = usableW / cellCols;  
    // const int cellH = usableH / cellRows;  
    // const int topMargin = 1;               // margine minimo dall'alto

    // for (int row = 0; row < cellRows; row++) {
    //     for (int col = 0; col < cellCols; col++) {
    //         int idx = row * cellCols + col;
    //         if (idx >= MAX_BTN_NUMBER) break;

    //         String val = config->buttons[idx]->key();
    //         if (val == "") val = "-";

    //         // suddividiamo il testo in massimo 3 righe da 5 caratteri ciascuna
    //         String lines[3] = {"", "", ""};
    //         int lineCount = 0;
    //         while (val.length() > 0 && lineCount < 3) {
    //             lines[lineCount] = val.substring(0, 5); 
    //             val = val.substring(5);
    //             lineCount++;
    //         }

    //         // offset verticale per centrare il testo nella cella
    //         int totalTextHeight = lineCount * 8; // 8 px per riga
    //         int yStart = row * cellH + (cellH - totalTextHeight)/2 + topMargin;

    //         for (int l = 0; l < lineCount; l++) {
    //             int lineLen = lines[l].length();
    //             int lineWidth = lineLen * 6; // 6 px per carattere
    //             int x = col * cellW + (cellW - lineWidth)/2; // centrato orizzontalmente
    //             int y = yStart + l * 8;            
    //             display.setCursor(x, y);
    //             display.print(lines[l]);
    //         }
    //     }
    // }

    // display.display();
    display.setTextSize(1);  // caratteri 6x8 px          
    const int usableH = SCREEN_HEIGHT; // usiamo tutta l'altezza
    const int usableW = SCREEN_WIDTH;  // usiamo tutta la larghezza
    const int cellCols = 3;                
    const int cellRows = 3;                
    const int cellW = usableW / cellCols;  
    const int cellH = usableH / cellRows;  
    const int topMargin = 1;               // margine minimo dall'alto

    // --- Disegno le celle con testo centrato ---
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

    // --- Disegno la griglia minimale: 1px tra le celle ---
    for (int col = 1; col < cellCols; col++) {
        int x = col * cellW;
        display.drawFastVLine(x, 0, usableH, SSD1306_WHITE);
    }
    for (int row = 1; row < cellRows; row++) {
        int y = row * cellH;
        display.drawFastHLine(0, y, usableW, SSD1306_WHITE);
    }

    display.display();
}


void I2C096Oled::_print_footer(const ButtonArray* config, 
                               ButtonsConfiguration::index_t selected_config,
                               ButtonsConfiguration::index_t config_count) const
{
    String cfg_selected = String(selected_config < 0 ? 0 : selected_config) + 
                          "/" + 
                          String(config_count < 0 ? 0 : config_count);
    String cfg_name = (config ? config->name() : "--");

    display.setTextSize(1);
    int charWidth = 6; // font base a textSize=1
    int maxChars = (SCREEN_WIDTH / charWidth) - cfg_selected.length() - 1;

    if (cfg_name.length() >= maxChars) 
    {
        cfg_name = cfg_name.substring(0, maxChars);
    }

    int spacer_count = (SCREEN_WIDTH - (cfg_selected.length() * charWidth + cfg_name.length() * charWidth)) / charWidth;
    String spacer = "";
    for (int i = 0; i < spacer_count; i++) 
    {
        spacer += ' ';
    }

    String footer = cfg_selected + spacer + cfg_name;
    display.setTextSize(1);  
    display.setCursor(0, SCREEN_HEIGHT - FOOTER_HEIGHT);
    display.print(footer);
    display.display();
}

void I2C096Oled::_print_config_info(const ButtonArray* config, 
                        ButtonsConfiguration::index_t selected_config,
                        ButtonsConfiguration::index_t config_count) const
{
    display.setTextSize(2);  // un po' più grande
    display.setTextColor(SSD1306_WHITE);

    // Preparazione delle stringhe
    String cfg_selected = String(selected_config < 0 ? 0 : selected_config) +
                        "/" +
                        String(config_count < 0 ? 0 : config_count);
    String cfg_name = (config ? config->name() : "--");

    // Spazio disponibile in pixel
    int screenW = SCREEN_WIDTH;

    // Calcolo posizione centrale della prima riga (cfg_selected)
    int selWidth = cfg_selected.length() * 12; // 12px per char con setTextSize(2)
    int xSel = max(0, (screenW - selWidth) / 2);
    int ySel = SCREEN_HEIGHT / 2 - 16;  // sopra il centro

    display.setCursor(xSel, ySel);
    display.print(cfg_selected);

    // Gestione cfg_name su una o due righe
    int maxCharsPerLine = screenW / 12; // circa 12px per char con setTextSize(2)
    String line1 = cfg_name.substring(0, min(maxCharsPerLine, cfg_name.length()));
    String line2 = "";

    if (cfg_name.length() > maxCharsPerLine) {
        line2 = cfg_name.substring(maxCharsPerLine, min(maxCharsPerLine * 2, cfg_name.length()));
    }

    int yName = SCREEN_HEIGHT / 2;  // subito sotto cfg_selected

    // Centrare line1
    int line1Width = line1.length() * 12;
    int xLine1 = max(0, (screenW - line1Width) / 2);
    display.setCursor(xLine1, yName);
    display.print(line1);

    // Centrare line2 se esiste
    if (line2.length() > 0) {
        int line2Width = line2.length() * 12;
        int xLine2 = max(0, (screenW - line2Width) / 2);
        display.setCursor(xLine2, yName + 16);  // salto riga pari all'altezza carattere
        display.print(line2);
    }

    display.display();

}