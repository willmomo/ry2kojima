# -*- encoding: sjis -*-

require 'net/http'
require 'uri'
require 'open-uri'
require 'nkf'

def download_sound
	p ARGV[0]
	begin
		File.binwrite('a.out', open(ARGV[0]).read)
	rescue
		puts $!
		puts "#{ARGV[0]} は存在しません。"
	end
end

def do_main
	download_sound
	return 0
end

# ---- program start ----

p __FILE__, $0

if __FILE__ == $0 then
	exit do_main
end
