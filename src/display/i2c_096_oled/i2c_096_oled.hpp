#include "../display.hpp"

class I2C096Oled : public Display
{
public:
    I2C096Oled();
    ~I2C096Oled();
    bool init() const override;
    void clear() const override;
    void print(const String& text) const override;
    void show_logo() const override;
    void show_info() const override;

    void print_configuration(const ButtonArray* config, 
                             ButtonsConfiguration::index_t selected_config,
                             ButtonsConfiguration::index_t config_count) const override;
private:
    void _print_footer(const ButtonArray* config, 
                       ButtonsConfiguration::index_t selected_config,
                       ButtonsConfiguration::index_t config_count) const;

    void _print_config_info(const ButtonArray* config, 
                            ButtonsConfiguration::index_t selected_config,
                            ButtonsConfiguration::index_t config_count) const;
};