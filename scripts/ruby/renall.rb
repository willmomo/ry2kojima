# encoding: utf-8

require 'optparse'

Version = "1.1.0.0 - 2015/07/22 - r.kojima"

#puts "__ENCODING__: #{__ENCODING__}"

def dbg(expr)
	puts "[#{expr.encoding}] #{expr}"
end

#----------------------------------------------------------------------
# forfiles な関数
#----------------------------------------------------------------------
def for_files(path, mask, subdir, &block)
	return enum_for(__method__, path, mask, subdir) unless block_given?
	
	# ファイル名に utf-8 が含まれる場合、File.ftype が Errno::ENOENT になってしまう。
	# glob のパターンを encode('utf-8') とすることで回避可能
	# ただし、block に渡される引数も utf-8 なので、色々とかっこ悪い
	
	pattern = File.join(path, '*')
	Dir.glob(pattern.encode('utf-8')) do |file|
		isdir = File.directory?(file)
		args = { :isdir=>isdir, :path=>file, :file=>File.basename(file) }
		if isdir then
			for_files(file, mask, subdir, &block) if subdir
		end
		if File.basename(file) =~ /#{mask.encode('utf-8')}/i then
			# block.call(args) if block_given?
			yield(args)
		end
	end
end

#----------------------------------------------------------------------
# プログラムスタート
#----------------------------------------------------------------------
def do_main()
	params = {:path=>'.', :subdir=>false, :test=>false}

	opt = OptionParser.new
	opt.banner += " 検索文字列 置換文字列"
	opt.on('-p', '--path=パス名') { |v| params[:path] = v.gsub(File::ALT_SEPARATOR) {File::SEPARATOR} }
	opt.on('-s', '--subdir') { |v| params[:subdir] = true }
	opt.on('-t', '--test') { |v| params[:test] = true }
	begin
		opt.parse!(ARGV)
	rescue => e
		puts e
		puts opt.help
		exit 1
	end
	
	if ARGV.count != 2 then
		puts opt.help
		exit 1
	end
	
# for_files が each としても使える例
#	targets = for_files(params[:path], ARGV[0], params[:subdir])
#	targets.each do |args|
#		puts args
#	end
	
	for_files(params[:path], ARGV[0], params[:subdir]) do |args|
	
		# utf-8混じりのファイル名に対応するため、args の中身は utf-8 になっている
		# ARGV など sjis encoding になってしまう物と整合性を取るため、
		# encode しまくり。
		
		new_path = File.join(
			File.dirname(args[:path]),
			args[:file].gsub(/#{ARGV[0].encode('utf-8')}/i, ARGV[1].encode('utf-8')).encode('utf-8'))
		if params[:test] then
			# テストモードが有効のときは、変更後のファイル名が存在するかどうかを調べる
			print "* 注意 * " if File.exists?(new_path)
			puts "'#{args[:path]}' -> '#{File.basename(new_path)}'"
		else
			puts new_path
			File::rename(args[:path], new_path)
		end
	end
	return 0
end

exit(do_main) if __FILE__ == $PROGRAM_NAME
