#!/usr/bin/ruby

$KCODE = 'utf-8'

require 'fileutils'
require 'date'
require 'tmpdir'

module Kjm
    #----------------------------------------------------------------------
    # ログの基本クラス
    #----------------------------------------------------------------------
    class Logger
    	attr_accessor :maxSize
    	attr_accessor :sedaiNum
    	attr_accessor :filePre
    	attr_accessor :fileExt
    	attr_accessor :outPath
    	attr_accessor :logLevel

        LOGLEVEL_FATAL = 5
        LOGLEVEL_ERROR = 4
        LOGLEVEL_WARN = 3
        LOGLEVEL_INFO = 2
        LOGLEVEL_DEBUG = 1
        LOGLEVEL_TRACE = 0

    	#------------------------------------------------------------------
    	# コンストラクタ
    	#------------------------------------------------------------------
    	def initialize
    	    # p Dir.tmpdir
    	    
    		@maxSize = (1 * 1024 * 1024)
    		@sedaiNum = 9
    		@filePre = File.basename($PROGRAM_NAME, File.extname($PROGRAM_NAME))
    		@fileExt = '.log'
    		if (RUBY_PLATFORM.downcase =~ /mswin(?!ce)|mingw|cygwin|bccwin/)
    			@outPath = './log'	# windows の時は、カレントディレクトリの下をデフォルトにする
    		else
    			@outPath = '/ifpc/log'
    		end
    		@logLevel = LOGLEVEL_DEBUG
    	end

    	def to_s
    		return "#{super.to_s}{@maxSize:#{@maxSize},@sedaiNum:#{@sedaiNum},@filePre:\"#{@filePre}\",@fileExt:\"#{@fileExt}\",@outPath:\"#{@outPath}\",@logLevel:#{@logLevel}}"
    	end

    	#------------------------------------------------------------------
    	# 古いログをシフトする
    	#------------------------------------------------------------------
    	def rotate
    		# 最も古い世代のログを削除
    		fname1 = File.join(@outPath, "#{@filePre}#{@fileExt}." + @sedaiNum.to_s)
    		FileUtils.rm(fname1, {:force => true}) if File.exist?(fname1)

    		# 世代数分の繰り返し
    		(@sedaiNum - 1).downto(1) do |num|
    		    fname2 = fname1
    		    fname1 = File.join(@outPath, "#{@filePre}#{@fileExt}." + num.to_s)

    			# 移動元ログファイルが存在する場合は移動
    			File.rename(fname1, fname2) if File.exist?(fname1)
    		end

    		# 最初のファイルをバックアップ
			File.rename(File.join(@outPath, "#{@filePre}#{@fileExt}"), fname1)
    	end

    	#------------------------------------------------------------------
    	# ログ出力関数
    	# strLog = ログファイルに書込む文言（ログファイル 1行分）
    	#------------------------------------------------------------------
    	def write_f(log_str); write(LOGLEVEL_FATAL, log_str); end
    	def write_e(log_str); write(LOGLEVEL_ERROR, log_str); end
    	def write_w(log_str); write(LOGLEVEL_WARN, log_str); end
    	def write_i(log_str); write(LOGLEVEL_INFO, log_str); end
    	def write_d(log_str); write(LOGLEVEL_DEBUG, log_str); end
    	def write_t(log_str); write(LOGLEVEL_TRACE, log_str); end

    	def write(logLevel, strLog)
    		return if logLevel < @logLevel

    		# 書込むログフォルダが存在しない場合は作成
    		FileUtils.mkdir_p(@outPath) unless File.exist?(@outPath)

    		# ログファイルにログを出力（1行分）
    		logFile = File.join(@outPath, "#{@filePre}#{@fileExt}")
    		File.open(logFile, 'a') do |f|
    			t = Time.now.instance_eval { '%s.%03d' % [strftime("%Y/%m/%d %H:%M:%S"), (usec / 1000.0).round] }
    			f.write("#{t} [#{logLevel}][#{sprintf('%04X', Process.pid)}] #{strLog}\n")
    		end

    		# ログファイルのサイズは、最大サイズを超えているか？
    		rotate if File.stat(logFile).size >= @maxSize
    	rescue => ex
    		output_err_stdmsg(ex)
    	end

    	def output_err_stdmsg(ex)
    		# 標準出力にエラーメッセージを出力
    		err_message = ex.message.to_s
    		err_backtrace = ex.backtrace.to_s	
    		puts err_message
    		puts err_backtrace
    		write_info(err_message)
    		write_info(err_backtrace)
    	end
    end
end
