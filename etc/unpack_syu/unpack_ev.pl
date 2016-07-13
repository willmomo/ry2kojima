open(FH, 'd:\hoge.txt') or die "can't open file: $!\n";
open(WH, '>d:\hoge.bin') or die "can't open file: $!\n";

binmode WH;

while (<FH>) {
	m/.+: (..) (..) (..) (..) (..) (..) (..) (..)/;
	
	print WH sprintf("%c", hex($1));
	print WH sprintf("%c", hex($2));
	print WH sprintf("%c", hex($3));
	print WH sprintf("%c", hex($4));
	print WH sprintf("%c", hex($5));
	print WH sprintf("%c", hex($6));
	print WH sprintf("%c", hex($7));
	print WH sprintf("%c", hex($8));
}

close FH;
close WH;
