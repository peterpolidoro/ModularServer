// ----------------------------------------------------------------------------
// SavedVariable.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef SAVED_VARIABLE_H
#define SAVED_VARIABLE_H
#include <EEPROM.h>
#include "Flash.h"


namespace RemoteDevice
{
class SavedVariable
{
public:
  template<typename T>
  SavedVariable(const _FLASH_STRING &name, const unsigned int eeprom_index, const T &default_value)
  {
    setName(name);
    eeprom_index_ = eeprom_index;
    size_ = sizeof(default_value);
    default_value_ptr_ = &default_value;
  }
  void setName(const _FLASH_STRING &name);
  template<typename T>
  int setValue(const T &value)
  {
    const byte* p = (const byte*)(const void*)&value;
    int ee = eeprom_index_;
    int i;
    for (i = 0; i < sizeof(value); i++)
    {
      if(EEPROM.read(ee)==*p)
      {
        ee++;
        p++;
      }
      else
      {
        EEPROM.write(ee++,*p++);
      }
    }
    size_ = i;
    return i;
  }
  template<typename T>
  int getValue(T &value)
  {
    byte* p = (byte*)(void*)&value;
    int ee = eeprom_index_;
    int i;
    for (i = 0; i < sizeof(value); i++)
    {
      if (i < size_)
      {
        *p++ = EEPROM.read(ee++);
      }
    }
    return i;
  }
  void setDefaultValue();
private:
  const _FLASH_STRING *name_ptr_;
  unsigned int eeprom_index_;
  unsigned int size_;
  const void *default_value_ptr_;
  boolean compareName(const _FLASH_STRING& name_to_compare);
  friend class Server;
};
}
#endif
