# -*- encoding: sjis -*-

#code = "80000006"
#date = "20110412"
#time = "1300"

code = "00011508"
date = "20150612"
time = "0945"

puts("�t�@�C������ `" + code + date + time + ".zip' �̎��B")

print("�p�X���[�h�� `")
for i in 0..7
	print format("%c", date[i].to_i + (40 + 10 * i))
end
print(code + "' �ł��B")
