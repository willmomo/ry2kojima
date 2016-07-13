$KCODE = 'sjis'

require 'net/http'

#$g_proxy = {'server'=>nil, 'port'=>nil, 'user'=>nil, 'password'=>nil}
$g_proxy = {'server'=>'sanko-net.securewg.jp', 'port'=>8080, 'user'=>'ry2kojima', 'password'=>'ota7Yoda'}

def papimo_confirm(uri, corpcode, id, password)
	#res = Net::HTTP.post_form(URI.parse(uri), {'corpcode'=>corpcode, 'id'=>id, 'password'=>password})

	objProxy = Net::HTTP::Proxy($g_proxy['server'], $g_proxy['port'], $g_proxy['user'], $g_proxy['password'])
	res = objProxy.post_form(URI.parse(uri), {'corpcode'=>corpcode, 'id'=>id, 'password'=>password})
	puts res.body
end

#------------------------------------------------------------------------------

uri = 'http://gn:1234@dev4m.papimo.jp/addon/machinedatas_api/confirm.xml'
corpcode = '80095009'
id = 'xud8jn99'
password = 'uy3mnkg5'

papimo_confirm(uri, corpcode, id, password)
