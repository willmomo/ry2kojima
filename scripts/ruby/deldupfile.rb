#------------------------------------------------------------------------------
# ��ڂ̈����Ŏw�肵���t�H���_����t�@�C�����
# �����̃t�@�C����
# ��ڂ̈����Ŏw�肵���t�H���_���猟��
# �t�@�C�������������Ƃ��́A��̃t�@�C�����r
# ��r���ʂ��y�����z�������Ƃ��A
# ��ڂ̃t�H���_���猩�������t�@�C�����폜����X�N���v�g
#------------------------------------------------------------------------------
$KCODE = 'SJIS'

require "optparse"

$g_realpart = false

#------------------------------------------------------------------------------
# �t�@�C��������v���Ă���t�@�C����T���o���p�^�[��
#------------------------------------------------------------------------------
def doMainA()
	Dir.glob(ARGV[0].gsub("\\", "/") + "/**/*") { |f1|
		if File.file?(f1) then
			puts "����..[#{FileTest.size(f1).to_s}] #{f1}"
			Dir.glob(ARGV[1].gsub("\\", "/") + "/**/" + File.basename(f1)) { |f2|
				puts "���..[#{FileTest.size(f2).to_s}] #{f2}"

				fp1 = File.open(f1, "r")
				fp2 = File.open(f2, "r")

				result = (fp1.read == fp2.read) ? true : false

				fp1.close
				fp2.close

				if result then
					puts "��v.."
					if $g_realpart then
						puts "'#{f1}' ���폜���܂��B"
						File.unlink(f1)
					end
				else
					puts "�s��v.."
				end
			}
		end
	}
end

#------------------------------------------------------------------------------
# �T�C�Y����v���Ă���t�@�C����T���o���p�^�[��
#------------------------------------------------------------------------------
def doMainB()
	Dir.glob(ARGV[0].gsub("\\", "/") + "/**/*") { |f1|
		if File.file?(f1) then
			puts "����..[#{FileTest.size(f1).to_s}] #{f1}"
			Dir.glob(ARGV[1].gsub("\\", "/") + "/**/*") { |f2|
				if FileTest.size(f1) == FileTest.size(f2) then
					puts "���..[#{FileTest.size(f2).to_s}] #{f2}"

					fp1 = File.open(f1, "r")
					fp2 = File.open(f2, "r")

					result = (fp1.read == fp2.read) ? true : false

					fp1.close
					fp2.close

					if result then
						puts "��v.."
						if $g_realpart then
							puts "'#{f1}' ���폜���܂��B"
							File.unlink(f1)
							break
						end
					else
						puts "�s��v.."
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
		opts.banner += " <�f�B���N�g��1> <�f�B���N�g��2>"
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
		puts "�������ُ�ł��B"
		puts opts.help
		exit
	end

	doMainB()
end



doMain()	# program start.
