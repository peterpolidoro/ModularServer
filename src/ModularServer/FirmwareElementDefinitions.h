// ----------------------------------------------------------------------------
// FirmwareElementDefinitions.h
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#ifndef _MODULAR_SERVER_FIRMWARE_ELEMENT_DEFINITIONS_H_
#define _MODULAR_SERVER_FIRMWARE_ELEMENT_DEFINITIONS_H_


namespace modular_server
{
// public
template <typename U,
  size_t MAX_SIZE>
bool FirmwareElement::firmwareNameInArray(Array<U,MAX_SIZE> & firmware_name_array)
{
  for (size_t i=0; i<firmware_name_array.size(); ++i)
  {
    if (compareFirmwareName(firmware_name_array[i]))
    {
      return true;
    }
  }
  return false;
}

}
#endif
