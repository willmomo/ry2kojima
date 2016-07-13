$KCODE = 'sjis'

require 'net/ftp'

def sampleFtp1(host, user, pass)
	p 'sampleFtp1'
	ftp = Net::FTP.new
	ftp.connect(host)
	ftp.login(user, pass)
	ftp.nlst('/glory').each {|list|
		p list
	}
	ftp.quit
end

def sampleFtp2(host, user, pass)
	begin
		p 'sampleFtp2'
		Net::FTP.open(host, user, pass) { |ftp|
			ftp.nlst('/glory').each {|list|
				p list
			}
		}
	rescue => ex
		p ex.class
		p ex.message
	end
end

sampleFtp1('221.246.147.36', 'anonymous', 'a@b.c')
sampleFtp2('221.246.147.36', 'anonymous', 'a@b.c')
