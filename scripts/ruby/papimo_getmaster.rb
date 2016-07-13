$KCODE = 'sjis'

require 'net/http'
require 'rexml/document'

#$g_proxy = {'server'=>nil, 'port'=>nil, 'user'=>nil, 'password'=>nil}
$g_proxy = {'server'=>'sanko-net.securewg.jp', 'port'=>8080, 'user'=>'ry2kojima', 'password'=>'ota7Yoda'}

def papimo_getmaster(uri, corpcode, version)
	#res = Net::HTTP.post_form(URI.parse(uri), {'corpcode'=>corpcode, 'id'=>id, 'password'=>password})

	objProxy = Net::HTTP::Proxy($g_proxy['server'], $g_proxy['port'], $g_proxy['user'], $g_proxy['password'])
	res = objProxy.post_form(URI.parse(uri), {'corpcode'=>corpcode, 'version'=>version})
	puts res.body

	doc = REXML::Document.new(res.body)
	puts 'status = ' + doc.root.text("status")
	puts 'url    = ' + doc.root.text("url")

	File.open('e:/tmp/hoge.zip', 'wb') do |f|
		f.print objProxy.get(URI.parse(doc.root.text("url")))
	end
end

#------------------------------------------------------------------------------

uri = 'http://gn:1234@dev4m.papimo.jp/addon/machinedatas_api/get_machine_master_url.xml'
corpcode = '80095009'
version = 'v1'

papimo_getmaster(uri, corpcode, version)
