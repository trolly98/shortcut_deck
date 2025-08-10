#pragma once

#include "button.hpp"
#include <Vector.h>
#include <EEPROM.h>

using function_t = String;

struct ButtonArray {
  FunctionButton buttons[MAX_BTN_NUMBER];
};

class ButtonsConfiguration
{
public:
  using index_t = uint32_t;
  ButtonsConfiguration() : 
  _cfg_selected{0},
  _buttons_selected{nullptr} 
  {
    _config.setStorage(_default_storage_array);
  }

  bool init()
  {
    this->loadData();
  }

  bool configuration_available()
  {
    return (_config.size() > 0);
  }

  bool add_configuration(const function_t function_list[MAX_BTN_NUMBER])
  {
    if(!this->_add_configuration(function_list))
    {
      return false;
    }
    this->saveData();
    return true;
  }

  bool remove_configuration(index_t index)
  {
    if(!this->_remove_configuration(index))
    {
      return false;
    }
    this->saveData();
    return true;
  }

  bool select_configuration(index_t index)
  {
    if(!this->_select_configuration(index))
    {
      return false;
    }
    this->saveData();
    return true;
  }

  ButtonArray* get_selected_configuration_btn()
  {
    if (_buttons_selected == nullptr)
    {
      _buttons_selected = this->get_configuration_btn(_cfg_selected);
    }
    return _buttons_selected;
  }

  ButtonArray* get_configuration_btn(index_t index)
  {
    if (static_cast<unsigned int>(_config.size()) <= index)
    {
      Serial.println("Configuration index: " + String(index) + " not present!");
      return nullptr;
    }
    return _config.at(index);
  }

  void print_configuration()
  {
    Serial.print(F("CONFIGURATION: "));
    index_t index { 0 };
    for (ButtonArray* btnArr : _config)
    {
      Serial.print(F("Config INDEX "));
      Serial.print(index);
      Serial.println(F(":"));
      Serial.print(F("  Buttons: "));
      for (int b = 0; b < MAX_BTN_NUMBER; b++)
      {
        Serial.print(F("["));
        Serial.print(b);
        Serial.print(F("] "));
        Serial.print( FunctionButton::get_number_string(btnArr->buttons[b].number()));
        Serial.print(F("("));
        Serial.print(btnArr->buttons[b].key());
        Serial.print(F(")"));

        if (b < 7) Serial.print(F(", "));
      }
      Serial.println();
      Serial.println(F("---------------------------"));
      ++index;
    }
  }

  const index_t current_index() const
  {
    return _cfg_selected;
  }


  void saveData()
  {
    int addr = 0;
    String matrixStr = "";

    for (ButtonArray* btnArr : _config)
    {
        for (int b = 0; b < MAX_BTN_NUMBER; b++)
        {
            matrixStr += btnArr->buttons[b].key();
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
  }

  void loadData() 
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
  
    // Serial.println("Read Matrixdata: " + String(matrixData));
    // Serial.println("Selected: " + String(selected));

    String function[8];   // array per le celle della riga
    int cellIndex = 0;    // indice cella corrente
    String cell = "";

    for (int pos = 0; pos < matrixData.length(); pos++) 
    {
        char ch = matrixData[pos];

        if (ch == ',' || ch == '\n') 
        {
            // Salva la cella in function, se c'è spazio
            if (cellIndex < 8) 
            {
                function[cellIndex++] = cell;
            }
            cell = "";

            if (ch == '\n') 
            {
                // Fine riga → stampa function
                // Serial.print("Riga: ");
                // for (int i = 0; i < cellIndex; i++) {
                //     Serial.print("[");
                //     Serial.print(function[i]);
                //     Serial.print("]");
                //     if (i < cellIndex - 1) Serial.print(", ");
                // }
                // Serial.println();

                if(!this->_add_configuration(function))
                {
                  Serial.println("Add configuration FAILED!");
                }
                
                // Reset per la nuova riga
                cellIndex = 0;
                for (int i = 0; i < 8; i++) function[i] = "";
            }
        } 
        else {
            cell += ch;  // accumulo carattere
        }
    }

    this->_select_configuration(selected);

  }

private:
  bool _add_configuration(const function_t function_list[MAX_BTN_NUMBER])
  {
    ButtonArray *arr = new ButtonArray();
    for (int i = 0; i < MAX_BTN_NUMBER; i++) 
    {
      const FunctionButton::Number number = FunctionButton::get_number(i);
      arr->buttons[i] = {number, function_list[i]};
    }
    _config.push_back(arr);
    Serial.println("Add configuration done, config size: " + String(_config.size()));
    return true;
  }

  bool _select_configuration(index_t index)
  {
    if (_cfg_selected == index)
    {
      Serial.println("Selected same configuration with index: " + String(index));
      return;
    }

    const auto btn_selected = this->get_configuration_btn(index);
    if (!btn_selected)
    {
      return false;
    }
    _cfg_selected = index;
    _buttons_selected = btn_selected;
    Serial.println("Configuration index: " + String(index) + " selected!");
    return true;
  }

  bool _remove_configuration(index_t index)
  {
    if (_config.size() < 1)
    {
      Serial.println("Could not remove all configurations!");
      return;
    }
    if (index < _config.size() && _config[index] != nullptr) 
    {
      delete _config[index];
      _config.remove(index);
      Serial.println("Remove configuration INDEX: " + String(index) + " done!");
      return true;
    }
    if (_cfg_selected >= index)
    {
      this->_select_configuration(0);
    }
    return false;
  }

  Vector<ButtonArray*> _config;
  ButtonArray* _default_storage_array[256];
  ButtonArray* _buttons_selected;
  index_t _cfg_selected;
};