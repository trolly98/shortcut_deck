#pragma once

#include "serial_line_parser.hpp"
#include "command_executor.hpp"

static String inputBuffer;

void check_serial_config()
{
  while (Serial.available()) 
  {
    char c = Serial.read();
    if (c == '\n') 
    {
      if (inputBuffer.length() > 0) 
      {
        CommandResult cmd_result = parse_serial_line(inputBuffer);
        if (!exec_cmd(cmd_result))
        {
          Serial.println("CMD execution failed!");
        }
        inputBuffer = "";
      }
    }
    else if (c != '\r') 
    {
      inputBuffer += c;
    }
  }
}