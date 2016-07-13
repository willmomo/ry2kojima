#!/usr/bin/ruby
#
# �t�@�C�����B�e�����Ƀt�@�C������ύX����
#                               2010/03/04 Yasuki
#                               2010/10/05 Yasuki
#
$KCODE = "sjis"

require 'rubygems'
require 'exifr'
require 'date'
require "optparse"

$g_subdir = false

begin
	opts = OptionParser.new
	opts.banner += " <�f�B���N�g��>"
	opts.version = '0.2011.6.9'
	opts.on('-s') { |v| $g_subdir = true }
	opts.parse!(ARGV)
rescue => e
	puts "#{e}"
	puts opts.help
	exit
ensure
end

if ARGV.length == 0 then
	puts "�������ُ�ł��B"
	puts opts.help
	exit
end

prevDir = ""

for folder in ARGV do
	puts "Base: #{folder}"
	Dir::glob("#{folder.gsub('\\', '/')}#{($g_subdir)?'/**':''}/*", File::FNM_CASEFOLD) { |orgfile|
		next if not File.file?(orgfile)

		if prevDir != File.dirname(orgfile) then
			puts ">> #{File.dirname(orgfile)}"
			prevDir = File.dirname(orgfile)
		end

		begin
			@E = EXIFR::JPEG.new(orgfile)
		rescue EXIFR::MalformedJPEG => e
			puts "exception: #{File.basename(orgfile)} �́AJPEG �ł͂���܂���B"
			next
		rescue => e
			puts "exception: #{File.basename(orgfile)} �ŁA��O���������܂����B"
			puts "exception: #{e}"
			exit
		ensure
		end

		datestr = @E.date_time_original.to_s

		#p "------------------------------------------------------------"
		#p File.basename(orgfile)
		#p datestr

		if (datestr != "") then
			newfilebase = DateTime.parse(datestr).strftime("%Y%m%d_%H%M%S")	# �ׂ�������
			##newfilebase = DateTime.parse(datestr).strftime("%y%m%d_%H%M")	# �g�ѕ�����

			if @E.subsec_time_orginal.to_s != "" then
				newfilebase += "_" + @E.subsec_time_orginal.to_s
			end

			for i in 1..99 do
				newfilename = File.join(File.dirname(orgfile), newfilebase + ("~%02d" % i) + ".jpg")
				break unless FileTest::exist?(newfilename)
			end

			puts "[#{datestr}] #{File.basename(orgfile)} �� #{File.basename(newfilename)}"
			File.rename(orgfile, newfilename)
		else
			puts "#{File.basename(orgfile)} : �B�e���t��񂪂���܂���B"
		end
	}
end
