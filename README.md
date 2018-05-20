# C++ Clamped Numbers

The C++ Clamped Numbers API provides wrappers for numeric types like `int` and `double` which constrain their values to within specified bounds. Custom numeric types (such as one representing roman numeral values) can also be used with these wrappers provide they implement the necessary operators.

There are three class templates for wrapping different types of numbers. `ClampedNaturalNumber` is designed to wrap unsigned integral types like `size_t` and corresponds with the set of natural numbers (including zero), ℕ. `ClampedInteger` is designed to wrap signed integral types like `int` amd corresponds with the set of integers, ℤ. Lastly, `ClampedDecimal` is designed to wrap floating-point types like `double` and corresponds with the set of all real numbers, ℝ.
