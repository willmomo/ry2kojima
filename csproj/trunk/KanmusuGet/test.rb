# p ENV['UserProfile'].gsub(File::ALT_SEPARATOR) {File::SEPARATOR}

fname_map = {}

#path1 = 'C:/Users/kojima/GoogleDrive/tools/ElectronicObserver/KCAPI/sound/**/[0-9][0-9][0-9][0-9][0-9][0-9].mp3'
path1 = ENV['UserProfile'] + '\GoogleDrive\tools\ElectronicObserver\KCAPI\sound\**\[0-9][0-9][0-9][0-9][0-9][0-9].mp3'
path1.gsub!(File::ALT_SEPARATOR) { File::SEPARATOR }
path2 = "./data/merge/"

Dir.glob(path1).each do |f|
	mp3name = File.basename(f)
	fname = File.basename(File.dirname(f))
	
	#puts fname
	#puts f
	
	if !fname_map.has_key?(fname)
		#puts "KanmusuGet.exe --json api_start2.json --api-filename #{fname}"
		ret = `KanmusuGet.exe --json api_start2.json --api-filename #{fname}`
		/.+=(.+)/ =~ ret
		fname_map[fname] = $1.strip
		puts "checking ... #{fname_map[fname]}"
	end

	mp3path = path2 + fname_map[fname].sub(" ", "_") + "/voice/*-" + mp3name
#	puts mp3path
#	Dir.glob(mp3path).each do |f2|
#		puts f2
#	end
	if Dir.glob(mp3path).empty?
		puts mp3path
		puts fname_map[fname]
		puts f
	end
end
