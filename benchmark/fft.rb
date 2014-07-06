def dit data, is
  # Solve for 2**ldn == data.size
  ldn = 1 
  nn = data.size.to_f
  ldn += 1 while (nn /= 2) > 1
  raise "data.size must be power of 2" unless nn == 1.0
  raise "is must be 1 or -1" unless is.abs == 1
  s2pi = is>0 ? 2.0*Math::PI : -2.0*Math::PI
  ldm = ldn & 1
  if ldm != 0 # Perform Radix-2 step when not power of 4
    0.step(data.size-1, 2) do |r|
      a0 = data[r];
      a1 = data[r+1];
      data[r] = a0 + a1;
      data[r+1] = a0 - a1;
    end
  end
  ldm += 2;
  while ldm <= ldn
    m = 1<<ldm
    ldm += 2
    m4 = m>>2
    ph0 = s2pi/m
    (0...m4).each do |j|
      phi = j*ph0
      e = Complex(Math::cos(phi), Math::sin(phi))
      e2 = Complex(Math::cos(phi*2), Math::sin(phi*2))
      e3 = Complex(Math::cos(phi*3), Math::sin(phi*3))
      i0 = j
      0.step(data.size-1, m) do |r|
        i1 = i0+m4
        i2 = i1+m4
        i3 = i2+m4
        a0 = data[i0]
        a2 = data[i1]
        a1 = data[i2]
        a3 = data[i3]
        a1 *= e;
        a2 *= e2
        a3 *= e3
        t0 = (a0+a2) + (a1+a3)
        t2 = (a0+a2) - (a1+a3)
        t1 = (a0-a2) + Complex(0,is) * (a1-a3)
        t3 = (a0-a2) - Complex(0,is) * (a1-a3)
        data[i0] = t0
        data[i1] = t1
        data[i2] = t2
        data[i3] = t3
        i0 += m
      end
    end
  end
end

def bitrev data
  j = 0
  data.each_index do |i|
    if j > i
      data[i], data[j] = data[j], data[i]
    end
    m = data.size >> 1
    while (m>=1 && j>=m) do
      j -= m
      m >>= 1
    end
    j += m
  end
end

a = Array.new(8192,0)
a[1] = 1
t0 = Time.now
bitrev a
dit a, -1
t1 = Time.now
puts "fft: #{(t1-t0) * 1000}ms"
