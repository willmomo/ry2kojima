$KCODE = 'sjis'
require 'net/ftp'
begin
	ftp = Net::FTP.new
	ftp.connect('localhost')
rescue => ex
	p ex.class
	p ex.message
end
