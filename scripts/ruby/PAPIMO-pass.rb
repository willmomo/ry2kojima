#! ruby -Ku
# $KCODE = 'SJIS'
$KCODE = 'utf8'

code = "80095010"
date = "20150527"
time = "1551"

puts("ファイル名が `" + code + date + time + ".zip' の時。")

print("パスワードは `")
for i in 0..7
	print format("%c", date[i, 1].to_i + (40 + 10 * i))
#	print format("%d", date[i].to_i)
#	p date[i, 1].to_i
end
print(code + "' です。")
