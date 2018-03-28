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
    
    BasicClampedNumber() = delete;
    BasicClampedNumber(const NumT &value, const NumT &min, const NumT &max):
      _value(value), _minValue((min <= value) ? min : value), _maxValue((max >= value) ? max : value) {}
    virtual ~BasicClampedNumber() = default;
    
    BasicClampedNumber(const BasicClampedNumber<NumT> &) = default;
    BasicClampedNumber(BasicClampedNumber<NumT> &&) = default;
    virtual BasicClampedNumber<NumT> operator=(const BasicClampedNumber<NumT> &) = default;
    virtual BasicClampedNumber<NumT> & operator=(BasicClampedNumber<NumT> &) = default;
    
    virtual const NumT & value() const final { return this->_value; }
    virtual const NumT & maxValue() const final { return this->_maxValue; }
    virtual const NumT & minValue() const final { return this->_minValue; }
    virtual const NumT & value(const NumT &newVal) final;
    virtual const NumT & maxValue(const NumT &newMax) final;
    virtual const NumT & minValue(const NumT &newMin) final;
    
    virtual BasicClampedNumber<NumT> & operator+=(const NumT &);
    virtual BasicClampedNumber<NumT> & operator-=(const NumT &);
    virtual BasicClampedNumber<NumT> & operator*=(const NumT &);
    virtual BasicClampedNumber<NumT> & operator/=(const NumT &);
    
    virtual BasicClampedNumber<NumT> & operator++();
    virtual BasicClampedNumber<NumT> & operator--();
    virtual BasicClampedNumber<NumT> operator++(int);
    virtual BasicClampedNumber<NumT> operator--(int);
    
    virtual bool operator==(const BasicClampedNumber<NumT> &other) const { return this->_value == other._value; }
    virtual bool operator< (const BasicClampedNumber<NumT> &other) const { return this->_value < other._value; }
    virtual bool operator!=(const BasicClampedNumber<NumT> &other) const { return !(*this == other); }
    virtual bool operator<=(const BasicClampedNumber<NumT> &other) const { return *this < other || *this == other; }
    virtual bool operator> (const BasicClampedNumber<NumT> &other) const { return !(*this <= other); }
    virtual bool operator>=(const BasicClampedNumber<NumT> &other) const { return !(*this < other); }
    
    virtual explicit NumT operator NumT() const final { return this->_value; }
    virtual explicit bool operator bool() const final { return this->_value == 0; }
  };
  
  template<typename NumT>
  BasicClampedNumber<NumT> operator+(BasicClampedNumber<NumT> lhs, const NumT &rhs)
  { return (lhs += rhs); }
  
  template<typename NumT>
  BasicClampedNumber<NumT> operator-(BasicClampedNumber<NumT> lhs, const NumT &rhs)
  { return (lhs -= rhs); }
  
  template<typename NumT>
  BasicClampedNumber<NumT> operator*(BasicClampedNumber<NumT> lhs, const NumT &rhs)
  { return (lhs *= rhs); }
  
  template<typename NumT>
  BasicClampedNumber<NumT> operator/(BasicClampedNumber<NumT> lhs, const NumT &rhs)
  { return (lhs /= rhs); }
  
  template<typename IntT>
  class ClampedInteger: public BasicClampedNumber<IntT>
  {
    public:
    
    ClampedInteger(const IntT &value, IntT min, IntT max):
        BasicClampedNumber<IntT>(value, min, max) {}
    virtual ~ClampedInteger() = default;
    
    virtual ClampedInteger<IntT> & operator%=(const IntT &);
  };
}
