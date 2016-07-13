#!/usr/bin/ruby

# いくつかのプログラムを実行して、自分は終わる。
# プロセス起動のサンプル

$KCODE = 'utf-8'

#----------------------------------------------------------------------
# utf-8のソースの出力をsjisに化かす
#----------------------------------------------------------------------
if RUBY_PLATFORM =~ /mswin(?!ce)|mingw|cygwin|bccwin/
	require 'kconv'
	class StdoutHook
		def write(str)
			STDOUT.write NKF.nkf("--ic=UTF-8 --oc=Windows-31J", str.to_s)
		end
		$> = new
	end
end

#----------------------------------------------------------------------

begin
	p "プログラムスタート"

	fork do
		exec("notepad.exe")
	end
rescue
	p "例外発生"
	p $!
end
