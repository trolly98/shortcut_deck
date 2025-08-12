#include "key_parser.hpp"

KeyParser::KeyParser() {}

KeyParser::~KeyParser() {}

int KeyParser::parse_keys(const String &key_combination,
                         SpecialKey* special_keys[], 
                         int max_keys)
{
    Serial.println("Parsing keys: " + key_combination);
    int count = 0;
    String current = "";

    for (int i = 0; i < key_combination.length(); i++) 
    {
        char ch = key_combination[i];
        if (ch == '+') 
        {
            if (count < max_keys) 
            {
                special_keys[count++] = this->parse_key(current);
            }
            current = "";
        } 
        else 
        {
            current += ch;
        }
    }

    if (current.length() > 0 && count < max_keys) 
    {
        special_keys[count++] = this->parse_key(current);
    }

    Serial.println("Parsed keys count: " + String(count));
    return count;
}

SpecialKey* KeyParser::parse_key(const String& key)
{
  return SpecialKeyFactory::create_special_key(key);
}