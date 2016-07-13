$KCODE = "sjis"

require 'date'
require 'fileutils'
require 'timeout'
require 'tmpdir'

class Log
	def initialize
		#@m_path = File.expand_path("log", ENV["TEMP"])
		set_path(0)
		@m_name = File.basename(__FILE__, ".*") + ".log"
		@m_size = 1 * 1024 * 1024
		@m_nums = 99
		@m_limit = 100
		@m_inline = false
	end

	def path
		@m_path
	end

	def path=(value)
		@m_path = value
	end

	def name
		@m_name
	end

	def name=(value)
		@m_name = value
	end

	def size
		@m_size
	end

	def size=(value)
		@m_size = value
	end

	def nums
		@m_nums
	end

	def nums=(value)
		@m_nums = value
	end

	def limit
		@m_limit
	end

	def limit=(value)
		@m_limit = value
	end

	#
	# 汎用的にログの出力先を設定する。
	#
	# mode = 0 : %TEMP% の下
	# mode = 1 : 実行ファイルの場所
	# mode = 2 : カレントディレクトリの下
	#
	def set_path(mode, path = "log")
		@m_path = case
			when mode == 0 then
				File.expand_path(path, ENV["TEMP"])
			when mode == 1 then
				File.expand_path(path, File.dirname(__FILE__))
			when mode == 2 then
				File.expand_path(path, Dir.pwd)
			else
				File.expand_path(path, ENV["TEMP"])
			end
	end


	class AccessDenied < StandardError; end
	def lock(&block)
		begin
			# 10秒以内に終わらない場合はAccessDenied例外が発生
			Timeout::timeout(10) do
				File.open(File.join(ENV["TEMP"], 'my-application.lock'), 'w') do |f|
					begin
						f.flock(File::LOCK_EX)
						block.call
					ensure
						f.flock(File::LOCK_UN)
					end
				end
			end
		rescue Exception => ex
			raise AccessDenied.new('timeout')
		end
	end


	#
	# ログ出力
	#
	def write(msg)
		t1 = Time.now.usec

		FileUtils.mkdir_p(@m_path)

		p Dir.tmpdir
		lock do
			lf = File.expand_path(@m_name, @m_path)
			File.open(lf, "a") {|file|
				file.flock(File::LOCK_EX)
				file.puts(DateTime.now.strftime("%Y/%m/%d %H:%M:%S") + " " + msg)
			}

			if File.stat(lf).size > @m_size
				old_lf = lf + format(".%03d", @m_nums)
				File.unlink(old) if File.exist?(old)
				(@m_nums - 1).downto(0) {|n|
					old_lf = lf + format(".%03d", n)
					bak_lf = lf + format(".%03d", n + 1)
					File.rename(old_lf, bak_lf) if File.exist?(old_lf)
				}
				File.rename(lf, lf + ".000")
			end
		end

		ret = (Time.now.usec - t1) / 1000
		if (@m_inline == false) and (ret > @m_limit) then
			@m_inline = true
			write("<<<< WARNING >>>> over limit time: #{ret} ms <<<< WARNING >>>>")
			@m_inline = false
		end
		
		ret
	end
end

#----------------------------------------------------------------------------

#logWrite('start')

g_log = Log.new
#g_log.path = File.expand_path("log", File.dirname(__FILE__))
#g_log.set_path(1)
g_log.set_path(2)
#g_log.limit = 10

p g_log.path
p g_log.name
p g_log.size
p g_log.nums
p g_log.limit

0.upto(1000) {|n|
	g_log.write("count is #{n}")
	sleep(0.1)
}
