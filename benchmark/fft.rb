# Radix-4 Fast Fourier Transform
module DSP
  class FFT
    private
    @dft_twiddle = Array.new
    @idft_twiddle = Array.new
    @rev_pattern = Array.new
    
    # Solve for 2**n == data.size
    def self.calc_n data
      n = 1 
      nn = data.size.to_f
      n += 1 while (nn /= 2) > 1
      raise "data.size must be power of 2" unless nn == 1.0
      n
    end
    
    def self.make_twiddle twiddle, is, n
      s2pi = is>0 ? 2.0*Math::PI : -2.0*Math::PI
      ldm = (n & 1) + 2
      idx = 0
      while ldm <= n
        m = 1<<ldm
        ldm += 2
        m4 = m>>2
        ph0 = s2pi/m
        (1...m4).each do |j|
          phi = j*ph0
          twiddle[idx] = Complex(Math::cos(phi), Math::sin(phi))
          idx += 1
          twiddle[idx] = Complex(Math::cos(phi*2), Math::sin(phi*2))
          idx += 1
          twiddle[idx] = Complex(Math::cos(phi*3), Math::sin(phi*3))
          idx += 1
        end
      end
    end
  
    def self.dit twiddle, ldn, data, is
      last = data.size-1
      ldm = ldn & 1
      if ldm != 0 
        # Perform Radix-2 step when not power of 4
        0.step(last, 2) do |r|
          a0 = data[r]
          a1 = data[r+1]
          data[r] = a0 + a1
          data[r+1] = a0 - a1
        end
      end
      idx = 0
      ldm += 2
      while ldm <= ldn
        m = 1<<ldm
        ldm += 2
        m4 = m>>2
        i0 = 0
        0.step(last, m) do |r|
          i1 = i0+m4
          i2 = i1+m4
          i3 = i2+m4
          a0 = data[i0]
          a2 = data[i1]
          a1 = data[i2]
          a3 = data[i3]
          t0 = (a0+a2) + (a1+a3)
          t2 = (a0+a2) - (a1+a3)
          a1 -= a3
          if (is>0)
            a3 = Complex(-a1.imag,a1.real)
          else
            a3 = Complex(a1.imag,-a1.real)
          end
          t1 = (a0-a2) + a3
          t3 = (a0-a2) - a3
          data[i0] = t0
          data[i1] = t1
          data[i2] = t2
          data[i3] = t3
          i0 += m
        end
        (1...m4).each do |j|
          e1 = twiddle[idx]; idx += 1
          e2 = twiddle[idx]; idx += 1
          e3 = twiddle[idx]; idx += 1
          i0 = j
          0.step(last, m) do |r|
            i1 = i0+m4
            i2 = i1+m4
            i3 = i2+m4
            a0 = data[i0]
            a2 = data[i1] * e2
            a1 = data[i2] * e1
            a3 = data[i3] * e3
            t0 = (a0+a2) + (a1+a3)
            t2 = (a0+a2) - (a1+a3)
            a1 -= a3
            if (is>0)
              a3 = Complex(-a1.imag,a1.real)
            else
              a3 = Complex(a1.imag,-a1.real)
            end
            t1 = (a0-a2) + a3
            t3 = (a0-a2) - a3
            data[i0] = t0
            data[i1] = t1
            data[i2] = t2
            data[i3] = t3
            i0 += m
          end
        end
      end
    end

    def self.bitrev_inplace rev_pattern, n, data
      m = rev_pattern.size
      m2 = m * 2
      if (n&1)==0
        (0...m).each do |k|
          (0...k).each do |j|
            j1 = j + rev_pattern[k]
            k1 = k + rev_pattern[j]
            data[j1], data[k1] = data[k1], data[j1]
            j1 += m
            k1 += m2
            data[j1], data[k1] = data[k1], data[j1]
            j1 += m
            k1 -= m
            data[j1], data[k1] = data[k1], data[j1]
            j1 += m
            k1 += m2
            data[j1], data[k1] = data[k1], data[j1]
          end
          j1 = k + m + rev_pattern[k]
          k1 = j1 + m
          data[j1], data[k1] = data[k1], data[j1]
        end
      else
        (0...m).each do |k|
          (0...k).each do |j|
            j1 = j + rev_pattern[k]
            k1 = k + rev_pattern[j]
            data[j1], data[k1] = data[k1], data[j1]
            j1 += m
            k1 += m
            data[j1], data[k1] = data[k1], data[j1]
          end
        end
      end
    end

    def self.bitrev_copy rev_pattern, n, data, out
      m = rev_pattern.size
      m2 = m * 2
      if (n&1)==0
        (0...m).each do |k|
          (0...k).each do |j|
            j1 = j + rev_pattern[k]
            k1 = k + rev_pattern[j]
            out[j1], out[k1] = data[k1], data[j1]
            j1 += m
            k1 += m2
            out[j1], out[k1] = data[k1], data[j1]
            j1 += m
            k1 -= m
            out[j1], out[k1] = data[k1], data[j1]
            j1 += m
            k1 += m2
            out[j1], out[k1] = data[k1], data[j1]
          end
          k1 = k + rev_pattern[k]
          out[k1] = data[k1]
          j1 = k1 + m
          k1 = j1 + m
          out[j1], out[k1] = data[k1], data[j1]
          k1 += m
          out[k1] = data[k1]
        end
      else
        out[0] = data[0]
        out[m] = data[m]
        (0...m).each do |k|
          (0...k).each do |j|
            j1 = j + rev_pattern[k]
            k1 = k + rev_pattern[j]
            out[j1], out[k1] = data[k1], data[j1]
            j1 += m
            k1 += m
            out[j1], out[k1] = data[k1], data[j1]
          end
          k1 = k + rev_pattern[k]
          out[k1] = data[k1]
          out[k1+m] = data[k1+m]
        end
      end
    end
    
    
    def self.make_rev_pattern rev_pattern, n
      rev_pattern.push 0
      l = 2**n
      m = 1
      while (m<<2) < l
        l >>= 1
        (0...m).each do |j|
          rev_pattern.push rev_pattern[j] + l
        end
        m <<= 1
      end
    end
  
    public
    
    def self.dft data, out=nil
      n = calc_n data
      unless rev_pattern = @rev_pattern[n]
        @rev_pattern[n] = rev_pattern = Array.new
        make_rev_pattern rev_pattern, n
      end
      unless twiddle = @dft_twiddle[n]
        @dft_twiddle[n] = twiddle = Array.new
        make_twiddle twiddle, -1, n
      end
      if out
        bitrev_copy rev_pattern, n, data, out
        dit twiddle, n, out, -1
        out
      else
        bitrev_inplace rev_pattern, n, data
        dit twiddle, n, data, -1
        data
      end
    end

    def self.idft data, out=nil
      n = calc_n data
      unless rev_pattern = @rev_pattern[n]
        @rev_pattern[n] = rev_pattern = Array.new
        make_rev_pattern rev_pattern, n
      end
      unless twiddle = @idft_twiddle[n]
        @idft_twiddle[n] = twiddle = Array.new
        make_twiddle twiddle, 1, n
      end
      if out
        bitrev_copy rev_pattern, n, data, out
        dit twiddle, n, out, 1
        out
      else
        bitrev_inplace rev_pattern, n, data
        dit twiddle, n, data, 1
        data
      end
    end
  
  end
end

a = Array.new(8192,Complex(0.5,0.5))
a[1] = Complex(1.0,0.0)
t0 = Time.now
DSP::FFT.dft a
t1 = Time.now
puts "fft: #{(t1-t0) * 1000}ms"
