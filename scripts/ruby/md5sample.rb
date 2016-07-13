#! ruby -Ks
# coding: windows-31j

require 'pp'
require 'digest/md5'

def md5digest(s)
# MD5 メッセージダイジェストの生成
  return Digest::MD5.digest(s)
end

def md5hexdigest(s)
# MD5 メッセージダイジェストの生成（16進数表現）
  return Digest::MD5.hexdigest(s)
end

if __FILE__ == $0
  FILENAME="C:\\Users\\kojima\\Documents\\temp\\daiinfo_auto.zip"
  # 念のためカレントディレクトリを変更しておく
  Dir.chdir( File.dirname( File.expand_path( FILENAME ) ) )
  File.open(FILENAME, "rb") do |f|
    s = f.read
    pp s
    pp md5digest s
    pp md5hexdigest s
  end
end
