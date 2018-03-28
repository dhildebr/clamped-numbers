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
  return ((newMin <= this->_value) ? this->_minValue = newMin : this->_minValue = this->_value);
}

template<typename NumT>
virtual const NumT & clamped::BasicClampedNumber<NumT>::maxValue(const NumT &newMax)
{
  return ((newMax >= this->_value) ? this->_maxValue = newMax : this->_maxValue = this->_value);
}

template<typename NumT>
virtual BasicClampedNumber<NumT> & clamped::BasicClampedNumber<NumT>::operator+=(const NumT &other)
{
  if(this->_value == this->_maxValue || other == 0)
    return *this;
  else if(other < 0)
    return (*this -= (-other));
  else {
    if(this->_maxValue >= 0 && this->_value < 0) // Different signs: the reverse is impossible
      return ((other + this->_value <= this->_maxValue) ? this->_value += other : this->_value = this->_maxValue);
    else
      return ((this->_maxValue - this->_value >= other) ? this->_value += other : this->_value = this->_maxValue);
  }
}

template<typename NumT>
virtual BasicClampedNumber<NumT> & clamped::BasicClampedNumber<NumT>::operator-=(const NumT &other)
{
  if(this->_value == this->_minValue || other == 0)
    return *this;
  else if(other < 0)
    return (*this += (-other));
  else {
    if(this->_minValue < 0 && this->_value >= 0) // Different signs: the reverse is impossible
      return ((other - this->_value >= this->_minValue) ? this->_value -= other : this->_value = this->_minValue);
    else
      return ((this->_value - this->_minValue <= other) ? this->_value -= other : this->_value = this->_minValue);
  }
}

template<typename NumT>
virtual BasicClampedNumber<NumT> & clamped::BasicClampedNumber<NumT>::operator*=(const NumT &other)
{
  return *this;
}

template<typename NumT>
virtual BasicClampedNumber<NumT> & clamped::BasicClampedNumber<NumT>::operator/=(const NumT &other)
{
  return *this;
}
