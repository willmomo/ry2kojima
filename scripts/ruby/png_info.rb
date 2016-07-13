#! ruby -Ks

#

def dumpString(s)
    s.bytes { |b|
        printf("%02X ", b)
    }
    puts
end

def doMain(fname)
    f = File.open(fname, "rb")
    
    png_sig = f.read(8)
    dumpString(png_sig)
    
    until f.eof do
        chunk_length = f.read(4).unpack("N")[0]
        chunk_type = f.read(4)
        chunk_data = f.read(chunk_length)
        chunk_crc = f.read(4)

        puts "[#{chunk_type}]"
        puts "Length=#{chunk_length}"
        print "CRC="
        dumpString(chunk_crc)
        
        if chunk_length > 0 then
            if chunk_type == "tEXt" then
                p chunk_data.split("\0")
            elsif chunk_type == "IDAT" then
                puts "--- image data ---"
            else
                dumpString(chunk_data)
            end
        end
        
        puts
    end
end

#

#doMain('C:\Users\kojima\Documents\temp\–rŒŽ[4]_1.png')
#doMain('C:\Users\kojima\Documents\temp\–rŒŽ[4]_2.png')
doMain(ARGV[0])
