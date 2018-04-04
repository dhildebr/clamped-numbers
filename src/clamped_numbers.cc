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
  // Discard no-effect additions
  if(this->_value == this->_maxValue || other == 0)
    return *this;
  
  // Delegate to subtraction when adding negatives
  else if(other < 0)
    return (*this -= (-other));
  
  // Handle remaining cases: other > 0
  else {
    // Positive max, negative min: the reverse is ipossible
    if(this->_maxValue >= 0 && this->_value < 0) {
      if(other + this->_value <= this->_maxValue)
        this->_value += other;
      else
        this->_value = this->_maxValue;
    }
    
    // Maximum and current have matching signs
    else {
      if(this->_maxValue - this->_value >= other)
        this->_value += other;
      else
        this->_value = this->_maxValue;
    }
  }
  
  return *this;
}

template<typename NumT>
virtual BasicClampedNumber<NumT> & clamped::BasicClampedNumber<NumT>::operator-=(const NumT &other)
{
  // Discard no-effect subtractions
  if(this->_value == this->_minValue || other == 0)
    return *this;
  
  // Delegate to addition for subtraction of negatives
  else if(other < 0)
    return (*this += (-other));
  
  // Handle remaining cases: other > 0
  else {
    // Negative minimum, positive current: the reverse is impossible
    if(this->_minValue < 0 && this->_value >= 0) {
      if(other - this->_value >= this->_minValue)
        this->_value -= other;
      else
        this->_value = this->_minValue;
    }
    
    // Minimum and current have matching signs
    else {
      if(this->_value - this->_minValue <= other)
        this->_value -= other;
      else
        this->_value = this->_minValue;
    }
  }
  
  return *this;
}

template<typename NumT>
virtual BasicClampedNumber<NumT> & clamped::BasicClampedNumber<NumT>::operator*=(const NumT &other)
{
  // Multiplication by zero is trivially done
  if(other == 0)
    this->_value = 0;
  
  // Delegate to division for multiplication where |other| < 1
  else if(other < 1 && other > -1)
    return (*this / (1 / other));
  
  // Handle remaining cases, i.e. where |other| >= 1
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
  
  // Division by -1 is the same as multiplication by -1 (delegate)
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
  
  // Delegate to multiplication for division by numbers where |other| < 1
  else if(other < 1 && other > -1)
    return (*this * (1 / other));
  
  // Handle division by positive numbers: other > 1
  else if(other > 0) {
    if(this->_value > 0)
      this->_value = (this->_value / this->_minValue >= other) ? this->_value / other : this->_minValue;
    else
      this->_value = (this->_value / this->_minValue <= -other) ? this->_value / other : this->_maxValue;
  }
  
  // Handle division by negative numbers: other < -1
  else {
    if(this->_value > 0)
      this->_value = (this->_value / this->_minValue <= -other) ? this->_value / other : this->_maxValue;
    else
      this->_value = (this->_value / this->_minValue >= other) ? this->_value / other : this->_minValue;
  }
  
  return *this;
}

template<typename NatT>
virtual ClampedNaturalNumber<NatT> & clamped::ClampedNaturalNumber<NatT>::operator%=(const NatT &other)
{
  if(other == 0 || other > this->_value)
    this->_value = 0;
  else if(this->_value % other < this->_minValue)
    this->_value = this->_minValue;
  else if(this->_value % other > this->_maxValue)
    this->_value = this->_maxValue;
  else
    this->_value %= other;
  
  return *this;
}
