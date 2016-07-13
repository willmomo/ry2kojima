# -*- coding: sjis -*-

require 'find'

Find.find('E:\field_data\Glory\20110407_大劇_本店\20110403recov') {
	|fname|
	if (File.basename(fname) == 'mf1151.bin') then
		puts fname

		data = File.open(fname, "rb") { |fh| fh.read }
		puts data[0, 8]
		for i in 0..7
			data[i] = format("%c", "A".bytes.to_a[0] + i)
		end
		puts data[0, 8]
		File.open(fname, "wb") { |fh| fh.write data }
	end
}
