$KCODE = 'sjis'

#----------------------------------------------------------------------------
# 復号化(IFBOX由来)
#----------------------------------------------------------------------------
def decrypt(x)
	w = x.unpack("m")[0].unpack("C*")

	ret = ""
	if w.shift == 0 then
		seed = w.shift
		last = 0
		w.each { |c|
			last = (last + (c ^ seed)) & 0xff
			ret += last.chr
			seed = ( (seed << 1) | (seed >> 7) ) & 0xff
		}
	else
		# unknown encode
	end

	return ret
end

#----------------------------------------------------------------------------
# 符号化(IFBOX由来)
#----------------------------------------------------------------------------
def encrypt(x)
	seed = rand(255)
	data = 0.chr + seed.chr

	back = 0;
	x.each_byte { |c|
		data += ( ((c - back) ^ seed) & 0xff ).chr
		back = c
		seed = ( (seed << 1) | (seed >> 7) ) & 0xff
	}

	return [data].pack("m").chomp
end

# 簡単な使い方

d = encrypt("80095005")
print "encrypt: #{d}\n"

s = decrypt(d)
print "decrpyt: #{s}\n"
