# -*- encoding: utf-8 -*-

require 'net/http'
require 'uri'
require 'open-uri'
require 'nkf'

def download_sound
	basePath = File.join(ENV['USERPROFILE'], 'Google ドライブ\Voice\艦これ')

	p ARGV[0]
	p ARGV[1]

	basePath = File.join(basePath, ARGV[0].encode('utf-8'))
	puts basePath

	Dir.mkdir(basePath) if !Dir.exists?(basePath)

	kanmusu = /sound\/(.+)\//.match(ARGV[1])[1]

	for num in 1..60 do
		path = "http://203.104.209.71/kcs/sound/#{kanmusu}/#{num}.mp3"
		puts path
		fname = File.join(basePath, File.basename(URI.parse(path).path))
		begin
			File.binwrite(fname, open(path).read)
		rescue
			puts "#{num}.mp3 は存在しません。"
		end
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
