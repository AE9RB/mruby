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
