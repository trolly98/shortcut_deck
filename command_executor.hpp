#pragma once

#include "serial_line_parser.hpp"
#include "globals.hpp"

bool exec_cmd(const CommandResult &cmd_result)
{
  switch (cmd_result.type) 
  {
    case CommandType::SWITCH_CFG:
    {
      Serial.print(F("Switch configuration to ID: "));
      Serial.println(cmd_result.payload);
      const ButtonsConfiguration::configuration_id_t id 
      = static_cast<ButtonsConfiguration::configuration_id_t>(cmd_result.payload.toInt());
      global_buttons_configuration.select_configuration(id);
      return true;
    }

    case CommandType::ADD_CFG:
      Serial.print(F("Add configurazione: "));
      Serial.println(cmd_result.payload);
      return true;

    case CommandType::RM_CFG:
      Serial.print(F("Remove configuration ID: "));
      Serial.println(cmd_result.payload);
      return true;
    case CommandType::SHOW_CFG:
    {
      global_buttons_configuration.print_configuration();
      return true;
    }
    default:
      Serial.println(F("Unknown command!"));
      return false;
  }
}