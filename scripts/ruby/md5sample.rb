#! ruby -Ks
# coding: windows-31j

require 'pp'
require 'digest/md5'

def md5digest(s)
# MD5 ���b�Z�[�W�_�C�W�F�X�g�̐���
  return Digest::MD5.digest(s)
end

def md5hexdigest(s)
# MD5 ���b�Z�[�W�_�C�W�F�X�g�̐����i16�i���\���j
  return Digest::MD5.hexdigest(s)
end

if __FILE__ == $0
  FILENAME="C:\\Users\\kojima\\Documents\\temp\\daiinfo_auto.zip"
  # �O�̂��߃J�����g�f�B���N�g����ύX���Ă���
  Dir.chdir( File.dirname( File.expand_path( FILENAME ) ) )
  File.open(FILENAME, "rb") do |f|
    s = f.read
    pp s
    pp md5digest s
    pp md5hexdigest s
  end
end
