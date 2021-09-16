// ----------------------------------------------------------------------------
// NamedElement.cpp
//
//
// Authors:
// Peter Polidoro peter@polidoro.io
// ----------------------------------------------------------------------------
#include "NamedElement.h"


namespace modular_server
{
// public
NamedElement::NamedElement()
{
  setName(constants::empty_constant_string);
}

void NamedElement::setName(const ConstantString & name)
{
  name_ptr_ = &name;
}

bool NamedElement::compareName(const char * name_to_compare)
{
  char name_str[name_ptr_->length()+1];
  name_str[0] = '\0';
  name_ptr_->copy(name_str);
  return String(name_str).equalsIgnoreCase(name_to_compare);
}

bool NamedElement::compareName(const ConstantString & name_to_compare)
{
  return (&name_to_compare == name_ptr_);
}

const ConstantString & NamedElement::getName()
{
  return *name_ptr_;
}

}
