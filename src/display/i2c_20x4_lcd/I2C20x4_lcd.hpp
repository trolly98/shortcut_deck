#include "../display.hpp"

class I2C20x4LCD : public Display
{
public:
    I2C20x4LCD();
    ~I2C20x4LCD();
    bool init() const override;
    void clear() const override;
    void print(const String& text) const override;
    void show_logo() const override;
    void show_info() const override;

    void print_configuration(const ButtonArray* config, 
                             ButtonsConfiguration::index_t selected_config,
                             ButtonsConfiguration::index_t config_count) const override;
};