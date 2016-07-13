#

require "openssl"

#
# C++ �̎����Ƒ��݂ɈÍ��E�����ł�������B
#
module KjmBlowfish
	def self.cipher(mode, key, data)
		cipher = OpenSSL::Cipher.new('bf-ecb').send(mode)

		# key �̒������w�肵�Ȃ��ƁA�����s�\���ŃG���[�ɂȂ�
		cipher.key_len = key.length
		cipher.key = key
		
		# padding �́A���O�ŏ�������
		cipher.padding = 0
		
		cipher.update(data) << cipher.final
	end
	
	def self.encrypt(key, data)
		pad = 8 - (data.length % 8)
		return cipher(:encrypt, key, data + (pad.chr * pad))
	end

	def self.decrypt(key, data)
		result = cipher(:decrypt, key, data)
		return result.partition(result[-1]).first
	end
end

if ARGV.length != 2 then
	puts 'usage: bfkjm.rb [key] [data]'
	exit(0)
end

key = ARGV[0]
str = ARGV[1]
if str[0] != '?' then
	cpt = KjmBlowfish.encrypt(key, str)
	puts "?#{cpt.unpack('H*').first}"
else
	org = KjmBlowfish.decrypt(key, [str[1, str.length]].pack('H*'))
	puts org
end

exit 0
