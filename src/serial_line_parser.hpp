#pragma once

#include "globals.hpp"

enum class CommandType 
{
  UNKNOWN,
  SWITCH_CFG,
  ADD_CFG,
  RM_CFG,
  SHOW_CFG,
  SHOW_ACTUAL_CFG
};

struct CommandResult 
{
  CommandType type;
  String payload;
};

CommandType recognize_command(const String &cmdStr) 
{
  if (cmdStr == "SWITCH_CFG") return CommandType::SWITCH_CFG;
  if (cmdStr == "ADD_CFG")    return CommandType::ADD_CFG;
  if (cmdStr == "RM_CFG")     return CommandType::RM_CFG;
  if (cmdStr == "SHOW_CFG")   return CommandType::SHOW_CFG;
  if (cmdStr == "SHOW_ACTUAL_CFG")   return CommandType::SHOW_ACTUAL_CFG;
  return CommandType::UNKNOWN;
}

CommandResult parse_serial_line(const String &line) 
{
  String cmdStr = line;
  String payload;
  int eqIndex = line.indexOf('=');
  if (eqIndex != -1)
  {
    cmdStr = line.substring(0, eqIndex);
    payload = line.substring(eqIndex + 1); 
  }  
  Serial.println("Command recognizion: " + cmdStr);
  CommandType cmd = recognize_command(cmdStr);
  return {recognize_command(cmdStr), payload};
}

// void parse_json(const String &json_data, char functions[MAX_BTN_NUMBER][MAX_CONFIGURATION_NUMBER]) 
// {
//     for (int i = 0; i < MAX_BTN_NUMBER; i++) 
//     {
//         String key = "\"btn_" + String(i + 1) + "\":\"";
//         int start = json_data.indexOf(key);
//         if (start != -1) 
//         {
//             start += key.length();
//             int end = json_data.indexOf("\"", start);
//             if (end != -1) 
//             {
//                 int len = end - start;
//                 if (len >= MAX_CONFIGURATION_NUMBER) len = MAX_CONFIGURATION_NUMBER - 1;
//                 json_data.substring(start, end).toCharArray(functions[i], len + 1);
//             } 
//             else 
//             {
//                 functions[i][0] = '\0';
//             }
//         } 
//         else 
//         {
//             functions[i][0] = '\0';
//         }
//     }
// }

void parse_json(String json_data, String functions[MAX_BTN_NUMBER]) 
{
    for (int i = 0; i < MAX_BTN_NUMBER; i++) 
    {
        String key = "\"btn_" + String(i + 1) + "\":\"";
        int start = json_data.indexOf(key);
        if (start != -1) 
        {
            start += key.length();
            int end = json_data.indexOf("\"", start);
            if (end != -1) 
            {
                functions[i] = json_data.substring(start, end);
            } 
            else 
            {
                functions[i] = "";
            }
        } 
        else 
        {
            functions[i] = "";
        }
    }
}
