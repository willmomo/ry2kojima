$KCODE = 'utf8'

require 'date'
require 'rexml/document'

#営業日を求める処理
def eigyoDate
	day = Time.now

	result = Date.new(day.year, day.month, day.day)
	if day.hour < 5 then
		result = result - 1
	end

	return result
end

#簡便の為、テキストデータとして、XMLを準備
xmlsource = <<EOF
<?xml version="1.0"?>
<root>
	<user>ifpcadmin</user>
	<pass>password</pass>
</root>
EOF


# XMLデータから、要素の情報を取り出す例
# 元データをファイルから読み込めば出来上がり

def getOption(source, key)
	result = "default"

	doc = REXML::Document.new source
	if doc.elements[key] != nil then
		result = doc.elements[key].text
	end

	return result
end

p eigyoDate.strftime('%Y%m%d') + '.zip'

# XMLデータから、要素の情報を取り出す例
# 元データをファイルから読み込めば出来上がり
doc = REXML::Document.new xmlsource
p doc.root
p doc.elements['root/user'].text
p getOption(xmlsource, 'root/user')
p getOption(xmlsource, 'root/user2')

# XMLデータをゼロから作り上げる
# 最後の処理をファイルに書けば、xmlファイルの出来上がり
doc2 = REXML::Document.new
doc2 << REXML::XMLDecl.new('1.0', 'UTF-8')
elem = doc2.add_element('root')
elem.add_element('user').add_text('ifpcadmin')
elem.add_element('pass').add_text('password')
doc2.write STDOUT
