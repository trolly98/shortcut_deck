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
    this->select_configuration(_config_size-1);
  }
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
  int addr = 0;
  String matrixStr = "";

  for (index_t index = 0; index < _config_size; ++index)
  {
      ButtonArray* btnArr = _config[index];
      if (btnArr == nullptr)
      {
        continue;
      }
      matrixStr += btnArr->name() + ':';
      for (int b = 0; b < MAX_BTN_NUMBER; b++)
      {
          matrixStr += btnArr->buttons[b]->key();
          if (b < MAX_BTN_NUMBER - 1)
          {
              matrixStr += ',';
          }
      }
      matrixStr += '\n';
  }
  for (size_t i = 0; i < matrixStr.length(); i++) 
  {
      EEPROM.update(addr++, matrixStr[i]);
  }
  EEPROM.update(addr++, '#');
  String selStr = String(_cfg_selected);
  for (size_t i = 0; i < selStr.length(); i++) 
  {
      EEPROM.update(addr++, selStr[i]);
  }
  EEPROM.update(addr++, '\0');
  EEPROM.commit();
  Serial.println(F("Configuration saved!"));
}

void ButtonsConfiguration::_load_data()
{
  String matrixData = "";
  int addr = 0;
  bool readingSelected = false;
  int selected = 0;
  String selStr = "";

  while (true) 
  {
    char ch = EEPROM.read(addr++);
    if (ch == '\0' || ch == (char)0xFF) 
    {
        if (readingSelected) 
        {
          selected = selStr.toInt();
        }
        break;
    }

    if (readingSelected) 
    {
        selStr += ch;
        continue;
    }

    if (ch == '#') 
    {
        readingSelected = true;
        continue;
    }

    matrixData += ch;
  }

  Serial.println("Read Matrixdata: " + String(matrixData));
  Serial.println("Selected: " + String(selected));

  String function[MAX_BTN_NUMBER];   // array per le celle della riga
  int cellIndex = 0;    // indice cella corrente
  String cell = "";
  String function_name = "--";

  for (int pos = 0; pos < matrixData.length(); pos++) 
  {
      char ch = matrixData[pos];
      if (ch == ':')
      {
          function_name = cell;
          cell = "";
      }
      else if (ch == ',' || ch == '\n') 
      {
          // Salva la cella in function, se c'è spazio
          if (cellIndex < MAX_BTN_NUMBER)
          {
              function[cellIndex++] = cell;
          }

          cell = "";

          if (ch == '\n') 
          {
              // Fine riga → stampa function
              //Serial.print("Riga: ");
              // for (int i = 0; i < cellIndex; i++) {
              //     //Serial.print("[");
              //     //Serial.print(function[i]);
              //     //Serial.print("]");
              //     if (i < cellIndex - 1) {
              //       //Serial.print(", ");
              //     }
              // }
              //Serial.println();

              if(!this->_add_configuration(function, function_name))
              {
                Serial.println(F("Add configuration FAILED!"));
              }

              // Reset per la nuova riga
              cellIndex = 0;
              for (int i = 0; i < MAX_BTN_NUMBER; i++) function[i] = "";
          }
      } 
      else 
      {
          cell += ch;  // accumulo carattere
      }
  }

  Serial.print(F("Selected: "));
  Serial.println(String(selected));
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
    arr->buttons[i] = new FunctionButton(number, function_list[i]);
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