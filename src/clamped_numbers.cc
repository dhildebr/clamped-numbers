#include "clamped_numbers.hh"

using namespace clamped;

template<typename NumT>
virtual const NumT & clamped::BasicClampedNumber<NumT>::value(const NumT &newVal)
{
  if(newVal < this->_minValue)
    return (this->_value = this->_minValue);
  else if(newVal > this->_maxValue)
    return (this->_value = this->_maxValue);
  else
    return (this->_value = newVal);
}

template<typename NumT>
virtual const NumT & clamped::BasicClampedNumber<NumT>::minValue(const NumT &newMin)
{
  if(newMin > this->_value)
    return (this->_minValue = this->_value);
  else
    return (this->_minValue = newMin);
}

template<typename NumT>
virtual const NumT & clamped::BasicClampedNumber<NumT>::maxValue(const NumT &newMax)
{
  if(newMax < this->_value)
    return (this->_maxValue = this->_value);
  else
    return (this->_maxValue = newMax);
}
