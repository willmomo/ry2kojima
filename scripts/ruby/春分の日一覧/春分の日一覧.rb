# -*- encoding: SJIS -*-

#
# 1900年～2099年までの春分の日の数学的な簡易な求め方
#
# 参考: http://ja.wikipedia.org/wiki/%E6%98%A5%E5%88%86%E3%81%AE%E6%97%A5
#
for y in 1900..2099 do
	amari = y % 4;

	if amari == 0 then
		if 1900 <= y and y <= 1956 then
			print "#{y} #{amari} 3月21日\n";
		elsif 1960 <= y and y <= 2088 then
			print "#{y} #{amari} 3月20日\n";
		elsif 2092 <= y and y <= 2096 then
			print "#{y} #{amari} 3月19日\n";
		else
			print "#{y} #{amari} -月--日\n";
		end
	elsif amari == 1 then
		if 1901 <= y and y <= 1989 then
			print "#{y} #{amari} 3月21日\n";
		elsif 1993 <= y and y <= 2097 then
			print "#{y} #{amari} 3月20日\n";
		else
			print "#{y} #{amari} -月--日\n";
		end
	elsif amari == 2 then
		if 1902 <= y and y <= 2022 then
			print "#{y} #{amari} 3月21日\n";
		elsif 2026 <= y and y <= 2098 then
			print "#{y} #{amari} 3月20日\n";
		else
			print "#{y} #{amari} -月--日\n";
		end
	elsif amari == 3 then
		if 1903 <= y and y <= 1923 then
			print "#{y} #{amari} 3月22日\n";
		elsif 1927 <= y and y <= 2055 then
			print "#{y} #{amari} 3月21日\n";
		elsif 2059 <= y and y <= 2099 then
			print "#{y} #{amari} 3月20日\n";
		else
			print "#{y} #{amari} -月--日\n";
		end
	end
end
