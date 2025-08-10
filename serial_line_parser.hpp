#pragma once

enum class CommandType 
{
  UNKNOWN,
  SWITCH_CFG,
  ADD_CFG,
  RM_CFG,
  SHOW_CFG
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
  return CommandType::UNKNOWN;
}


CommandResult parse_serial_line(const String &line) 
{
  String cmdStr = line;
  String payload;

  int eqIndex = line.indexOf('=');
  if (eqIndex == -1)
  {
    cmdStr = line.substring(0, eqIndex);
    payload = line.substring(eqIndex + 1); 
  }  
  Serial.println("Command recognizion...");
  CommandType cmd = recognize_command(cmdStr);
  return {recognize_command(cmdStr), payload};
}
