$KCODE = 'sjis'

#----------------------------------------------------------------------
# �g�p�@��\��
#----------------------------------------------------------------------
def usage
	puts 'usage: delall �t�H���_�� [...]'
end

#----------------------------------------------------------------------
# �w��t�H���_�ȉ����ċA�I�ɍ폜
#----------------------------------------------------------------------
def delall(path)
	Dir.foreach(path) { |item|
		if item != '.' && item != '..' then
			child = File.join(path, item)
			if FileTest.directory?(child) then
				delall child
			else
				puts "[F] #{child}"
				File.delete child
			end
		end
	}
	puts "[D] #{path}"
	Dir.rmdir path
end

#----------------------------------------------------------------------
# ���C��
#----------------------------------------------------------------------
def doMain
	if ARGV.length == 0 then
		usage
		return -1
	end

	ARGV.each { |arg|
		delall arg.gsub('\\', '/')
	}

	return 0
end

#======================================================================

exit doMain
