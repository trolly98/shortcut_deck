#include "../display.hpp"

class I2C20x4LCD : public Display
{
public:
    I2C20x4LCD();
    ~I2C20x4LCD();
    bool init() override;
    void clear() override;
    void print(const String& text) override;

    void print_configuration(const ButtonArray* config, 
                             ButtonsConfiguration::index_t selected_config,
                             ButtonsConfiguration::index_t config_count) const override;
};