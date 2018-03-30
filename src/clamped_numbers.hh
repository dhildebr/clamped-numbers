#pragma once

#include <cstdint>

#if defined(INT8_MAX) && defined(INT8_MIN)
#define CLAMPED_INT8
#endif

#if defined(INT16_MAX) && defined(INT16_MIN)
#define CLAMPED_INT16
#endif

#if defined(INT32_MAX) && defined(INT32_MIN)
#define CLAMPED_INT32
#endif

#if defined(INT64_MAX) && defined(INT64_MIN)
#define CLAMPED_INT64
#endif

#if defined(UINT8_MAX)
#define CLAMPED_UINT8
#endif

#if defined(UINT16_MAX)
#define CLAMPED_UINT16
#endif

#if defined(UINT32_MAX)
#define CLAMPED_UINT32
#endif

#if defined(UINT64_MAX)
#define CLAMPED_UINT64
#endif

namespace clamped
{
  template<typename NumT>
  class BasicClampedNumber
  {
    protected:
    
    NumT _maxValue;
    NumT _minValue;
    NumT _value;

    public:
    
    /**
     * The default constructor is disabled. A BasicClampedNumber is
     * sufficiently general that no reasonable default values can be generated.
     */
    BasicClampedNumber() = delete;
    
    /**
     * Constructs a new BasicClampedNumber with the specified current, minimum,
     * and maximum values. The minimum value must be less than or equal to the
     * starting value: if it is not, it is itself clamped to the starting value.
     * The maximum value is similarly constrained, and must be greater than or
     * equal to the starting value.
     * 
     * \param value The starting value of this number
     * \param min the minimum value for this number
     * \param max the maximum value for this number
     */
    BasicClampedNumber(const NumT &value, const NumT &min, const NumT &max) :
        _value(value), _minValue((min <= value) ? min : value), _maxValue((max >= value) ? max : value)
    {
    }
    
    /**
     * Provides a virtual destructor with the default dehavior.
     */
    virtual ~BasicClampedNumber() = default;
    
    /**
     * Provides defaiult behavior for the copy constructor.
     */
    BasicClampedNumber(const BasicClampedNumber<NumT> &) = default;
    
    /**
     * Provides defaiult behavior for the move constructor.
     */
    BasicClampedNumber(BasicClampedNumber<NumT> &&) = default;
    
    /**
     * Provides default behavior for the copy assignment operator.
     * 
     * \return Returns a copy of the right operand.
     */
    virtual BasicClampedNumber<NumT> operator=(const BasicClampedNumber<NumT> &) = default;
    
    /**
     * Provides default behavior for the move assignment operator.
     * 
     * \return Returns the left operand's data, moved into the right operand.
     */
    virtual BasicClampedNumber<NumT> & operator=(BasicClampedNumber<NumT> &) = default;
    
    /**
     * Returns this number's current value by const reference.
     * 
     * \return Returns this number's current value.
     */
    virtual const NumT & value() const final
    {
      return this->_value;
    }
    
    /**
     * Returns this number's current maximum value by const reference.
     * 
     * \return Returns this number's current maximum value.
     */
    virtual const NumT & maxValue() const final
    {
      return this->_maxValue;
    }
    
    /**
     * Returns this number's current minimum value by const reference.
     * 
     * \return Returns this number's current minimum value.
     */
    virtual const NumT & minValue() const final
    {
      return this->_minValue;
    }
    
    /**
     * Sets this number's current value, as constrained by its bounds. In other
     * words, this function allow manual setting of the value, so long as the
     * new value is at least as much as the current minimum and no more than the
     * current maximum.
     * 
     * \param newVal the new new value for this number
     * \return Returns this number's current value after reassignment.
     */
    virtual const NumT & value(const NumT &newVal) final;

    /**
     * Sets this number's maximum value to that specified. The new maximum must
     * still be greater than or equal to the current stored value: if it is not,
     * it is constrained to the current value.
     * 
     * \param newMax the new maximum for this number
     * \return Returns this number's maximum value after reassignment.
     */
    virtual const NumT & maxValue(const NumT &newMax) final;

    /**
     * Sets this number's minimum value to that specific. The minimum must
     * still be less than or equal to the current stored value: if it is not, it
     * is constrained to the current value.
     * 
     * \param newMin the new minimum for this number
     * \return Returns this number's minimum value after reassignment.
     */
    virtual const NumT & minValue(const NumT &newMin) final;

    /**
     * Sets this number's current value to its maximum. After calling this
     * function, therefore, value() == maxValue().
     * 
     * \return Returns this number's current value after modification.
     */
    virtual const NumT & minimize() final
    {
      return (this->_value = this->_minValue);
    }
    
    /**
     * Sets this number's current value to its minimum. After calling this
     * function, therefore, value() == minValue().
     * 
     * \return Returns this number's current value after modification.
     */
    virtual const NumT & maximize() final
    {
      return (this->_value = this->_maxValue);
    }
    
    /**
     * Adds the given number to this one, as constrained by this
     * number's bounds.
     * 
     * \param other the right operand for addition
     * \return Returns this number, allowing chaining of operations.
     */
    virtual BasicClampedNumber<NumT> & operator+=(const NumT &other);

    /**
     * Subtracts the given number from this one, as constrained by this
     * number's bounds.
     * 
     * \param other the right operand for subtraction
     * \return Returns this number, allowing chaining of operations.
     */
    virtual BasicClampedNumber<NumT> & operator-=(const NumT &other);

    /**
     * Multiplies this number by the one given, as constrained by this
     * number's bounds.
     * 
     * \param other the right operand for multiplication
     * \return Returns this number, allowing chaining of operations.
     */
    virtual BasicClampedNumber<NumT> & operator*=(const NumT &other);

    /**
     * Divides this number by the one given, as constrained by this number's
     * bounds. Division by zero will not throw an exception: instead, the
     * resultant undefined or "infinte" value will be this number's maximum or
     * minimum, depending on its sign (if applicable) prior to division.
     * 
     * \param other the right operand for division
     * \return Returns this number, allowing chaining of operations.
     */
    virtual BasicClampedNumber<NumT> & operator/=(const NumT &other);

    /**
     * Increments this number by one, within its bounds.
     * 
     * \return Returns this number post-incrementation.
     */
    virtual BasicClampedNumber<NumT> & operator++()
    {
      return (*this += 1);
    }
    
    /**
     * Decrements this number by one, within its bounds.
     * 
     * \return Returns this number post-decrementation.
     */
    virtual BasicClampedNumber<NumT> & operator--()
    {
      return (*this -= 1);
    }
    
    /**
     * Increments this number by one, within its bounds.
     * 
     * \return Returns a copy of this number, reflecting its state prior
     * to incrementation.
     */
    virtual BasicClampedNumber<NumT> operator++(int)
    {
      BasicClampedNumber<NumT> preIncr(*this);
      ++(*this);
      return preIncr;
    }
    
    /**
     * Decrements this number by one, within its bounds.
     * 
     * \return Returns a copy of this number, reflecting its state prior
     * to decrementation.
     */
    virtual BasicClampedNumber<NumT> operator--(int)
    {
      BasicClampedNumber<NumT> preDecr(*this);
      --(*this);
      return preDecr;
    }
    
    /**
     * Returns whether this number equals the other. The bounds of each number
     * play no part in equality: only the primary stored value is considered.
     * 
     * \param other the right operand compared against
     * \return Returns true if this number equals the other, else false
     */
    virtual bool operator==(const BasicClampedNumber<NumT> &other) const
    {
      return this->_value == other._value;
    }
    
    /**
     * Returns whether this number is not equal to the other. The bounds of each
     * number play no part in equality: only the primary stored value
     * is considered.
     * 
     * \param other the right operand compared against
     * \return Returns true if this number does not equal the other, else false
     */
    virtual bool operator!=(const BasicClampedNumber<NumT> &other) const
    {
      return !(*this == other);
    }
    
    /**
     * Returns whether this number is less than the other. The bounds of each
     * number play no part in equality: only the primary stored value
     * is considered.
     * 
     * \param other the right operand compared against
     * \return Returns true if this number is less than the other, else false
     */
    virtual bool operator<(const BasicClampedNumber<NumT> &other) const
    {
      return this->_value < other._value;
    }
    
    /**
     * Returns whether this number is less than or equal to the other. The
     * bounds of each number play no part in equality: only the primary stored
     * value is considered.
     * 
     * \param other the right operand compared against
     * \return Returns true if this number is less than or equal to the other,
     * else false
     */
    virtual bool operator<=(const BasicClampedNumber<NumT> &other) const
    {
      return *this < other || *this == other;
    }
    
    /**
     * Returns whether this number is greater than the other. The bounds of each
     * number play no part in equality: only the primary stored value
     * is considered.
     * 
     * \param other the right operand compared against
     * \return Returns true if this number is greater than the other, else false
     */
    virtual bool operator>(const BasicClampedNumber<NumT> &other) const
    {
      return !(*this <= other);
    }
    
    /**
     * Returns whether this number is greater than or equal to the other. The
     * bounds of each number play no part in equality: only the primary stored
     * value is considered.
     * 
     * \param other the right operand compared against
     * \return Returns true if this number is greater than or equal to the
     * other, else false
     */
    virtual bool operator>=(const BasicClampedNumber<NumT> &other) const
    {
      return !(*this < other);
    }
    
    /**
     * Allows the excplicit conversion of this BasicClampedNumber to an instance
     * of NumT, the numerical type it wraps. This conversion is similar to
     * calling value(), but returns a copy instead of a const reference.
     * 
     * \return Returns a copy of this number's internal value.
     */
    virtual explicit NumT operator NumT() const final
    {
      return this->_value;
    }
    
    /**
     * Allows the explicit conversion of this BasicClampedNumber to a boolean
     * value. This is equivalent to asking whether NumT(*this) == 0. As the
     * bounds of some numbers may forbid their equalling zero, these numbers
     * will always convert to false.
     * 
     * \return Returns true if this number equals zero, else false.
     */
    virtual explicit bool operator bool() const final
    {
      return this->_value == 0;
    }
  };
  
  template<typename NumT>
  BasicClampedNumber<NumT> operator+(BasicClampedNumber<NumT> lhs, const NumT &rhs)
  {
    return (lhs += rhs);
  }
  
  template<typename NumT>
  BasicClampedNumber<NumT> operator-(BasicClampedNumber<NumT> lhs, const NumT &rhs)
  {
    return (lhs -= rhs);
  }
  
  template<typename NumT>
  BasicClampedNumber<NumT> operator*(BasicClampedNumber<NumT> lhs, const NumT &rhs)
  {
    return (lhs *= rhs);
  }
  
  template<typename NumT>
  BasicClampedNumber<NumT> operator/(BasicClampedNumber<NumT> lhs, const NumT &rhs)
  {
    return (lhs /= rhs);
  }
  
  template<typename IntT>
  class ClampedInteger: public BasicClampedNumber<IntT>
  {
    public:
    
    ClampedInteger(const IntT &value, IntT min, IntT max) :
        BasicClampedNumber<IntT>(value, min, max)
    {
    }
    virtual ~ClampedInteger() = default;
    
    virtual ClampedInteger<IntT> & operator%=(const IntT &);
  };
}
