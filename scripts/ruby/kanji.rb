require 'kconv'

class String
	def hex2bin
		s = self
		raise "Not a valid hex string" unless(s =~ /^[\da-fA-F]+$/)
		s = '0' + s if((s.length & 1) != 0)
		s.scan(/../).map{ |b| b.to_i(16) }.pack('C*')
	end

	def bin2hex
		self.unpack('C*').map{ |b| "%02X" % b }.join('')
	end
end

puts ARGV[0]
puts ARGV[0].match(/message:(.+)\}\}/)[1].gsub(/\\u([\da-fA-F]{2})([\da-fA-F]{2})/, '\1\2').hex2bin.kconv(Kconv::SJIS, Kconv::UTF16)
