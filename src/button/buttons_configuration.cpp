#include "buttons_configuration.hpp"

ButtonsConfiguration::ButtonsConfiguration() : 
  _cfg_selected{-1},
  _buttons_selected{nullptr},
  _config{nullptr},
  _config_size{0}
{
}

bool ButtonsConfiguration::init()
{
  EEPROM.begin(2200);
  this->_load_data();
  return true;
}

bool ButtonsConfiguration::configuration_available()
{
  return (_config_size > 0);
}

bool ButtonsConfiguration::add_configuration(const function_t function_list[MAX_BTN_NUMBER], 
                                             const String& name)
{
  if(!this->_add_configuration(function_list, name))
  {
    return false;
  }
  if (_config_size > 0)
  {
    if (!this->select_configuration(_config_size-1))
    {
      this->_save_data();
    }
  }
  return true;
}

bool ButtonsConfiguration::update_configuration(index_t index,
                                               const function_t function_list[MAX_BTN_NUMBER], 
                                               const String& name)
{
  if (index < 0 || index >= _config_size)
  {
    Serial.print(F("Configuration index: "));
    Serial.print(index);
    Serial.println(F(" out of range!"));
    return false;
  }

  ButtonArray* btn_array = _config[index];
  if (!btn_array)
  {
    Serial.print(F("Configuration at index: "));
    Serial.print(index);
    Serial.println(F(" is null!"));
    return false;
  }

  btn_array->set_name(name);
  for (int i = 0; i < MAX_BTN_NUMBER; i++)
  {
    if (function_list[i].key != "")
    {
      btn_array->buttons[i]->set_key(function_list[i].key);
    }
    if (function_list[i].name != "")
    {
      btn_array->buttons[i]->set_name(function_list[i].name);
    }
  }

  this->_select_configuration(index);
  this->_save_data();
  return true;
}

bool ButtonsConfiguration::remove_configuration(index_t index)
{
  if(!this->_remove_configuration(index))
  {
    return false;
  }
  this->_save_data();
  return true;
}

bool ButtonsConfiguration::remove_all_configurations()
{
  for (index_t i = _config_size - 1; i >= 0; --i)
  {
    if (!this->_remove_configuration(i))
    {
      Serial.println("Failed to remove configuration at index: " + String(i));
      return false;
    }
  }
  this->_save_data();
  return true;
}

bool ButtonsConfiguration::select_configuration(index_t index)
{
  if(!this->_select_configuration(index))
  {
    return false;
  }
  this->_save_data();
  return true;
}

ButtonArray* ButtonsConfiguration::get_selected_configuration_btn()
{
  if (_buttons_selected == nullptr)
  {
    _buttons_selected = this->get_configuration_btn(_cfg_selected);
  }
  return _buttons_selected;
}

ButtonArray* ButtonsConfiguration::get_configuration_btn(index_t index)
{
  if (index >= _config_size || _config[index] == nullptr)
  {
    //Serial.println("Configuration index: " + String(index) + " not present!");
    return nullptr;
  }
  return _config[index];
}

void ButtonsConfiguration::print_configuration()
{
  Serial.print(F("CONFIGURATION SIZE: "));
  Serial.println(_config_size);

  for (index_t index = 0; index < _config_size; ++index)
  {
    ButtonArray* btnArr = _config[index];
    if (!btnArr) continue;

    Serial.print(F("Config INDEX "));
    Serial.print(index);
    Serial.print(F(" - Name: "));
    Serial.println(btnArr->name());

    for (int b = 0; b < MAX_BTN_NUMBER; b++)
    {
      Serial.print(F("["));
      Serial.print(b);
      Serial.print(F("-"));
      Serial.print(btnArr->buttons[b]->pin());
      Serial.print(F("] "));
      Serial.print(FunctionButton::get_number_string(
                     btnArr->buttons[b]->number()
                   ));
      Serial.print(F("("));
      Serial.print(btnArr->buttons[b]->name());
      Serial.print(F(":"));
      Serial.print(btnArr->buttons[b]->key());
      Serial.print(F(")"));

      if (b < MAX_BTN_NUMBER - 1)
        Serial.print(F(", "));
    }
    Serial.println();
    Serial.println(F("---------------------------"));
  }
}

void ButtonsConfiguration::print_actual_configuration()
{
  Serial.print(F("CURRENT CONFIGURATION INDEX: "));
  Serial.println(_cfg_selected);

  if (_cfg_selected < 0 || _cfg_selected >= _config_size)
  {
    Serial.println(F("---------------------------"));
    return;
  }

  ButtonArray* btnArr = _config[_cfg_selected];
  if (!btnArr) 
  {
    Serial.println(F("---------------------------"));
    return;
  }

  Serial.print(F("Config INDEX "));
  Serial.print(_cfg_selected);
  Serial.print(F(" - Name: "));
  Serial.println(btnArr->name());

  for (int b = 0; b < MAX_BTN_NUMBER; b++)
  {
    Serial.print(F("["));
    Serial.print(b);
    Serial.print(F("-"));
    Serial.print(btnArr->buttons[b]->pin());
    Serial.print(F("] "));
    Serial.print(FunctionButton::get_number_string(
                   btnArr->buttons[b]->number()
                 ));
    Serial.print(F("("));
    Serial.print(btnArr->buttons[b]->name());
    Serial.print(F(":"));
    Serial.print(btnArr->buttons[b]->key());
    Serial.print(F(")"));

    if (b < MAX_BTN_NUMBER - 1)
      Serial.print(F(", "));
  }
  Serial.println();
  Serial.println(F("---------------------------"));
}

const ButtonsConfiguration::index_t ButtonsConfiguration::current_index() const
{
  return _cfg_selected;
}

const ButtonsConfiguration::index_t ButtonsConfiguration::config_count() const
{
  return _config_size;
}

void ButtonsConfiguration::_save_data()
{
    constexpr size_t BUF_SIZE = 512; // adatta alla tua EEPROM
    char buffer[BUF_SIZE];
    size_t pos = 0;

    for (index_t index = 0; index < _config_size; ++index)
    {
        ButtonArray* btnArr = _config[index];
        if (!btnArr) continue;

        // Nome config
        int n = snprintf(buffer + pos, BUF_SIZE - pos, "%s:", btnArr->name().c_str());
        if (n < 0 || (pos + n) >= BUF_SIZE) break;
        pos += n;

        for (int b = 0; b < MAX_BTN_NUMBER; b++)
        {
            FunctionButton* btn = btnArr->buttons[b];
            if (!btn) continue;

            n = snprintf(buffer + pos, BUF_SIZE - pos, "%s-%s",
                         btn->name().c_str(), btn->key().c_str());
            if (n < 0 || (pos + n) >= BUF_SIZE) break;
            pos += n;

            if (b < MAX_BTN_NUMBER - 1)
            {
                if (pos < BUF_SIZE - 1) buffer[pos++] = ',';
                else break;
            }
        }

        if (pos < BUF_SIZE - 1) buffer[pos++] = '\n';
        else break;
    }

    // Terminatore configurazioni
    if (pos < BUF_SIZE - 1) buffer[pos++] = '#';

    // Config selezionata
    int n = snprintf(buffer + pos, BUF_SIZE - pos, "%d", _cfg_selected);
    if (n > 0 && (pos + n) < BUF_SIZE) pos += n;

    // Terminatore stringa
    buffer[pos++] = '\0';

    // Scrivi in EEPROM
    size_t addr = 0;
    for (size_t i = 0; i < pos && addr < EEPROM.length(); i++)
    {
        EEPROM.update(addr++, buffer[i]);
    }
    EEPROM.commit();

    Serial.println(F("Configuration saved!"));
}

void ButtonsConfiguration::_load_data()
{
    constexpr size_t BUF_SIZE = 512;
    char buffer[BUF_SIZE];

    // Leggi EEPROM in buffer
    size_t addr = 0;
    size_t pos = 0;
    while (addr < EEPROM.length() && pos < BUF_SIZE - 1)
    {
        char ch = EEPROM.read(addr++);
        if (ch == '\0' || ch == (char)0xFF) break;
        buffer[pos++] = ch;
    }
    buffer[pos] = '\0';

    // Trova separatore config/selected
    char* hash = strchr(buffer, '#');
    if (!hash)
    {
        Serial.println(F("No saved configuration found."));
        return;
    }

    *hash = '\0'; // separa le due parti
    int selected = atoi(hash + 1);

    Serial.print(F("Read Matrixdata:\n"));
    Serial.println(buffer);
    Serial.print(F("Selected: "));
    Serial.println(selected);

    // Parsing riga per riga manuale
    char* lineStart = buffer;
    while (lineStart && *lineStart)
    {
        char* lineEnd = strchr(lineStart, '\n');
        if (lineEnd) *lineEnd = '\0';

        // Ogni riga: ConfigName:btnName-key,btnName-key,...
        char* colon = strchr(lineStart, ':');
        if (!colon) {
            lineStart = lineEnd ? lineEnd + 1 : nullptr;
            continue;
        }

        *colon = '\0';
        const char* cfgName = lineStart;
        const char* rest = colon + 1;

        function_t functions[MAX_BTN_NUMBER];
        for (int i = 0; i < MAX_BTN_NUMBER; i++) functions[i] = {"", ""};
        int cellIndex = 0;

        const char* tokenStart = rest;
        while (tokenStart && *tokenStart && cellIndex < MAX_BTN_NUMBER)
        {
            const char* comma = strchr(tokenStart, ',');
            size_t len = comma ? (size_t)(comma - tokenStart) : strlen(tokenStart);
            char tmp[64];
            strncpy(tmp, tokenStart, len);
            tmp[len] = '\0';

            char* dash = strchr(tmp, '-');
            if (dash) {
                *dash = '\0';
                functions[cellIndex].name = tmp;
                functions[cellIndex].key  = dash + 1;
            } else {
                functions[cellIndex].name = "";
                functions[cellIndex].key  = tmp;
            }

            cellIndex++;
            tokenStart = comma ? comma + 1 : nullptr;
        }

        if (!this->_add_configuration(functions, cfgName))
        {
            Serial.println(F("Add configuration FAILED!"));
        }

        lineStart = lineEnd ? lineEnd + 1 : nullptr;
    }

    this->_select_configuration(selected);
}


bool ButtonsConfiguration::_add_configuration(const function_t function_list[MAX_BTN_NUMBER], 
                                              const String& name)
{
  if (_config_size >= MAX_CONFIGURATION_NUMBER)
  {
    Serial.print(F("Maximum configurations reached: "));
    Serial.println(MAX_CONFIGURATION_NUMBER);
    return false;
  }
  ButtonArray* arr = new ButtonArray(name);
  for (int i = 0; i < MAX_BTN_NUMBER; i++) 
  {
    const FunctionButton::Number number = FunctionButton::get_number(i);
    arr->buttons[i] = new FunctionButton(number, function_list[i].key, function_list[i].name);
  }
  _config[_config_size++] = arr;
  Serial.print(F("Add configuration done, config size: "));
  Serial.println(_config_size);
  return true;
}

bool ButtonsConfiguration::_select_configuration(index_t index)
{
  if (index >= _config_size)
  {
    Serial.print(F("Configuration index: "));
    Serial.print(index);
    Serial.println(F(" out of range!"));
    return false;
  }
  if (_cfg_selected == index)
  {
    Serial.print(F("Selected same configuration with index: "));
    Serial.println(index);
    return false;
  }

  _cfg_selected = index;
  _buttons_selected = this->get_configuration_btn(index);
  //Serial.println("Configuration index: " + String(index) + " selected!");
  return true;
}

bool ButtonsConfiguration::_remove_configuration(index_t index)
{
  if (_config_size < 1)
  {
    Serial.println("No configuration saved!");
    return false;
  }
  if (index < _config_size && _config[index] != nullptr) 
  {
    ButtonArray* arr = _config[index];

    for (int i = 0; i < MAX_BTN_NUMBER; i++)
    {
      delete arr->buttons[i];
      arr->buttons[i] = nullptr;
    }

    delete arr;
    _config[index] = nullptr;
    
    // Shift left per riempire il buco e mantenere array compatto
    for (int i = index; i < _config_size - 1; i++)
    {
        _config[i] = _config[i + 1];
    }
    _config[_config_size - 1] = nullptr;
    _config_size--;
    Serial.println("Remove configuration INDEX: " + String(index) + " done!");

    if (_cfg_selected >= index)
    {
      this->_select_configuration(_cfg_selected - 1);
    }
    return true;
  }

  Serial.println("Remove configuration INDEX: " + String(index) + " not found!");
  return false;
}