$KCODE = 'sjis'

require 'net/http'
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

#------------------------------------------------------------------------------
# GETして表示するだけ
#------------------------------------------------------------------------------
def ex1()
	Net::HTTP.get_print '192.168.120.214', '/dev/default.asp'
end

#------------------------------------------------------------------------------
# URIを使う
#------------------------------------------------------------------------------
def ex2()
	Net::HTTP.get_print URI.parse('http://192.168.120.214/dev/default.asp')
end

#------------------------------------------------------------------------------
# より汎用的な例
#------------------------------------------------------------------------------
def ex3()
	url = URI.parse('http://192.168.120.214/dev/default.asp')
	res = Net::HTTP.start(url.host, url.port) {|http|
		http.get('/dev/default.asp')
	}
	puts res.body
end

#------------------------------------------------------------------------------
# 上の例よりさらに汎用的な例
#------------------------------------------------------------------------------
def ex4()
	url = URI.parse('http://192.168.120.214/dev/default.asp')
	req = Net::HTTP::Get.new(url.path)
	res = Net::HTTP::start(url.host, url.port) {|http|
		http.request(req)
	}
	puts res.body
end

#------------------------------------------------------------------------------
# proxy経由
#------------------------------------------------------------------------------
def pxex(uri)
#	url = URI.parse('http://www.eigowithluke.com/2011/03/excuse-me-sorry/')
#	res = Net::HTTP::Proxy('sanko-net.securewg.jp', 8080, 'ry2kojima', 'ota7Yoda').start(url.host, url.port) {|http|
#		http.get('/index.html')
#	}
#	puts res.body

	#proxy_serv = nil
	#proxy_port = nil
	proxy_serv = 'sanko-net.securewg.jp'
	proxy_port = 8080
	url = URI.parse(uri)

	p url
	p url.path
	p url.host

	req = Net::HTTP::Get.new(url.path)

	p req

	res = Net::HTTP::Proxy(proxy_serv, proxy_port, 'ry2kojima', 'ota7Yoda').start(url.host, url.port) {|http|
		http.request(req)
	}
	puts res
end

def ex5()
	uri = 'http://papimo.exabyte.co.jp/hallman/PHP/manage/uploadDataZip.php'
	url = URI.parse(uri)
	req = Net::HTTP::Get.new(url.path)

#	proxy_serv = 'sanko-net.securewg.jp'
#	proxy_port = 8080
	proxy_serv = nil
	proxy_port = nil
	Net::HTTP::Proxy(proxy_serv, proxy_port, 'ry2kojima', 'ota7Yoda').start(url.host, url.port) {|http|

##	res = Net::HTTP.start(url.host, url.port) {|http|

		request = Net::HTTP::Post.new(url.path)

		#ヘッダー部
		request["user-agent"] = "Ruby/#{RUBY_VERSION} MyHttpClient"
		request.set_content_type("multipart/form-data; boundary=myboundary")
		#（以下でも可）
		#request["content-type"] = "multipart/form-data; boundary=myboundary"

		#ボディ部
		#multipart/form-dataの仕様にあわせてbodyを作成
		body = ""
		body.concat("--myboundary\r\n")
		body.concat("content-disposition: form-data; name=\"COMPANYID\";\r\n")
		body.concat("\r\n")
		body.concat("1\r\n")

		body.concat("--myboundary\r\n")
		body.concat("content-disposition: form-data; name=\"TENPOID\";\r\n")
		body.concat("\r\n")
		body.concat("13\r\n")

		body.concat("--myboundary\r\n")
		body.concat("content-disposition: form-data; name=\"PASSWORD\";\r\n")
		body.concat("\r\n")
		body.concat("Administrator\r\n")

		body.concat("--myboundary\r\n")
		body.concat("content-disposition: form-data; name=\"PNSZIPFILE\"; filename=\"sample.zip\"\r\n")
		body.concat("content-type: application/x-zip-compressed\r\n")
		body.concat("\r\n")
		File::open("sample.zip"){|f| body.concat(f.read + "\r\n") }

		body.concat("--myboundary--\r\n")
		request.body = body

		#送信
		response = http.request(request)

		puts 'ex5'
		puts response.body

		puts response.body.match(/message\":\"(.+)\"/)[1].gsub(/\\u([\da-fA-F]{2})([\da-fA-F]{2})/, '\1\2').hex2bin.kconv(Kconv::SJIS, Kconv::UTF16)
	}
	#puts res.body
end

#------------------------------------------------------------------------------


#ex1()
#ex2()
#ex3()
#ex4()

#pxex('http://www.yahoo.co.jp/')
#pxex('http://www.eigowithluke.com/2011/03/excuse-me-sorry/')

ex5()
