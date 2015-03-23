// ----------------------------------------------------------------------------
// SavedVariable.h
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#ifndef _MODULAR_DEVICE_SAVED_VARIABLE_H_
#define _MODULAR_DEVICE_SAVED_VARIABLE_H_
#include <EEPROM.h>
#include "Flash.h"
#include "ConstantVariables.h"


namespace ModularDevice
{
class SavedVariable
{
public:
  SavedVariable();
  template<typename T>
  SavedVariable(const ConstantString &name,
                const unsigned int eeprom_index,
                const T &default_value)
  {
    setName(name);
    eeprom_index_ = eeprom_index;
    size_ = sizeof(default_value);
    default_value_ptr_ = &default_value;
    array_length_ = 0;
  }
  template<typename T>
  SavedVariable(const ConstantString &name,
                const unsigned int eeprom_index,
                const T default_value[],
                const unsigned int array_length)
  {
    setName(name);
    eeprom_index_ = eeprom_index;
    array_element_size_ = sizeof(T);
    size_ = array_length*array_element_size_;
    default_value_ptr_ = default_value;
    array_length_ = array_length;
  }
  void setName(const ConstantString &name);
  template<typename T>
  int setValue(const T &value)
  {
    int i = 0;
    if ((sizeof(value) == size_) && (array_length_ == 0))
    {
      const byte* p = (const byte*)(const void*)&value;
      int ee = eeprom_index_;
      for (i = 0; i < size_; i++)
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
    }
    return i;
  }
  template<typename T>
  int setValue(const T value[], const unsigned int array_index)
  {
    int i = 0;
    if (array_index < array_length_)
    {
      const byte* p = (const byte*)(const void*)&value[array_index];
      int ee = eeprom_index_ + array_index*array_element_size_;
      for (i = 0; i < array_element_size_; i++)
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
    }
    return i;
  }
  template<typename T>
  int getValue(T &value)
  {
    int i = 0;
    if ((sizeof(value) == size_) && (array_length_ == 0))
    {
      byte* p = (byte*)(void*)&value;
      int ee = eeprom_index_;
      int i;
      for (i = 0; i < size_; i++)
      {
        *p++ = EEPROM.read(ee++);
      }
    }
    return i;
  }
  template<typename T>
  int getValue(T value[], unsigned int array_index)
  {
    int i = 0;
    if (array_index < array_length_)
    {
      byte* p = (byte*)(void*)&value[array_index];
      int ee = eeprom_index_ + array_index*array_element_size_;
      for (i = 0; i < array_element_size_; i++)
      {
        if (i < size_)
        {
          *p++ = EEPROM.read(ee++);
        }
      }
    }
    return i;
  }
  void setDefaultValue();
  int getSize();
private:
  const ConstantString *name_ptr_;
  unsigned int eeprom_index_;
  unsigned int size_;
  const void *default_value_ptr_;
  unsigned int array_length_;
  unsigned int array_element_size_;
  boolean compareName(const ConstantString& name_to_compare);
  friend class Server;
};
}
#endif
