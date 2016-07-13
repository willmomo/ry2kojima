$KCODE = 'sjis'

require 'date'

text = '2012071109200012345678web.zip'
text = '20120711292000-12345678-web.zip'

if m = fname.match(/\A(\d{14})-(\d{8})-web.zip\z/)
	d = DateTime.strptime(m[1], "%Y%m%d%H%M%S")
end
