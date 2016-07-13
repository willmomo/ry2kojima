#! ruby -Ks
#
# dirdiff.rb �͈ȉ���URL�������
# http://webos-goodies.jp/archives/51298439.html
#
# ex)
# KanMusuMerge.rb
# \\pubftp\home\kojima\ExDocuments\KanMusu\0136_��a��_20160305 C:\Users\kojima\Documents\temp\�V�����t�H���_�[\0136_��a��_20160311

require 'fileutils'
require File.join(File.dirname($0), 'dirdiff.rb')

# png �t�@�C���� IDAT ���������o��
def png_get_IDAT(fname)
    ret = ""
    
    f = File.open(fname, "rb")
    
    png_sig = f.read(8)
    
    until f.eof do
        chunk_length = f.read(4).unpack("N")[0]
        chunk_type = f.read(4)
        chunk_data = f.read(chunk_length)
        chunk_crc = f.read(4)

        if chunk_type == "IDAT" then
            ret << chunk_data
        end
    end
    
    return ret
end

# png �t�@�C���̃f�[�^�����������r����
def cmp_png(f1, f2)
    idat1 = png_get_IDAT(f1)
    idat2 = png_get_IDAT(f2)
    
    return (idat1 == idat2)
end

### �w�肵���͂ނ�����W�J
def expand_kanmusu(path)
    print "$ �͂ނ���W�J --- #{File.basename(path)}\n"
    
    expand_path = File.join(Dir.pwd, File.basename(path))
#    expand_path.encode!(Encoding::UTF_8)
    
    FileUtils.rm_r(expand_path.encode("utf-8")) if FileTest.exist?(expand_path)
    Dir.mkdir(expand_path)
    
    find_path = File.join(path, "*.zip").gsub('\\', '/')

    Dir.glob(find_path) do |f|
        outpath = File.join(expand_path, File.basename(f, ".*"))
        `7za x \"#{f.gsub('/', '\\')}\" -o\"#{outpath.gsub('/', '\\')}\" -y`
    end
    
    return expand_path
end

### �t�H���_���r
def comp_kanmusu(path1, path2)
    diff = DirDiff.new(path1, path2)
    diff.each do |fname, type, operation|
        if type == :file and operation == :modified and File.extname(fname) == ".png" then
            if cmp_png(File.join(path1, fname), File.join(path2, fname)) then
                next
            end
        end
        printf("%-16s %-10s %s\n", fname.inspect, type.inspect, operation.inspect)
    end
end

### main function
def main(path1, path2)
    comp_kanmusu(
        expand_kanmusu(path1),
        expand_kanmusu(path2))
end

### program start ###
# main(ARGV[0], ARGV[1])
main(
    'C:\Users\kojima\Documents\temp\�V�����t�H���_�[\0025_�k��_20160315_1',
    'C:\Users\kojima\Documents\temp\�V�����t�H���_�[\0025_�k��_20160315_2')
