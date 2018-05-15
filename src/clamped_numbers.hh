#pragma once

#include <cstdint>

#include <limits>

#if !defined(CLAMPED_INT8) && defined(INT8_MAX) && defined(INT8_MIN)
#define CLAMPED_INT8
#endif

#if !defined(CLAMPED_INT16) && defined(INT16_MAX) && defined(INT16_MIN)
#define CLAMPED_INT16
#endif

#if !defined(CLAMPED_INT32) && defined(INT32_MAX) && defined(INT32_MIN)
#define CLAMPED_INT32
#endif

#if !defined(CLAMPED_INT64) && defined(INT64_MAX) && defined(INT64_MIN)
#define CLAMPED_INT64
#endif

#if !defined(CLAMPED_UINT8) && defined(UINT8_MAX)
#define CLAMPED_UINT8
#endif

#if !defined(CLAMPED_UINT16) && defined(UINT16_MAX)
#define CLAMPED_UINT16
#endif

#if !defined(CLAMPED_UINT32) && defined(UINT32_MAX)
#define CLAMPED_UINT32
#endif

#if !defined(CLAMPED_UINT64) && defined(UINT64_MAX)
#define CLAMPED_UINT64
#endif

namespace clamped
{
  /**
   * A generic number with defined lower and upper bounds beyond which its value
   * will never pass. The type parameter for this class determines the numeric
   * type which is kept within these bounds. Clamped numbers are designed with
   * the built-in types such as `int`, `double`, and `size_t` in mind, though
   * any numeric type with suitable operators defined can be substituted as the
   * type parameter.
   * 
   * Numeric types used in a `BasicClampedNumber` must, in general, implement
   * the same operators as a float. This includes the four basic arithmetic
   * operators (+, -, *, /, +=, -=, *=, /=) and the comparison operators
   * (==, !=, <, <=, >, >=), each with an `int` as the right operand. For
   * example, it must be possible to ask whether `num == 0`. A 
   * `BasicClampedNumber` does not have a concept of a sign, and hence the
   * numeric type it wraps does not necessarily need to implement the
   * unary - operator.
   * 
   * While `BasicClampedNumber` is technically a fully concrete class and has
   * all the necessary operators to wrap a floating-point type such as `double`,
   * direct instantiation is not reccomended, at least not for the built-in
   * types. For floating-point types, `ClampedDecimal` is instead reccomended.
   * 
   * \param NumT the numeric type being bounded
   * 
   * \see ClampedNaturalNumber ClampedInteger ClampedDecimal
   */
  template<typename NumT>
  class BasicClampedNumber
  {
    protected:
    
    NumT _value;
    NumT _minValue;
    NumT _maxValue;

    public:
    
    /**
     * The default constructor is disabled. A `BasicClampedNumber` is
     * sufficiently general that no reasonable default values can be generated.
     */
    BasicClampedNumber() = delete;
    
    /**
     * Constructs a new `BasicClampedNumber` with the specified current,
     * minimum, and maximum values. The minimum value must be less than or equal
     * to the starting value: if it is not, it is itself clamped to the starting
     * value. The maximum value is similarly constrained, and must be greater
     * than or equal to the starting value.
     * 
     * \param value the starting value of this number
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
     * function, therefore, `value() == maxValue()`.
     * 
     * \return Returns this number's current value after modification.
     */
    virtual const NumT & minimize() final
    {
      return (this->_value = this->_minValue);
    }
    
    /**
     * Sets this number's current value to its minimum. After calling this
     * function, therefore, `value() == minValue()`.
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
     * Returns a new `BasicClampedNumber` with a value equal to that of the
     * original, plus the given number, within the clamped number's bounds. For
     * example, given a number with value 5 and bounds [0, 10], num + 10 returns
     * a new number with value 10 and bounds [0, 10].
     * 
     * \param other the number added onto this one
     * \return Returns the sum of this and the other number.
     */
    virtual BasicClampedNumber<NumT> operator+(const NumT &other) const
        {
      BasicClampedNumber<NumT> sum(*this);
      sum += other;
      return sum;
    }
    
    /**
     * Returns a new `BasicClampedNumber` with a value equal to that of the
     * original, minus the given number, within the clamped nunber's bounds. For
     * example, given a number with value 5 and bounds [0, 10], num - 10 returns
     * a new number with value 0 and bounds [0, 10].
     * 
     * \param other the number subtracted from this one
     * \return Returns the difference of this and the other number.
     */
    virtual BasicClampedNumber<NumT> operator-(const NumT &other) const
        {
      BasicClampedNumber<NumT> difference(*this);
      difference -= other;
      return difference;
    }
    
    /**
     * Returns a new `BasicClampedNumber` with a value equal to that of the
     * original, multiplied by the given number, within the clamped nunber's
     * bounds. For example, given a number with value 10 and bounds [0, 50],
     * num * 10 returns a new number with value 50 and bounds [0, 50].
     * 
     * \param other the number by which this one is multiplied
     * \return Returns the product of this and the other number.
     */
    virtual BasicClampedNumber<NumT> operator*(const NumT &other) const
        {
      BasicClampedNumber<NumT> product(*this);
      product *= other;
      return product;
    }
    
    /**
     * Returns a new `BasicClampedNumber` with a value equal to that of the
     * original, divided by the given number, within the clamped nunber's
     * bounds. For example, given a number with value 50 and bounds [25, 100],
     * num / 10 returns a new number with value 25 and bounds [25, 100].
     * 
     * \param other the number by which this one is divided
     * \return Returns the quotient of this and the other number.
     */
    virtual BasicClampedNumber<NumT> operator/(const NumT &other) const
        {
      BasicClampedNumber<NumT> quotient(*this);
      quotient /= other;
      return quotient;
    }
    
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
     * Allows the excplicit conversion of this `BasicClampedNumber` to an
     * instance of `NumT`, the numerical type it wraps. This conversion is
     * similar to calling `value()`, but it returns a copy instead of a
     * constant reference.
     * 
     * \return Returns a copy of this number's internal value.
     */
    virtual explicit operator NumT() const final
    {
      return this->_value;
    }
    
    /**
     * Allows the explicit conversion of this `BasicClampedNumber` to a boolean
     * value. This is equivalent to asking whether `NumT(*this) == 0`. As the
     * bounds of some numbers may forbid their equalling zero, these numbers
     * will always convert to false.
     * 
     * \return Returns true if this number equals zero, else false.
     */
    virtual explicit operator bool() const final
    {
      return this->_value == 0;
    }
  };
  
  /**
   * A natural number with defined lower and upper bounds beyond which its value
   * will never pass. As compared to the very general `BasicClampedNumber`, a
   * `ClampedNaturalNumber` is designed to wrap an unsigned integral numerical
   * type, such as a `size_t` or `unsigned long`.
   * 
   * Numeric types wrapped in a `ClampedNaturalNumber` must - as well as the
   * arithmetic and coparison operators required for all `BasicClampedNumber`s -
   * implement the remainder operator (%, %=).
   * 
   * If representation of negative numbers is needed, `ClampedInteger` should be
   * preffered over this type. A `ClampedNaturalNumber` corresponds with the
   * mathematical concept of natural numbers (including zero), wheras a
   * `ClampedInteger` corresponds with the superset thereof, of all integers.
   * 
   * \param NatT the unsigned integral type being wrapped
   * 
   * \see ClampedInteger
   */
  template<typename NatT>
  class ClampedNaturalNumber: public BasicClampedNumber<NatT>
  {
    public:
    
    /**
     * Constructs a new `ClampedNaturalNumber` with the specified current,
     * minimum, and maximum values. The minimum value must be less than or equal
     * to the starting value: if it is not, it is itself clamped to the starting
     * value. The maximum value is similarly constrained, and must be greater
     * than or equal to the starting value.
     * 
     * \param value the starting value of this number
     * \param min the minimum value for this number
     * \param max the maximum value for this number
     */
    ClampedNaturalNumber(const NatT &value, NatT min, NatT max) :
        BasicClampedNumber<NatT>(value, min, max)
    {
    }
    
    /**
     * Provides a virtual destructor with the default dehavior.
     */
    virtual ~ClampedNaturalNumber() = default;
    
    virtual ClampedNaturalNumber<NatT> & operator+=(const NatT &other) override;

    virtual ClampedNaturalNumber<NatT> & operator-=(const NatT &other) override;

    virtual ClampedNaturalNumber<NatT> & operator*=(const NatT &other) override;

    virtual ClampedNaturalNumber<NatT> & operator/=(const NatT &other) override;

    /**
     * Sets this number's value to the remainder of division by the given
     * number, within this number's bounds. Notably, division by zero will yield
     * a remainder of zero, rather than having undefined behavior.
     * 
     * \param other the value by which to divide this one
     * \return Returns this number, allowing chain of operations.
     */
    virtual ClampedNaturalNumber<NatT> & operator%=(const NatT &other);

    /**
     * Returns a new `ClampedNaturalNumber` holding the remainder of the
     * division of the original by the given number, within this number's
     * bounds. For example, given a number with value 7 and bounds [5, 10],
     * num % 2 returns a new number with value 5 and bounds [5, 10]. Notably, if
     * the right operand is zero then the remainder will be zero, rather than
     * being undefined.
     * 
     * \param other the number from which this one's remainder is found
     * \return Returns the remainder of dividing this number by the other.
     */
    virtual ClampedNaturalNumber<NatT> operator%(const NatT &other) const
        {
      ClampedNaturalNumber<NatT> remainder(*this);
      remainder %= other;
      return remainder;
    }
  };
  
  /**
   * An integer with defined lower and upper bounds beyond which its value will
   * never pass. A clamped integer corresponds with signed integral types such
   * as `int` and `long`, and the mathematical set of all integers. Taken
   * together, `ClampedInteger` and `ClampedDecimal` are named to draw a
   * parallel to Java's `BigInteger` and `BigDecimal` types.
   * 
   * In addition  to those implemented by the types a `ClampedNaturalNumber`
   * wraps, the numeric type contained by a `ClampedInteger` must also implement
   * the unary - operator. In short, the wrapped type must therefore have all
   * the same operators as an `int`, except for the bitwise ones.
   * 
   * \see ClampedDecimal
   */
  template<typename IntT>
  class ClampedInteger: public ClampedNaturalNumber<IntT>
  {
    public:
    
    /**
     * Constructs a new `ClampedInteger` with the specified current, minimum,
     * and maximum values. The minimum value must be less than or equal to the
     * starting value: if it is not, it is itself clamped to the starting value.
     * The maximum value is similarly constrained, and must be greater than or
     * equal to the starting value.
     * 
     * \param value the starting value of this number
     * \param min the minimum value for this number
     * \param max the maximum value for this number
     */
    ClampedInteger(const IntT &value, const IntT &min, const IntT &max) :
        ClampedNaturalNumber<IntT>(value, min, max)
    {
    }
    
    /**
     * Provides a virtual destructor with the default dehavior.
     */
    virtual ~ClampedInteger() = default;
    
    virtual ClampedInteger<IntT> & operator+=(const IntT &other);

    virtual ClampedInteger<IntT> & operator-=(const IntT &other);

    virtual ClampedInteger<IntT> & operator*=(const IntT &other);

    virtual ClampedInteger<IntT> & operator/=(const IntT &other);

    virtual ClampedInteger<IntT> & operator%=(const IntT &other);

    /**
     * Returns the negative of this number. The value is negated, but the
     * mimimum and maximum will be unchanged, except where they are stretched to
     * fit the new value, in line with the constructors
     * of `BasicClampedNumber`s.
     * 
     * \return Returns the negation of this number.
     */
    virtual ClampedInteger<IntT> operator-() const
    {
      IntT negVal = -(this->_value);
      return {negVal, this->_minValue, this->_maxValue};
    }
  };
  
  /**
   * A real number with defined lower and upper bounds beyond which its value
   * will never pass. A `ClampedDecimal` corresponds with floating-point types
   * such as `float` and `double`, and with the mathematical set of all real
   * numbers. Taken together, `ClampedInteger` and `ClampedDecimal` are named to
   * draw a parallel to Java's `BigInteger` and `BigDecimal` types.
   * 
   * In addition  to those implemented by the types a `BasicClampedNumber`
   * wraps, the numeric type contained by a `ClampedDecimal` must also implement
   * the unary - operator.
   * 
   * \see ClampedInteger
   */
  template<typename FloatT>
  class ClampedDecimal: public BasicClampedNumber<FloatT>
  {
    public:
    
    /**
     * Constructs a new `ClampedDecimal` with an initial value of zero and
     * bounds [-1, 1]. This default construction can be thought of as a
     * "normalized" real number.
     */
    ClampedDecimal() :
        BasicClampedNumber<FloatT>(0, -1, 1)
    {
    }
    
    /**
     * Constructs a new ClampedDecimal with the specified current, minimum, and
     * maximum values. The minimum value must be less than or equal to the
     * starting value: if it is not, it is itself clamped to the starting value.
     * The maximum value is similarly constrained, and must be greater than or
     * equal to the starting value.
     * 
     * \param value the starting value of this number
     * \param min the minimum value for this number
     * \param max the maximum value for this number
     */
    ClampedDecimal(const FloatT &value, const FloatT &min, const FloatT &max) :
        BasicClampedNumber<FloatT>(value, min, max)
    {
    }
    
    /**
     * Provides a virtual destructor with the default dehavior.
     */
    virtual ~ClampedDecimal() = default;
    
    /**
     * Returns the negative of this number. The value is negated, but the
     * mimimum and maximum will be unchanged, except where they are stretched to
     * fit the new value, in line with the constructors
     * of `BasicClampedNumber`s.
     * 
     * \return Returns the negation of this number.
     */
    virtual ClampedDecimal<FloatT> operator-() const
    {
      FloatT negVal = -(this->_value);
      return {negVal, this->_minValue, this->_maxValue};
    }
  };
  
  /**
   * An `int` with defined lower and upper bounds beyond which its value will
   * never pass.
   */
  template<>
  class ClampedInteger<int>
  {
    public:
    
    /**
     * Constructs a new `ClampedStdInt` with an initial value of zero and no
     * bounds. With these default bounds left intact, this number will thus act
     * like an `int` that does not overflow nor underflow when the maximum or
     * minimum value it can represent is exceeded.
     */
    ClampedInteger<int>() :
        ClampedInteger(0, std::numeric_limits<int>::min(), std::numeric_limits<int>::max())
    {
    }
    
    /**
     * Constructs a new `ClampedStdInt` with the given initial value and no
     * bounds. With these default bounds left intact, this number will thus act
     * like an `int` that does not overflow nor underflow when the maximum or
     * minimum value it can represent is exceeded.
     * 
     * \param value the starting value of this number
     */
    ClampedInteger<int>(int value) :
        ClampedInteger(value, std::numeric_limits<int>::min(), std::numeric_limits<int>::max())
    {
    }
    
    /**
     * Constructs a new `ClampedStdInt` with the given initial value and the
     * specified lower and upper bounds. The minimum value must be less than or
     * equal to the starting value: if it is not, it is itself clamped to the
     * starting value. The maximum value is similarly constrained, and must be
     * greater than or equal to the starting value.
     * 
     * \param value the starting value of this number
     * \param min the minimum value for this number
     * \param max the maximum value for this number
     */
    ClampedInteger<int>(int value, int min, int max) :
        ClampedInteger(value, min, max)
    {
    }
    
    /**
     * Provides a virtual destructor with the default dehavior.
     */
    virtual ~ClampedInteger<int>() = default;
  };
  
  /**
   * An `unsigned int` with defined lower and upper bounds beyond which its
   * value will never pass.
   */
  template<>
  class ClampedNaturalNumber<unsigned int>
  {
    public:
    
    /**
     * Constructs a new `ClampedStdUInt` with an initial value of zero and no
     * bounds. With these default bounds left intact, this number will thus act
     * like an `unsigned int` that does not overflow nor underflow when the
     * maximum or minimum value it can represent is exceeded.
     */
    ClampedNaturalNumber<unsigned int>() :
        ClampedNaturalNumber(0, std::numeric_limits<unsigned int>::min(), std::numeric_limits<unsigned int>::max())
    {
    }
    
    /**
     * Constructs a new `ClampedStdUInt` with the given initial value and no
     * bounds. With these default bounds left intact, this number will thus act
     * like an `int` that does not overflow nor underflow when the maximum or
     * minimum value it can represent is exceeded.
     * 
     * \param value the starting value of this number
     */
    ClampedNaturalNumber<unsigned int>(unsigned int value) :
        ClampedNaturalNumber(value, std::numeric_limits<unsigned int>::min(), std::numeric_limits<unsigned int>::max())
    {
    }
    
    /**
     * Constructs a new `ClampedStdUInt` with the given initial value and the
     * specified lower and upper bounds. The minimum value must be less than or
     * equal to the starting value: if it is not, it is itself clamped to the
     * starting value. The maximum value is similarly constrained, and must be
     * greater than or equal to the starting value.
     * 
     * \param value the starting value of this number
     * \param min the minimum value for this number
     * \param max the maximum value for this number
     */
    ClampedNaturalNumber<unsigned int>(unsigned int value, unsigned int min, unsigned int max) :
        ClampedNaturalNumber(value, min, max)
    {
    }
    
    /**
     * Provides a virtual destructor with the default dehavior.
     */
    virtual ~ClampedNaturalNumber<unsigned int>() = default;
  };
  
  /**
   * A signed integer of the largest type supported with defined lower and upper
   * bounds beyond which its value will never pass. In other words, this class
   * is a type specialization of `ClampedInteger` for `intmax_t` in `<cstdint>`.
   */
  template<>
  class ClampedInteger<intmax_t>
  {
    public:
    
    /**
     * Constructs a new `ClampedMaxInt` with an initial value of zero and no
     * bounds. With these default bounds left intact, this number will thus act
     * like an `intmax_t` that does not overflow nor underflow when the maximum
     * or minimum value it can represent is exceeded.
     */
    ClampedInteger<intmax_t>() :
        ClampedInteger(0, std::numeric_limits<intmax_t>::min(), std::numeric_limits<intmax_t>::max())
    {
    }
    
    /**
     * Constructs a new `ClampedMaxInt` with the given initial value and no
     * bounds. With these default bounds left intact, this number will thus act
     * like an `intmax_t` that does not overflow nor underflow when the maximum
     * or minimum value it can represent is exceeded.
     * 
     * \param value the starting value of this number
     */
    ClampedInteger<intmax_t>(intmax_t value) :
        ClampedInteger(value, std::numeric_limits<intmax_t>::min(), std::numeric_limits<intmax_t>::max())
    {
    }
    
    /**
     * Constructs a new `ClampedMaxInt` with the given initial value and the
     * specified lower and upper bounds. The minimum value must be less than or
     * equal to the starting value: if it is not, it is itself clamped to the
     * starting value. The maximum value is similarly constrained, and must be
     * greater than or equal to the starting value.
     * 
     * \param value the starting value of this number
     * \param min the minimum value for this number
     * \param max the maximum value for this number
     */
    ClampedInteger<intmax_t>(intmax_t value, intmax_t min, intmax_t max) :
        ClampedInteger(value, min, max)
    {
    }
    
    /**
     * Provides a virtual destructor with the default dehavior.
     */
    virtual ~ClampedInteger<intmax_t>() = default;
  };
  
  /**
   * An unsigned signed integer of the largest type supported with defined lower
   * and upper bounds beyond which its value will never pass. In other words,
   * this class is a type specialization of `ClampedInteger` for `uintmax_t`
   * in `<cstdint>`.
   */
  template<>
  class ClampedNaturalNumber<uintmax_t>
  {
    public:
    
    /**
     * Constructs a new `ClampedMaxUInt` with an initial value of zero and no
     * bounds. With these default bounds left intact, this number will thus act
     * like a `uintmax_t` that does not overflow nor underflow when the maximum
     * or minimum value it can represent is exceeded.
     */
    ClampedNaturalNumber<uintmax_t>() :
        ClampedNaturalNumber(0, std::numeric_limits<uintmax_t>::min(), std::numeric_limits<uintmax_t>::max())
    {
    }
    
    /**
     * Constructs a new `ClampedMaxUInt` with the given initial value and no
     * bounds. With these default bounds left intact, this number will thus act
     * like a `uintmax_t` that does not overflow nor underflow when the maximum
     * or minimum value it can represent is exceeded.
     * 
     * \param value the starting value of this number
     */
    ClampedNaturalNumber<uintmax_t>(uintmax_t value) :
        ClampedNaturalNumber(value, std::numeric_limits<uintmax_t>::min(), std::numeric_limits<uintmax_t>::max())
    {
    }
    
    /**
     * Constructs a new `ClampedMaxUInt` with the given initial value and the
     * specified lower and upper bounds. The minimum value must be less than or
     * equal to the starting value: if it is not, it is itself clamped to the
     * starting value. The maximum value is similarly constrained, and must be
     * greater than or equal to the starting value.
     * 
     * \param value the starting value of this number
     * \param min the minimum value for this number
     * \param max the maximum value for this number
     */
    ClampedNaturalNumber<uintmax_t>(uintmax_t value, uintmax_t min, uintmax_t max) :
        ClampedNaturalNumber(value, min, max)
    {
    }
    
    /**
     * Provides a virtual destructor with the default dehavior.
     */
    virtual ~ClampedNaturalNumber<uintmax_t>() = default;
  };
  
# ifdef CLAMPED_INT8
  /**
   * A signed integer exactly 8 bits in size and with defined lower and upper
   * bounds beyond which its value will never pass. This type wil only be
   * defined if `int8_t` from `<cstdint>`, upon which it relies, also exists. If
   * so, the macro `CLAMPED_INT8` will also be defined.
   */
  class ClampedInt8: public ClampedInteger<int8_t>
  {
    public:
    
    /**
     * Constructs a new `ClampedInt8` with an initial value of zero and no
     * bounds. With these default bounds left intact, this number will thus act
     * like an `int8_t` that does not overflow nor underflow when the maximum
     * or minimum value it can represent is exceeded.
     */
    ClampedInt8() :
        ClampedInteger<int8_t>(0, std::numeric_limits<int8_t>::min(), std::numeric_limits<int8_t>::max())
    {
    }
    
    /**
     * Constructs a new `ClampedInt8` with the given initial value and no
     * bounds. With these default bounds left intact, this number will thus act
     * like a `int8_t` that does not overflow nor underflow when the maximum or
     * minimum value it can represent is exceeded.
     * 
     * \param value the starting value of this number
     */
    ClampedInt8(int8_t value) :
        ClampedInteger<int8_t>(value, std::numeric_limits<int8_t>::min(), std::numeric_limits<int8_t>::max())
    {
    }
    
    /**
     * Constructs a new `ClampedInt8` with the given initial value and the
     * specified lower and upper bounds. The minimum value must be less than or
     * equal to the starting value: if it is not, it is itself clamped to the
     * starting value. The maximum value is similarly constrained, and must be
     * greater than or equal to the starting value.
     * 
     * \param value the starting value of this number
     * \param min the minimum value for this number
     * \param max the maximum value for this number
     */
    ClampedInt8(int8_t value, int8_t min, int8_t max) :
        ClampedInteger<int8_t>(value, min, max)
    {
    }
    
    /**
     * Provides a virtual destructor with the default dehavior.
     */
    virtual ~ClampedInt8() = default;
  };
# endif
  
# ifdef CLAMPED_INT16
  /**
   * A signed integer exactly 16 bits in size and with defined lower and upper
   * bounds beyond which its value will never pass. This type wil only be
   * defined if `int16_t` from `<cstdint>`, upon which it relies, also exists.
   * If so, the macro `CLAMPED_INT16` will also be defined.
   */
  class ClampedInt16: public ClampedInteger<int16_t>
  {
    public:
    
    /**
     * Constructs a new `ClampedInt16` with an initial value of zero and no
     * bounds. With these default bounds left intact, this number will thus act
     * like an `int16_t` that does not overflow nor underflow when the maximum
     * or minimum value it can represent is exceeded.
     */
    ClampedInt16() :
        ClampedInteger(0, std::numeric_limits<int16_t>::min(), std::numeric_limits<int16_t>::max())
    {
    }
    
    /**
     * Constructs a new `ClampedInt16` with the given initial value and no
     * bounds. With these default bounds left intact, this number will thus act
     * like a `int16_t` that does not overflow nor underflow when the maximum or
     * minimum value it can represent is exceeded.
     * 
     * \param value the starting value of this number
     */
    ClampedInt16(int16_t value) :
        ClampedInteger(value, std::numeric_limits<int16_t>::min(), std::numeric_limits<int16_t>::max())
    {
    }
    
    /**
     * Constructs a new `ClampedInt16` with the given initial value and the
     * specified lower and upper bounds. The minimum value must be less than or
     * equal to the starting value: if it is not, it is itself clamped to the
     * starting value. The maximum value is similarly constrained, and must be
     * greater than or equal to the starting value.
     * 
     * \param value the starting value of this number
     * \param min the minimum value for this number
     * \param max the maximum value for this number
     */
    ClampedInt16(int16_t value, int16_t min, int16_t max) :
        ClampedInteger(value, min, max)
    {
    }
    
    /**
     * Provides a virtual destructor with the default dehavior.
     */
    virtual ~ClampedInt16() = default;
  };
# endif
  
# ifdef CLAMPED_INT32
  /**
   * A signed integer exactly 32 bits in size and with defined lower and upper
   * bounds beyond which its value will never pass. This type wil only be
   * defined if `int32_t` from `<cstdint>`, upon which it relies, also exists.
   * If so, the macro `CLAMPED_INT32` will also be defined.
   */
  class ClampedInt32: public ClampedInteger<int32_t>
  {
    public:
    
    /**
     * Constructs a new `ClampedInt32` with an initial value of zero and no
     * bounds. With these default bounds left intact, this number will thus act
     * like an `int32_t` that does not overflow nor underflow when the maximum
     * or minimum value it can represent is exceeded.
     */
    ClampedInt32() :
        ClampedInteger(0, std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max())
    {
    }
    
    /**
     * Constructs a new `ClampedInt32` with the given initial value and no
     * bounds. With these default bounds left intact, this number will thus act
     * like a `int32_t` that does not overflow nor underflow when the maximum or
     * minimum value it can represent is exceeded.
     * 
     * \param value the starting value of this number
     */
    ClampedInt32(int32_t value) :
        ClampedInteger(value, std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max())
    {
    }
    
    /**
     * Constructs a new `ClampedInt32` with the given initial value and the
     * specified lower and upper bounds. The minimum value must be less than or
     * equal to the starting value: if it is not, it is itself clamped to the
     * starting value. The maximum value is similarly constrained, and must be
     * greater than or equal to the starting value.
     * 
     * \param value the starting value of this number
     * \param min the minimum value for this number
     * \param max the maximum value for this number
     */
    ClampedInt32(int32_t value, int32_t min, int32_t max) :
        ClampedInteger(value, min, max)
    {
    }
    
    /**
     * Provides a virtual destructor with the default dehavior.
     */
    virtual ~ClampedInt32() = default;
  };
# endif
  
# ifdef CLAMPED_INT64
  /**
   * A signed integer exactly 64 bits in size and with defined lower and upper
   * bounds beyond which its value will never pass. This type wil only be
   * defined if `int164_t` from `<cstdint>`, upon which it relies, also exists.
   * If so, the macro `CLAMPED_INT64` will also be defined.
   */
  class ClampedInt64: public ClampedInteger<int64_t>
  {
    public:
    
    /**
     * Constructs a new `ClampedInt64` with an initial value of zero and no
     * bounds. With these default bounds left intact, this number will thus act
     * like an `int64_t` that does not overflow nor underflow when the maximum
     * or minimum value it can represent is exceeded.
     */
    ClampedInt64() :
        ClampedInteger(0, std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::max())
    {
    }
    
    /**
     * Constructs a new `ClampedInt64` with the given initial value and no
     * bounds. With these default bounds left intact, this number will thus act
     * like a `int64_t` that does not overflow nor underflow when the maximum or
     * minimum value it can represent is exceeded.
     * 
     * \param value the starting value of this number
     */
    ClampedInt64(int64_t value) :
        ClampedInteger(value, std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::max())
    {
    }
    
    /**
     * Constructs a new `ClampedInt64` with the given initial value and the
     * specified lower and upper bounds. The minimum value must be less than or
     * equal to the starting value: if it is not, it is itself clamped to the
     * starting value. The maximum value is similarly constrained, and must be
     * greater than or equal to the starting value.
     * 
     * \param value the starting value of this number
     * \param min the minimum value for this number
     * \param max the maximum value for this number
     */
    ClampedInt64(int64_t value, int64_t min, int64_t max) :
        ClampedInteger(value, min, max)
    {
    }
    
    /**
     * Provides a virtual destructor with the default dehavior.
     */
    virtual ~ClampedInt64() = default;
  };
# endif
  
# ifdef CLAMPED_UINT8
  /**
   * An unsigned integer exactly 8 bits in size and with defined lower and upper
   * bounds beyond which its value will never pass. This type wil only be
   * defined if `uint8_t` from `<cstdint>`, upon which it relies, also exists.
   * If so, the macro `CLAMPED_UINT8` will also be defined.
   */
  class ClampedUInt8: public ClampedNaturalNumber<uint8_t>
  {
    public:
    
    /**
     * Constructs a new `ClampedUInt8` with an initial value of zero and no
     * bounds. With these default bounds left intact, this number will thus act
     * like a `uint8_t` that does not overflow nor underflow when the maximum or
     * minimum value it can represent is exceeded.
     */
    ClampedUInt8() :
        ClampedNaturalNumber(0, std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint8_t>::max())
    {
    }
    
    /**
     * Constructs a new `ClampedUInt8` with the given initial value and no
     * bounds. With these default bounds left intact, this number will thus act
     * like a `uint8_t` that does not overflow nor underflow when the maximum or
     * minimum value it can represent is exceeded.
     * 
     * \param value the starting value of this number
     */
    ClampedUInt8(uint8_t value) :
        ClampedNaturalNumber(value, std::numeric_limits<uint8_t>::min(), std::numeric_limits<uint8_t>::max())
    {
    }
    
    /**
     * Constructs a new `ClampedUInt8` with the given initial value and the
     * specified lower and upper bounds. The minimum value must be less than or
     * equal to the starting value: if it is not, it is itself clamped to the
     * starting value. The maximum value is similarly constrained, and must be
     * greater than or equal to the starting value.
     * 
     * \param value the starting value of this number
     * \param min the minimum value for this number
     * \param max the maximum value for this number
     */
    ClampedUInt8(uint8_t value, uint8_t min, uint8_t max) :
        ClampedNaturalNumber(value, min, max)
    {
    }
    
    /**
     * Provides a virtual destructor with the default dehavior.
     */
    virtual ~ClampedUInt8() = default;
  };
# endif
  
# ifdef CLAMPED_UINT16
  /**
   * An unsigned integer exactly 16 bits in size and with defined lower and
   * upper bounds beyond which its value will never pass. This type wil only be
   * defined if `uint16_t` from `<cstdint>`, upon which it relies, also exists.
   * If so, the macro `CLAMPED_UINT16` will also be defined.
   */
  class ClampedUInt16: public ClampedNaturalNumber<uint16_t>
  {
    public:
    
    /**
     * Constructs a new `ClampedUInt16` with an initial value of zero and no
     * bounds. With these default bounds left intact, this number will thus act
     * like a `uint16_t` that does not overflow nor underflow when the maximum
     * or minimum value it can represent is exceeded.
     */
    ClampedUInt16() :
        ClampedNaturalNumber(0, std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint16_t>::max())
    {
    }
    
    /**
     * Constructs a new `ClampedUInt16` with the given initial value and no
     * bounds. With these default bounds left intact, this number will thus act
     * like a `uint16_t` that does not overflow nor underflow when the maximum
     * or minimum value it can represent is exceeded.
     * 
     * \param value the starting value of this number
     */
    ClampedUInt16(uint16_t value) :
        ClampedNaturalNumber(value, std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint16_t>::max())
    {
    }
    
    /**
     * Constructs a new `ClampedUInt16` with the given initial value and the
     * specified lower and upper bounds. The minimum value must be less than or
     * equal to the starting value: if it is not, it is itself clamped to the
     * starting value. The maximum value is similarly constrained, and must be
     * greater than or equal to the starting value.
     * 
     * \param value the starting value of this number
     * \param min the minimum value for this number
     * \param max the maximum value for this number
     */
    ClampedUInt16(uint16_t value, uint16_t min, uint16_t max) :
        ClampedNaturalNumber(value, min, max)
    {
    }
    
    /**
     * Provides a virtual destructor with the default dehavior.
     */
    virtual ~ClampedUInt16() = default;
  };
# endif
  
# ifdef CLAMPED_UINT32
  /**
   * An unsigned integer exactly 32 bits in size and with defined lower and
   * upper bounds beyond which its value will never pass. This type wil only be
   * defined if `uint32_t` from `<cstdint>`, upon which it relies, also exists.
   * If so, the macro `CLAMPED_UINT32` will also be defined.
   */
  class ClampedUInt32: public ClampedNaturalNumber<uint32_t>
  {
    public:
    
    /**
     * Constructs a new `ClampedUInt32` with an initial value of zero and no
     * bounds. With these default bounds left intact, this number will thus act
     * like a `uint32_t` that does not overflow nor underflow when the maximum
     * or minimum value it can represent is exceeded.
     */
    ClampedUInt32() :
        ClampedNaturalNumber(0, std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max())
    {
    }
    
    /**
     * Constructs a new `ClampedUInt32` with the given initial value and no
     * bounds. With these default bounds left intact, this number will thus act
     * like a `uint32_t` that does not overflow nor underflow when the maximum
     * or minimum value it can represent is exceeded.
     * 
     * \param value the starting value of this number
     */
    ClampedUInt32(uint32_t value) :
        ClampedNaturalNumber(value, std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max())
    {
    }
    
    /**
     * Constructs a new `ClampedUInt32` with the given initial value and the
     * specified lower and upper bounds. The minimum value must be less than or
     * equal to the starting value: if it is not, it is itself clamped to the
     * starting value. The maximum value is similarly constrained, and must be
     * greater than or equal to the starting value.
     * 
     * \param value the starting value of this number
     * \param min the minimum value for this number
     * \param max the maximum value for this number
     */
    ClampedUInt32(uint32_t value, uint32_t min, uint32_t max) :
        ClampedNaturalNumber(value, min, max)
    {
    }
    
    /**
     * Provides a virtual destructor with the default dehavior.
     */
    virtual ~ClampedUInt32() = default;
  };
# endif
  
# ifdef CLAMPED_UINT64
  /**
   * An unsigned integer exactly 64 bits in size and with defined lower and
   * upper bounds beyond which its value will never pass. This type wil only be
   * defined if `uint64_t` from `<cstdint>`, upon which it relies, also exists.
   * If so, the macro `CLAMPED_UINT64` will also be defined.
   */
  class ClampedUInt64: public ClampedNaturalNumber<uint64_t>
  {
    public:
    
    /**
     * Constructs a new `ClampedUInt64` with an initial value of zero and no
     * bounds. With these default bounds left intact, this number will thus act
     * like a `uint64_t` that does not overflow nor underflow when the maximum
     * or minimum value it can represent is exceeded.
     */
    ClampedUInt64() :
        ClampedNaturalNumber(0, std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::max())
    {
    }
    
    /**
     * Constructs a new `ClampedUInt64` with the given initial value and no
     * bounds. With these default bounds left intact, this number will thus act
     * like a `uint64_t` that does not overflow nor underflow when the maximum
     * or minimum value it can represent is exceeded.
     * 
     * \param value the starting value of this number
     */
    ClampedUInt64(uint64_t value) :
        ClampedNaturalNumber(value, std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::max())
    {
    }
    
    /**
     * Constructs a new `ClampedUInt64` with the given initial value and the
     * specified lower and upper bounds. The minimum value must be less than or
     * equal to the starting value: if it is not, it is itself clamped to the
     * starting value. The maximum value is similarly constrained, and must be
     * greater than or equal to the starting value.
     * 
     * \param value the starting value of this number
     * \param min the minimum value for this number
     * \param max the maximum value for this number
     */
    ClampedUInt64(uint64_t value, uint64_t min, uint64_t max) :
        ClampedNaturalNumber(value, min, max)
    {
    }
    
    /**
     * Provides a virtual destructor with the default dehavior.
     */
    virtual ~ClampedUInt64() = default;
  };
# endif
  
  using ClampedStdInt = ClampedInteger<int>;
  using ClampedStdUInt = ClampedNaturalNumber<unsigned int>;
  using ClampedMaxInt = ClampedInteger<intmax_t>;
  using ClampedMaxUInt = ClampedNaturalNumber<uintmax_t>;
}
