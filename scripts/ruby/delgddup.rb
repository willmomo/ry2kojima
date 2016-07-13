#----------------------------------------------------------------------
# Google Drive で重複しているファイルを探して削除する
#----------------------------------------------------------------------
$KCODE = 'sjis'

require 'fileutils'
require 'digest/md5'

#----------------------------------------------------------------------
# 使用法を表示
#----------------------------------------------------------------------
def usage
	puts 'usage: delall フォルダ名 [...]'
end

#----------------------------------------------------------------------
# Google Drive で重複しているファイルを探して削除する
#----------------------------------------------------------------------
def delgddup(path)
	Dir.foreach(path) { |item|
		if item != '.' && item != '..' then
			child = File.join(path, item)
			if FileTest.directory?(child) then
				delgddup child
			else
				if /^(.+) \(1\)(.+)$/ =~ item then
					child2 = File.join(path, "#$1#$2")
					if File.exists?(child2) then
						if FileUtils.cmp(child, child2) then
							md5_1 = Digest::MD5.hexdigest(File.open(child,'rb').read)
							md5_2 = Digest::MD5.hexdigest(File.open(child2,'rb').read)

							puts "[F] #{md5_1} #{child}"
							puts "[F] #{md5_2} #{child2}"

							File.chmod(0777, child)
							File.delete(child)
						end
					end
				end
			end
		end
	}
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
		delgddup arg.gsub('\\', '/')
	}

	return 0
end

#======================================================================

exit doMain
