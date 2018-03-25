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
    
    BasicClampedNumber() = 0;
    BasicClampedNumber(const NumT &) = 0;
    BasicClampedNumber(const NumT &, const NumT &, const NumT &) = 0;
    virtual ~BasicClampedNumber() = 0;
    
    BasicClampedNumber(const BasicClampedNumber<NumT> &) = 0;
    BasicClampedNumber(BasicClampedNumber<NumT> &&) = 0;
    virtual BasicClampedNumber<NumT> operator=(const BasicClampedNumber<NumT> &) = 0;
    virtual BasicClampedNumber<NumT> & operator=(BasicClampedNumber<NumT> &) = 0;
    
    virtual NumT & value() const final { return this->_value; }
    virtual NumT & maxValue() const final { return this->_maxValue; }
    virtual NumT & minValue() const final { return this->_minValue; }
    virtual NumT & value(const NumT &newVal) final { return (this->_value = newVal); }
    virtual NumT & maxValue(const NumT &newMax) final { return (this->_maxValue = newMax); }
    virtual NumT & minValue(const NumT &newMin) final { return (this->_maxValue = newMin); }
    
    virtual NumT & operator+=(const BasicClampedNumber<NumT> &) = 0;
    virtual NumT & operator-=(const BasicClampedNumber<NumT> &) = 0;
    virtual NumT & operator*=(const BasicClampedNumber<NumT> &) = 0;
    virtual NumT & operator/=(const BasicClampedNumber<NumT> &) = 0;
    
    virtual NumT & operator++() = 0;
    virtual NumT & operator--() = 0;
    virtual NumT & operator++(int) = 0;
    virtual NumT & operator--(int) = 0;
    
    virtual bool operator==(const BasicClampedNumber<NumT> &) const = 0;
    virtual bool operator<(const BasicClampedNumber<NumT> &)  const = 0;
    virtual bool operator!=(const BasicClampedNumber<NumT> &other) const { return !(*this == other); }
    virtual bool operator<=(const BasicClampedNumber<NumT> &other) const { return *this < other || *this == other; }
    virtual bool operator>(const BasicClampedNumber<NumT> &other)  const { return !(*this <= other); }
    virtual bool operator>=(const BasicClampedNumber<NumT> &other) const { return !(*this < other); }
    
    virtual NumT operator NumT() { return this->_value; }
  };
  
  template<typename IntT>
  class ClampedInteger:
      public BasicClampedNumber<IntT>
  {
    
  };
}
