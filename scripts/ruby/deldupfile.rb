#------------------------------------------------------------------------------
# 一つ目の引数で指定したフォルダからファイルを列挙
# 同名のファイルを
# 二つ目の引数で指定したフォルダから検索
# ファイルが見つかったときは、二つのファイルを比較
# 比較結果が【同じ】だったとき、
# 一つ目のフォルダから見つかったファイルを削除するスクリプト
#------------------------------------------------------------------------------
$KCODE = 'SJIS'

require "optparse"

$g_realpart = false

#------------------------------------------------------------------------------
# ファイル名が一致しているファイルを探し出すパターン
#------------------------------------------------------------------------------
def doMainA()
	Dir.glob(ARGV[0].gsub("\\", "/") + "/**/*") { |f1|
		if File.file?(f1) then
			puts "検索..[#{FileTest.size(f1).to_s}] #{f1}"
			Dir.glob(ARGV[1].gsub("\\", "/") + "/**/" + File.basename(f1)) { |f2|
				puts "候補..[#{FileTest.size(f2).to_s}] #{f2}"

				fp1 = File.open(f1, "r")
				fp2 = File.open(f2, "r")

				result = (fp1.read == fp2.read) ? true : false

				fp1.close
				fp2.close

				if result then
					puts "一致.."
					if $g_realpart then
						puts "'#{f1}' を削除します。"
						File.unlink(f1)
					end
				else
					puts "不一致.."
				end
			}
		end
	}
end

#------------------------------------------------------------------------------
# サイズが一致しているファイルを探し出すパターン
#------------------------------------------------------------------------------
def doMainB()
	Dir.glob(ARGV[0].gsub("\\", "/") + "/**/*") { |f1|
		if File.file?(f1) then
			puts "検索..[#{FileTest.size(f1).to_s}] #{f1}"
			Dir.glob(ARGV[1].gsub("\\", "/") + "/**/*") { |f2|
				if FileTest.size(f1) == FileTest.size(f2) then
					puts "候補..[#{FileTest.size(f2).to_s}] #{f2}"

					fp1 = File.open(f1, "r")
					fp2 = File.open(f2, "r")

					result = (fp1.read == fp2.read) ? true : false

					fp1.close
					fp2.close

					if result then
						puts "一致.."
						if $g_realpart then
							puts "'#{f1}' を削除します。"
							File.unlink(f1)
							break
						end
					else
						puts "不一致.."
					end
				end
			}
		end
	}
end

#------------------------------------------------------------------------------
# program start.
#------------------------------------------------------------------------------
def doMain
	begin
		opts = OptionParser.new
		opts.banner += " <ディレクトリ1> <ディレクトリ2>"
		opts.version = '0.2011.6.8'
		opts.on('--realpart') { |v| $g_realpart = true }
		opts.parse!(ARGV)
	rescue => e
		puts "#{e}"
		puts opts.help
		exit
	ensure
	end

	if ARGV.length != 2 then
		puts "引数が異常です。"
		puts opts.help
		exit
	end

	doMainB()
end



doMain()	# program start.
