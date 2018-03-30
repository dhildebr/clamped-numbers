#include "clamped_numbers.hh"

using namespace clamped;

template<typename NumT>
inline virtual const NumT & clamped::BasicClampedNumber<NumT>::value(const NumT &newVal)
{
  if(newVal < this->_minValue)
    return (this->_value = this->_minValue);
  else if(newVal > this->_maxValue)
    return (this->_value = this->_maxValue);
  else
    return (this->_value = newVal);
}

template<typename NumT>
inline virtual const NumT & clamped::BasicClampedNumber<NumT>::minValue(const NumT &newMin)
{
  // The new maximum must be greater than or equal to the current value
  return ((newMin <= this->_value) ? this->_minValue = newMin : this->_minValue = this->_value);
}

template<typename NumT>
inline virtual const NumT & clamped::BasicClampedNumber<NumT>::maxValue(const NumT &newMax)
{
  // The new minimum must be less than or equal to the current value
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
  if(other == 0)
    this->_value = 0;
  else if(other < 1 && other > -1)
    return (*this / (1 / other));
  else {
    if(this->_maxValue / this->_value >= other)
      this->_value *= other;
    else if((this->_value > 0 && other > 0) || (this->_value < 0 && other < 0))
      this->_value = this->_maxValue;
    else
      this->_value = this->_minValue;
  }
  
  return *this;
}

template<typename NumT>
virtual BasicClampedNumber<NumT> & clamped::BasicClampedNumber<NumT>::operator/=(const NumT &other)
{
  // Discard no-effect divisions
  if(this->_value == 0 || other == 1)
    return *this;
  
  // Division by -1 is the same as multiplication by -1
  else if(other == -1)
    return (*this *= -1);
  
  // Handle division by zero
  else if(other == 0) {
    if(this->_value > 0)
      this->_value = this->_maxValue;
    else if(this->_value < 0)
      this->_value = this->_minValue;
    else
      this->_value = 0;
  }
  
  // Delegate to multiplication for division by numbers where |n| < 1
  else if(other < 1 && other > -1)
    return (*this * (1 / other));
  
  // Handle multiplication by positive numbers
  else if(other > 0) {
    if(this->_value > 0)
      this->_value = (this->_value / this->_minValue >= other) ? this->_value / other : this->_minValue;
    else
      this->_value = (this->_value / this->_minValue <= -other) ? this->_value / other : this->_maxValue;
  }
  
  // Handle multiplication by negative numbers
  else {
    if(this->_value > 0)
      this->_value = (this->_value / this->_minValue <= -other) ? this->_value / other : this->_maxValue;
    else
      this->_value = (this->_value / this->_minValue >= other) ? this->_value / other : this->_minValue;
  }
  
  return *this;
}
