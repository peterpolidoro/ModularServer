// ----------------------------------------------------------------------------
// Interrupt.cpp
//
//
// Authors:
// Peter Polidoro polidorop@janelia.hhmi.org
// ----------------------------------------------------------------------------
#include "Interrupt.h"


namespace modular_server
{
// public
Interrupt::Interrupt()
{
  setup(constants::empty_constant_string);
}

// void Interrupt::attachFunctor(const Functor0 & functor)
// {
//   functor_ = functor;
// }

// void Interrupt::addParameter(Parameter & parameter)
// {
//   const ConstantString & parameter_name = parameter.getName();
//   int parameter_index = findParameterIndex(parameter_name);
//   if (parameter_index < 0)
//   {
//     parameter_ptrs_.push_back(&parameter);
//   }
// }

// void Interrupt::setReturnTypeLong()
// {
//   return_type_ = JsonStream::LONG_TYPE;
// }

// void Interrupt::setReturnTypeDouble()
// {
//   return_type_ = JsonStream::DOUBLE_TYPE;
// }

// void Interrupt::setReturnTypeBool()
// {
//   return_type_ = JsonStream::BOOL_TYPE;
// }

// void Interrupt::setReturnTypeNull()
// {
//   return_type_ = JsonStream::NULL_TYPE;
// }

// void Interrupt::setReturnTypeString()
// {
//   return_type_ = JsonStream::STRING_TYPE;
// }

// void Interrupt::setReturnTypeObject()
// {
//   return_type_ = JsonStream::OBJECT_TYPE;
// }

// void Interrupt::setReturnTypeArray()
// {
//   return_type_ = JsonStream::ARRAY_TYPE;
// }

// void Interrupt::setReturnTypeAny()
// {
//   return_type_ = JsonStream::ANY_TYPE;
// }

// void Interrupt::setReturnType(JsonStream::JsonTypes type)
// {
//   return_type_ = type;
// }

// JsonStream::JsonTypes Interrupt::getReturnType()
// {
//   return return_type_;
// }

// // protected

// // private
Interrupt::Interrupt(const ConstantString & name)
{
  setup(name);
}

void Interrupt::setup(const ConstantString & name)
{
  setName(name);
}

// int Interrupt::findParameterIndex(const ConstantString & parameter_name)
// {
//   int parameter_index = -1;
//   for (size_t i=0; i<parameter_ptrs_.size(); ++i)
//   {
//     if (parameter_ptrs_[i]->compareName(parameter_name))
//     {
//       parameter_index = i;
//       break;
//     }
//   }
//   return parameter_index;
// }

// size_t Interrupt::getParameterCount()
// {
//   return parameter_ptrs_.size();
// }

// void Interrupt::functor()
// {
//   if (functor_)
//   {
//     functor_();
//   }
// }

}
