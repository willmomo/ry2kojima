$KCODE = 'utf8'

require 'date'
require 'rexml/document'

#簡便の為、テキストデータとして、XMLを準備
xmlsource = <<EOF
<?xml version="1.0"?>
<root>
	<3>111</3>
	<1>222</1>
	<3>222</3>
	<1>444</1>
	<1>555</1>
</root>
EOF

doc = REXML::Document.new xmlsource
p doc
p doc.root
p doc.root.elements[3, '1'], doc.root.elements[3, '1'].text
p doc.root.elements[2, '1'], doc.root.elements[2, '1'].text

