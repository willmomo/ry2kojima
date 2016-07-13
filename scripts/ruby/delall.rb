$KCODE = 'sjis'

#----------------------------------------------------------------------
# 使用法を表示
#----------------------------------------------------------------------
def usage
	puts 'usage: delall フォルダ名 [...]'
end

#----------------------------------------------------------------------
# 指定フォルダ以下を再帰的に削除
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
# メイン
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
