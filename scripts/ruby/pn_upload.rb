$KCODE = 'sjis'

require 'net/http'
require 'kconv'

$g_proxy = {'server'=>nil, 'port'=>nil, 'user'=>nil, 'password'=>nil}
#$g_proxy = {'server'=>'sanko-net.securewg.jp', 'port'=>8080, 'user'=>'ry2kojima', 'password'=>'ota7Yoda'}

def papimo_confirm(uri, companyid, tenpoid, password, pnszipfile)
	#res = Net::HTTP.post_form(URI.parse(uri), {'corpcode'=>corpcode, 'id'=>id, 'password'=>password})

	objProxy = Net::HTTP::Proxy($g_proxy['server'], $g_proxy['port'], $g_proxy['user'], $g_proxy['password'])
	res = objProxy.post_form(URI.parse(uri), {'COMPANYID'=>companyid, 'TENPOID'=>tenpoid, 'PASSWORD'=>password}, 'pnszipfile')
	puts res.body
end

def papimo_confirm2(uri, companyid, tenpoid, password, pnszipfile)
	#res = Net::HTTP.post_form(URI.parse(uri), {'corpcode'=>corpcode, 'id'=>id, 'password'=>password})

	objProxy = Net::HTTP::Proxy($g_proxy['server'], $g_proxy['port'], $g_proxy['user'], $g_proxy['password'])
	url = URI.parse(uri)
#	res = objProxy.post_form(URI.parse(uri), {'COMPANYID'=>companyid, 'TENPOID'=>tenpoid, 'PASSWORD'=>password})
#	puts res.body
end

#------------------------------------------------------------------------------

uri = 'http://pachi-navi.jp/PNManagerTest/PHP/Manage/uploadDataZip.php'
companyid = '80095009'
tenpoid = 'xud8jn99'
password = 'uy3mnkg5'
pnszipfile = ''

#papimo_confirm(uri, companyid, tenpoid, password, pnszipfile)
papimo_confirm2(uri, companyid, tenpoid, password, pnszipfile)
