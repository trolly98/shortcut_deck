#include "../display.hpp"

class I2C096Oled : public Display
{
public:
    I2C096Oled();
    ~I2C096Oled();
    bool init() override;
    void clear() override;
    void print(const String& text) override;
    void show_logo() override;

    void print_configuration(const ButtonArray* config, 
                             ButtonsConfiguration::index_t selected_config,
                             ButtonsConfiguration::index_t config_count) const override;
};