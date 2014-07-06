##
# complex.rb Complex class
# See Copyright Notice in mruby.h

class Complex

  def to_s
    "#{real}#{(imag >= 0 || imag.nan?) ? "+#{imag}" : imag}#{"*" if !imag.finite?}i"
  end

  def inspect
    "(#{to_s})"
  end

end
