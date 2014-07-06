assert("Complex().kind_of?") do
  assert_kind_of Numeric, Complex()
  assert_kind_of Complex, Complex()
end

assert("Complex() varargs") do
  assert_equal 0, Complex().real
  assert_equal 0, Complex().imag
  assert_equal 12.34, Complex(12.34).real
  assert_equal 0, Complex(12.34).imag
  assert_equal 12.34, Complex(12.34, 56.78).real
  assert_equal 56.78, Complex(12.34, 56.78).imag
end

assert("Complex() equality") do
  assert_not_equal Complex(0/0, 0/0), Complex(0/0, 0/0)
  assert_not_equal Complex(12.34, 0), Complex(12.34, 56.78)
  assert_not_equal Complex(12.34, 56.78), Complex(0, 56.78)
  assert_equal Complex(12.34, 56.78), Complex(12.34, 56.78)
  assert_equal 12.34, Complex(12.34)
  assert_equal 42, Complex(42)
  assert_equal Complex(12.34), 12.34
  assert_equal Complex(42), 42
end

assert("Complex() addition") do
  assert_equal Complex(8,10), Complex(3,4) + Complex(5,6)
  assert_equal Complex(3.6,4), Complex(3.1,4) + 0.5
  assert_equal Complex(8.1,4), Complex(3.1,4) + 5
  assert_equal Complex(3.6,4), 0.5 + Complex(3.1,4)
  assert_equal Complex(8.1,4), 5 + Complex(3.1,4)
end

assert("Complex() subtraction") do
  assert_equal Complex(1,3), Complex(5,6) - Complex(4,3)
  assert_equal Complex(2.6,4), Complex(3.1,4) - 0.5
  assert_equal Complex(-1.9,4), Complex(3.1,4) - 5
  assert_equal Complex(-2.6,4), 0.5 - Complex(3.1,4)
  assert_equal Complex(1.9,4), 5 - Complex(3.1,4)
end

assert("Complex() multiplication") do
  assert_equal Complex(2,39), Complex(5,6) * Complex(4,3)
  assert_equal Complex(1.55,2), Complex(3.1,4) * 0.5
  assert_equal Complex(15.5,20), Complex(3.1,4) * 5
  assert_equal Complex(1.55,2), 0.5 * Complex(3.1,4)
  assert_equal Complex(15.5,20), 5 * Complex(3.1,4)
end
