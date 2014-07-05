##
# complex.rb Complex class
# See Copyright Notice in mruby.h

class Complex
  
  def to_s
    "(#{real}+#{imag}i)"
  end

  alias :inspect :to_s
  
end
