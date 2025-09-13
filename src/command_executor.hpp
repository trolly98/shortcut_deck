#pragma once

#include "serial_line_parser.hpp"
#include "globals.hpp"
#include "utils.hpp"

bool exec_cmd(const CommandResult &cmd_result)
{
  switch (cmd_result.type) 
  {
    case CommandType::SWITCH_CFG:
    {
      const ButtonsConfiguration::index_t index 
        = static_cast<ButtonsConfiguration::index_t>(cmd_result.payload.toInt());
      global_buttons_configuration.select_configuration(index-1);
      return true;
    }

    case CommandType::ADD_CFG:
    {
      function_t functions[MAX_BTN_NUMBER];
      String function_name = extract_function_name(cmd_result.payload);
      extract_functions(cmd_result.payload, functions);
      global_buttons_configuration.add_configuration(functions, function_name);
      return true;
    }

    case CommandType::UPDATE_CFG:
    {
      function_t functions[MAX_BTN_NUMBER];
      int configuration_index = extract_function_index(cmd_result.payload);
      if (configuration_index < 0)
      {
        Serial.println(F("Invalid configuration index: -1!"));
        return false;
      }
      String function_name = extract_function_name(cmd_result.payload);
      extract_functions(cmd_result.payload, functions);
      global_buttons_configuration.update_configuration(configuration_index-1, functions, function_name);

      update_display();
      return true;
    }

    case CommandType::RM_CFG:
    {
      const ButtonsConfiguration::index_t index 
        = static_cast<ButtonsConfiguration::index_t>(cmd_result.payload.toInt());
      if (index <= 0)
      {
        Serial.println(F("Invalid configuration index!"));
        return false;
      }
      global_buttons_configuration.remove_configuration(index-1);
      update_display();
      return true;
    }

    case CommandType::RM_ALL_CFG:
    {
      global_buttons_configuration.remove_all_configurations();
      return true;
    }
    
    case CommandType::SHOW_CFG:
    {
      global_buttons_configuration.print_configuration();
      return true;
    }

    case CommandType::SHOW_ACTUAL_CFG:
    {
      global_buttons_configuration.print_actual_configuration();
      return true;
    }

    case CommandType::SHOW_ACTUAL_CFG_INDEX:
    {
      Serial.print(F("Current configuration index: "));
      Serial.print(global_buttons_configuration.current_index());
      Serial.println(F(";"));
      return true;
    }

    case CommandType::SHOW_CFG_INDEXES:
    {
      Serial.print(F("Available configuration indexes: "));
      Serial.print(global_buttons_configuration.config_count());
      Serial.println(F(";"));
      return true;
    }
    
    default:
      Serial.println(F("Unknown command!"));
      return false;
  }
}