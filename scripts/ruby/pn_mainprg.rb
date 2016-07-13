$KCODE = 'sjis'

require 'date'
require 'net/ftp'
require 'rexml/document'


def writeXml(argFname, argSec, argKey, argValue)
	begin
		doc = REXML::Document.new(File.new(argFname))
	rescue
		doc = REXML::Document.new
		doc << REXML::XMLDecl.new('1.0', 'UTF-8')
		doc.add_element('root')
	end

	if (sec = doc.root.elements[1,argSec]) == nil then
		sec = doc.root.add_element(argSec)
	end

	if (key = sec.elements[1,argKey]) == nil then
		key = sec.add_element(argKey)
	end

	key.text = argValue

	File.open(argFname, 'w') {|f|
		doc.write(f)
	}
end

def readXml(argFname, argSec, argKey, argDeflt)
	begin
		result = REXML::Document.new(File.new(argFname)).root.text("#{argSec}/#{argKey}")
		if result == nil then
			STDERR.puts 'readXml : [Section] or [Key] Error!'
			result = argDeflt
			writeXml(argFname, argSec, argKey, argDeflt)
		end
	rescue
		STDERR.puts 'readXml : Argment Error!'
		result = argDeflt
		writeXml(argFname, argSec, argKey, argDeflt)
	end

	return result
end

def eigyoDate
	day = Time.now

	result = Date.new(day.year, day.month, day.day)
	if	day.hour < 5 then
		result = result - 1
	end

	return result
end

def getFtpFile(argHost, argUser, argPass, argDirPath, argfName)
	begin
		Net::FTP.open(argHost, argUser, argPass) { |ftp|
			ftp.binary = true
			ftp.chdir(argDirPath)

			ftp.nlst.each { | fObj |
				if fObj.downcase == argfName.downcase then
					ftp.get(fObj)

					if fObj == argfName then
					else
						system('mv ' + fObj + ' ' + argfName)
					end
					
					break
				end
			}
		}
	rescue	=> ex
		STDERR.puts ex.backtrace.pop + ' : ' + ex.message
	end
end




argSetFile = ARGV[0]
if argSetFile == nil then
	STDERR.puts 'mainProg.rb : Argment Error!'
	argSetFile = 'set.xml'
end

if File.exist?(argSetFile) then
else
	STDERR.puts 'mainProg.rb : Not Found ' + argSetFile + '!'
end

DefHost = ARGV[1]
DefUser = ARGV[2]
DefPass = ARGV[3]
DefDirPath = ARGV[4]

DefHost = "172.17.102.20" if DefHost == nil
DefUser = "mpcadmin" if DefUser == nil
DefPass = "glory" if DefPass == nil
DefDirPath = "/e-drive/csv3" if DefDirPath == nil


Host = readXml(argSetFile, 'FTP', 'Host', DefHost)
User = readXml(argSetFile, 'FTP', 'User', DefUser)
Pass = readXml(argSetFile, 'FTP', 'Pass', DefPass)
DirPath = readXml(argSetFile, 'FTP', 'DirPath', DefDirPath)


fName = eigyoDate.strftime('%Y%m%d') + '.zip'
oldfName = 'old_' + fName

getFtpFile(Host, User, Pass, DirPath, fName)

if File.exist?(fName) then
else
	STDERR.puts 'mainProg.rb : FTP Get Error (' + fName + ')!'
	exit(0)
end

if File.exist?(oldfName) then
	system('cmp --silent ' + fName + ' ' + oldfName)
	if $?.exitstatus == 1 then
		puts 'Send File(1)'
	end
else
	puts 'Send File(2)'
end

system('cp ' + fName + ' ' + oldfName)
system('rm ' + fName)
