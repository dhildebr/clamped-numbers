#include <cstdint>

#include "clamped_numbers.hh"

using namespace clamped;

// An enumeration of clamping reactions during modification
enum class ClampReaction: uint8_t
{ 
  MAXIMUM, // Value should clamp to maximum
  MINIMUM, // Value should clamp to minimum
  NONE     // Value can be modified normally
};

template<typename NumT>
inline const NumT & clamped::BasicClampedNumber<NumT>::value(const NumT &newVal)
{
  if(newVal < this->_minValue)
    return (this->_value = this->_minValue);
  else if(newVal > this->_maxValue)
    return (this->_value = this->_maxValue);
  else
    return (this->_value = newVal);
}

template<typename NumT>
inline const NumT & clamped::BasicClampedNumber<NumT>::minValue(const NumT &newMin)
{
  // The new maximum must be greater than or equal to the current value
  return ((newMin <= this->_value) ? this->_minValue = newMin : this->_minValue = this->_value);
}

template<typename NumT>
inline const NumT & clamped::BasicClampedNumber<NumT>::maxValue(const NumT &newMax)
{
  // The new minimum must be less than or equal to the current value
  return ((newMax >= this->_value) ? this->_maxValue = newMax : this->_maxValue = this->_value);
}

// ############################################### BasicClampedNumber ############################################### //
// ################################################################################################################## //
// ############################################## ClampedNaturalNumber ############################################## //

template<typename NatT>
ClampedNaturalNumber<NatT> & clamped::ClampedNaturalNumber<NatT>::operator+=(const NatT &other)
{
  // Discard no-effect additions
  if(this->_value >= this->_maxValue || other == 0)
    return *this;
  
  // Handle remaining cases: other > 0
  else {
    if(this->_maxValue - this->_value >= other)
      this->_value += other;
    else
      this->_value = this->_maxValue;
  }
  
  return *this;
}

template<typename NatT>
ClampedNaturalNumber<NatT> & clamped::ClampedNaturalNumber<NatT>::operator-=(const NatT &other)
{
  // Discard no-effect subtractions
  if(this->_value <= this->_minValue || other == 0)
    return *this;
  
  // Handle remaining cases: other > 0
  else {
    if(this->_value - this->_minValue <= other)
      this->_value -= other;
    else
      this->_value = this->_minValue;
  }
  
  return *this;
}

template<typename NatT>
ClampedNaturalNumber<NatT> & clamped::ClampedNaturalNumber<NatT>::operator*=(const NatT &other)
{
  // Multiplication by zero is trivially done
  if(other == 0 || this->_value == 0)
    this->_value = 0;
  
  // Handle remaining cases, i.e. where |other| >= 1
  else {
    if(this->_maxValue / this->_value >= other)
      this->_value *= other;
    else
      this->_value = this->_maxValue;
  }
  
  return *this;
}

template<typename NatT>
ClampedNaturalNumber<NatT> & clamped::ClampedNaturalNumber<NatT>::operator/=(const NatT &other)
{
  // Discard no-effect divisions
  if(this->_value != 0 && other != 1) {
    // Handle division by zero
    if(other == 0) {
      if(this->_value > 0)
        this->_value = this->_maxValue;
      else
        this->_value = 0;
    }
    
    // Handle division by positive numbers: other > 1
    else if(other > 0) {
      if(this->_value / this->_minValue >= other)
        this->_value /= other;
      else
        this->_value = this->_minValue;
    }
    
    return *this;
  }
}

template<typename NatT>
ClampedNaturalNumber<NatT> & clamped::ClampedNaturalNumber<NatT>::operator%=(const NatT &other)
{
  if(this->_value % other < this->_minValue)
    this->_value = this->_minValue;
  else if(this->_value % other > this->_maxValue)
    this->_value = this->_maxValue;
  else
    this->_value %= other;
  
  return *this;
}

// ############################################## ClampedNaturalNumber ############################################## //
// ################################################################################################################## //
// ################################################# ClampedInteger ################################################# //

// Invariants: other > 0
template<typename IntT>
static ClampReaction addReactionInteger(const IntT &current, const IntT &other, const IntT &, const IntT &max)
{
  // Positive max, negative current: the reverse is impossible
  if(max >= 0 && current < 0)
    return (current + other <= max) ? ClampReaction::NONE : ClampReaction::MAXIMUM;
  
  // Maximum and current have matching signs
  else
    return (max - current >= other) ? ClampReaction::NONE : ClampReaction::MAXIMUM;
}

// Invariants: other > 0
template<typename IntT>
static ClampReaction subtractReactionInteger(const IntT &current, const IntT &other, const IntT &min, const IntT &)
{
  // Negative minimum, positive current: the reverse is impossible
  if(min < 0 && current >= 0)
    return (current - other >= min) ? ClampReaction::NONE : ClampReaction::MINIMUM;
  
  // Minimum and current have matching signs
  else
    return (current - min <= other) ? ClampReaction::NONE : ClampReaction::MINIMUM;
}

// Invariants: current != 0, |other| >= 1
template<typename IntT>
static ClampReaction multiplyReactionInteger(const IntT &current, const IntT &other, const IntT &min, const IntT &max)
{
  if(current > 0) {
    if(other > 0) {
      if(other >= max)
        return ClampReaction::MAXIMUM;
      else
        return (max / current >= other) ? ClampReaction::NONE : ClampReaction::MAXIMUM;
    }
    else {
      if(min >= 0)
        return ClampReaction::MINIMUM;
      else if(other <= min)
        return ClampReaction::MINIMUM;
      else
        return (min / -other >= current) ? ClampReaction::NONE : ClampReaction::MINIMUM;
    }
  }
  else {
    if(other > 0) {
      return (min / current >= other) ? ClampReaction::NONE : ClampReaction::MINIMUM;
    }
    else {
      if(other <= min)
        return ClampReaction::MINIMUM;
      else
        return (min / current >= -other) ? ClampReaction::NONE : ClampReaction::MINIMUM;
    }
  }
  
  return ClampReaction::NONE;
}

// Invariants: current != 0, other != 0, other != 1
template<typename IntT>
static ClampReaction divideReactionInteger(const IntT &current, const IntT &other, const IntT &min, const IntT &max)
{
  if(current > 0) {
    if(other > 0) {
      if(min < 0)
        return ClampReaction::NONE;
      else
        return (current / other >= min) ? ClampReaction::NONE : ClampReaction::MINIMUM;
    }
    else {
      if(min >= 0)
        return ClampReaction::MINIMUM;
      else
        return (current / other >= min) ? ClampReaction::NONE : ClampReaction::MINIMUM;
    }
  }
  else {
    if(other > 0) {
      return (current / other >= min) ? ClampReaction::NONE : ClampReaction::MINIMUM;
    }
    else {
      if(max < 0)
        return ClampReaction::MAXIMUM;
      else
        return (current / other <= max) ? ClampReaction::NONE : ClampReaction::MAXIMUM;
    }
  }
  
  return ClampReaction::NONE;
}

template<typename IntT>
ClampedInteger<IntT> & clamped::ClampedInteger<IntT>::operator+=(const IntT &other)
{
  // Discard no-effect additions
  if(other == 0)
    return *this;
  
  // Delegate to subtraction when adding negatives
  else if(other < 0)
    return (*this -= (-other));
  
  // Handle remaining cases: other > 0
  else {
    switch(addReactionInteger(this->_value, other, this->_minValue, this->_maxValue)) {
      case ClampReaction::NONE:
        this->_value += other;
      break;
      case ClampReaction::MAXIMUM:
        this->_value = this->_maxValue;
      break;
      case ClampReaction::MINIMUM:
        this->_value = this->_minValue;
      break;
    }
    
    return *this;
  }
}

template<typename IntT>
ClampedInteger<IntT> & clamped::ClampedInteger<IntT>::operator-=(const IntT &other)
{
  // Discard no-effect subtractions
  if(other == 0)
    return *this;
  
  // Delegate to addition for subtraction of negatives
  else if(other < 0)
    return (*this += (-other));
  
  // Handle remaining cases: other > 0
  else {
    switch(subtractReactionInteger(this->_value, other, this->_minValue, this->_maxValue)) {
      case ClampReaction::NONE:
        this->_value -= other;
      break;
      case ClampReaction::MAXIMUM:
        this->_value = this->_maxValue;
      break;
      case ClampReaction::MINIMUM:
        this->_value = this->_minValue;
      break;
    }
    
    return *this;
  }
}

template<typename IntT>
ClampedInteger<IntT> & clamped::ClampedInteger<IntT>::operator*=(const IntT &other)
{
  // Multiplication by zero is trivially done
  if(this->_value == 0 || other == 0)
    this->_value = 0;
  
  // Handle remaining cases, i.e. where |other| >= 1
  else {
    switch(multiplyReactionInteger(this->_value, other, this->_minValue, this->_maxValue)) {
      case ClampReaction::NONE:
        this->_value *= other;
      break;
      case ClampReaction::MAXIMUM:
        this->_value = this->_maxValue;
      break;
      case ClampReaction::MINIMUM:
        this->_value = this->_minValue;
      break;
    }
  }
  
  return *this;
}

template<typename IntT>
ClampedInteger<IntT> & clamped::ClampedInteger<IntT>::operator/=(const IntT &other)
{
  // Discard no-effect divisions
  if(this->_value == 0 || other == 1)
    return *this;
  
  // Handle division by zero
  else if(other == 0) {
    if(this->_value > 0)
      this->_value = this->_maxValue;
    else if(this->_value < 0)
      this->_value = this->_minValue;
    else
      this->_value = 0;
  }
  
  // Handle division by positive numbers: other != 0, other != 1
  else {
    switch(divideReactionInteger(this->_value, other, this->_minValue, this->_maxValue)) {
      case ClampReaction::NONE:
        this->_value /= other;
      break;
      case ClampReaction::MAXIMUM:
        this->_value = this->_maxValue;
      break;
      case ClampReaction::MINIMUM:
        this->_value = this->_minValue;
      break;
    }
  }
  
  return *this;
}

template<typename IntT>
ClampedInteger<IntT> & clamped::ClampedInteger<IntT>::operator%=(const IntT &other)
{
  if(this->_value % other < this->_minValue)
    this->_value = this->_minValue;
  else if(this->_value % other > this->_maxValue)
    this->_value = this->_maxValue;
  else
    this->_value %= other;
  
  return *this;
}

// ################################################# ClampedInteger ################################################# //
// ################################################################################################################## //
// ################################################# ClampedDecimal ################################################# //

template<typename FloatT>
static ClampReaction addReactionDecimal(const FloatT &current, const FloatT &other,
    const FloatT &min, const FloatT &max)
{
  return ClampReaction::NONE;
}

template<typename FloatT>
static ClampReaction subtractReactionDecimal(const FloatT &current, const FloatT &other,
    const FloatT &min, const FloatT &max)
{
  return ClampReaction::NONE;
}

template<typename FloatT>
static ClampReaction multiplyReactionDecimal(const FloatT &current, const FloatT &other,
    const FloatT &min, const FloatT &max)
{
  return ClampReaction::NONE;
}

template<typename FloatT>
static ClampReaction divideReactionDecimal(const FloatT &current, const FloatT &other,
    const FloatT &min, const FloatT &max)
{
  return ClampReaction::NONE;
}

template<typename FloatT>
ClampedDecimal<FloatT> & clamped::ClampedDecimal<FloatT>::operator+=(const FloatT &other)
{
  return *this;
}

template<typename FloatT>
ClampedDecimal<FloatT> & clamped::ClampedDecimal<FloatT>::operator-=(const FloatT &other)
{
  return *this;
}

template<typename FloatT>
ClampedDecimal<FloatT> & clamped::ClampedDecimal<FloatT>::operator*=(const FloatT &other)
{
  return *this;
}

template<typename FloatT>
ClampedDecimal<FloatT> & clamped::ClampedDecimal<FloatT>::operator/=(const FloatT &other)
{
  return *this;
}

// ################################################################################################################## //

template<typename NumT>
BasicClampedNumber<NumT> & clamped::BasicClampedNumber<NumT>::operator+=(const NumT &other)
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
BasicClampedNumber<NumT> & clamped::BasicClampedNumber<NumT>::operator-=(const NumT &other)
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
BasicClampedNumber<NumT> & clamped::BasicClampedNumber<NumT>::operator*=(const NumT &other)
{
  // Multiplication by zero is trivially done
  if(other == 0)
    this->_value = 0;
  
  // Delegate to division for multiplication where |other| < 1
  else if(other < 1 && other > -1)
    return (*this /= (1 / other));
  
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
BasicClampedNumber<NumT> & clamped::BasicClampedNumber<NumT>::operator/=(const NumT &other)
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
    return (*this *= (1 / other));
  
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
