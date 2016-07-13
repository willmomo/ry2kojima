$KCODE = 'sjis'

require 'rexml/document'

def readXml(fname, sec, key, default)
	begin
		result = REXML::Document.new(File.new(fname)).root.text("#{sec}/#{key}")
		result = default if result == nil
	rescue
		result = 'err: ' + default
	end
	return result
end

def writeXml(fname, sec, key, value)
	begin
		doc = REXML::Document.new(File.new(fname))
	rescue
		doc = REXML::Document.new
		doc << REXML::XMLDecl.new('1.0', 'UTF-8')
		doc.add_element('root')
	end

	if (sec = doc.root.elements[1, sec]) == nil then
		sec = doc.root.add_element(sec)
	end

	if (key = sec.elements[1, key]) == nil then
		key = sec.add_element(key)
	end

	key.text = value

	doc.write(File.open(fname, 'w'))
end

p readXml(ARGV[0], ARGV[1], ARGV[2], 'def-value')
#writeXml(ARGV[0], ARGV[1], ARGV[2], ARGV[3])
#p readXml(ARGV[0], ARGV[1], ARGV[2], 'def-value')

