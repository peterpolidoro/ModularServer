// ----------------------------------------------------------------------------
// Response.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Response.h"


namespace modular_server
{
void Response::writeNullToResponse()
{
  json_stream_.writeNull();
}

void Response::writeToResponse(Vector<const constants::SubsetMemberType> & value, JsonStream::JsonTypes type)
{
  switch (type)
  {
    case JsonStream::LONG_TYPE:
    {
      Array<long,constants::SUBSET_ELEMENT_COUNT_MAX> subset_elements_array;
      for (size_t i=0; i<value.size(); ++i)
      {
        subset_elements_array.push_back(value[i].l);
      }
      json_stream_.write(subset_elements_array);
      break;
    }
    case JsonStream::DOUBLE_TYPE:
    {
      break;
    }
    case JsonStream::BOOL_TYPE:
    {
      break;
    }
    case JsonStream::NULL_TYPE:
    {
      break;
    }
    case JsonStream::STRING_TYPE:
    {
      Array<ConstantString *,constants::SUBSET_ELEMENT_COUNT_MAX> subset_elements_array;
      for (size_t i=0; i<value.size(); ++i)
      {
        subset_elements_array.push_back(value[i].cs_ptr);
      }
      json_stream_.write(subset_elements_array);
      break;
    }
    case JsonStream::OBJECT_TYPE:
    {
      break;
    }
    case JsonStream::ARRAY_TYPE:
    {
      break;
    }
    case JsonStream::VALUE_TYPE:
    {
      break;
    }
  }
}

void Response::writeResultKeyToResponse()
{
  // Prevent multiple results in one response
  if (!result_key_in_response_ && !error_)
  {
    result_key_in_response_ = true;
    json_stream_.writeKey(constants::result_constant_string);
  }
}

void Response::beginResponseObject()
{
  json_stream_.beginObject();
}

void Response::endResponseObject()
{
  json_stream_.endObject();
}

void Response::beginResponseArray()
{
  json_stream_.beginArray();
}

void Response::endResponseArray()
{
  json_stream_.endArray();
}

}
