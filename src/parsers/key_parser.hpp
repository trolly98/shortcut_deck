#pragma once

#include "special_key_factory.hpp"

class KeyParser
{
public:
  KeyParser();
  ~KeyParser();
  int parse_keys(const String &key_combination,
                SpecialKey* special_keys[], 
                int max_keys);
  SpecialKey* parse_key(const String& key);
};