# -*- encoding: SJIS -*-

#
# 1900�N�`2099�N�܂ł̏t���̓��̐��w�I�ȊȈՂȋ��ߕ�
#
# �Q�l: http://ja.wikipedia.org/wiki/%E6%98%A5%E5%88%86%E3%81%AE%E6%97%A5
#
for y in 1900..2099 do
	amari = y % 4;

	if amari == 0 then
		if 1900 <= y and y <= 1956 then
			print "#{y} #{amari} 3��21��\n";
		elsif 1960 <= y and y <= 2088 then
			print "#{y} #{amari} 3��20��\n";
		elsif 2092 <= y and y <= 2096 then
			print "#{y} #{amari} 3��19��\n";
		else
			print "#{y} #{amari} -��--��\n";
		end
	elsif amari == 1 then
		if 1901 <= y and y <= 1989 then
			print "#{y} #{amari} 3��21��\n";
		elsif 1993 <= y and y <= 2097 then
			print "#{y} #{amari} 3��20��\n";
		else
			print "#{y} #{amari} -��--��\n";
		end
	elsif amari == 2 then
		if 1902 <= y and y <= 2022 then
			print "#{y} #{amari} 3��21��\n";
		elsif 2026 <= y and y <= 2098 then
			print "#{y} #{amari} 3��20��\n";
		else
			print "#{y} #{amari} -��--��\n";
		end
	elsif amari == 3 then
		if 1903 <= y and y <= 1923 then
			print "#{y} #{amari} 3��22��\n";
		elsif 1927 <= y and y <= 2055 then
			print "#{y} #{amari} 3��21��\n";
		elsif 2059 <= y and y <= 2099 then
			print "#{y} #{amari} 3��20��\n";
		else
			print "#{y} #{amari} -��--��\n";
		end
	end
end
