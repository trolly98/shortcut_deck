#pragma once

#include "serial_line_parser.hpp"
#include "globals.hpp"

bool exec_cmd(const CommandResult &cmd_result)
{
  switch (cmd_result.type) 
  {
    case CommandType::SWITCH_CFG:
    {
      const ButtonsConfiguration::index_t index 
        = static_cast<ButtonsConfiguration::index_t>(cmd_result.payload.toInt());
      global_buttons_configuration.select_configuration(index);
      return true;
    }

    case CommandType::ADD_CFG:
    {
      String functions[MAX_BTN_NUMBER];
      //char functions[MAX_CONFIGURATION_NUMBER][MAX_CONFIGURATION_NUMBER];
      parse_json(cmd_result.payload, functions);
      global_buttons_configuration.add_configuration(functions);
      return true;
    }

    case CommandType::RM_CFG:
    {
      const ButtonsConfiguration::index_t index 
        = static_cast<ButtonsConfiguration::index_t>(cmd_result.payload.toInt());
      global_buttons_configuration.remove_configuration(index);
      return true;
    }
    
    case CommandType::SHOW_CFG:
    {
      global_buttons_configuration.print_configuration();
      return true;
    }
    
    case CommandType::SHOW_ACTUAL_CFG:
    {
      Serial.println("Current cfg index: " + String(global_buttons_configuration.current_index()));
      return true;
    }
    
    default:
      Serial.println(F("Unknown command!"));
      return false;
  }
}