#This is a sample file.
#It lists common implemented classes and functions and shows how
#to use them. If you want a complete API reference check doc/ifpreference.html.

require 'ifp' 

#Initializes the player.
#If it is not possible to create the class an
#Interrupt exception is raised.
p = IFP::Player.new

#IFP.version returns an Array that holds 3 integer.
#The first number is the major the second the minor 
#and the third the micro version.
#Appending .join(".") makes it more readable. ( i.e MAJ.MIN.MIC )
p "Const: #{IFP.version.join(".")}"


p "p is of class: #{p.class}"

#The firmver function returns the raw version string.
rawver =  p.firmver
firmver = sprintf("%x.%02x", rawver/0x0100, rawver%0x100)
p "Firmware version: #{firmver}"


#model returns the model string.
x = p.model
p "Model: #{x}"

#capacity or get_capacity returns the maximum capacity of the device.
x = p.capacity.to_f
p "Capacity: #{x / (1024*1024).to_f} MB"

#free_space returns the free space on device.
x = p.free_space.to_f
p "Free Space: #{x / (1024*1024).to_f} MB"

#battery_state returns an integer from 0 to 4.
x =  sprintf("%x", p.battery_state)
p "Battery state: #{x}/4"

#Note: You can use "/" and "\\" for all file operations. Doesn't make any
#difference.

#rename(file1, file2) renames a file on the player.
#It's commented of course because you probably don't have those files.
#p.rename("/file1.ogg", "/file2.ogg")

#remove_file removes the file. Also commented for the same
#reason as rename()
#p.remove_file("/commanche.ogg")

#download_dir(remotedir, localdir) downloads remotedir and saves it as localdir.
#Optionally you can give a block and read some metadata for each file copied
#and the whole copying process.
#p.download_dir("/Diverses", "Diverses") do |name, bytes, total, fileno, filetotal|
    #p "#{name} #{bytes}/#{total} #{fileno} of #{filetotal}"
#end

#rmdir(dir)  #Removes dir from player. Returns true or false.

#directory?(dir) tests if dir exists and is a directory. Returns true if it is
#and false if not.
if p.directory?("testdir")
    p "testdir is a directory"
else
    p "testdir is not a directory"
end

#entries?(path) reads all entries of 'path'. It does however
#not collect all files and dirs recursively. Use treewalk_dirs for
#that matter instead.
#Use a block to collect the files since the method only returns
#true for success or false.
puts "Iter through dirs using IFP::Player.entries ?(y|n)"
input = gets.chomp

p.entries("/") do |name, size, type|
    p "#{name} #{type} #{size}"
end if input == "y" or input == "yes"

#treewalk_dirs(dir) starts from dir and walks through all subfolders
#recursively. It returns an array of files. Optionally you can also use
#a block to read metadata.
puts "Iter through dirs using IFP::Player.treewalk_dirs ?(y|n)"
input = gets.chomp

p.treewalk_dirs("/") do |name, size, type|
    p "#{name} #{type} #{size}"
end if input == "y" or input == "yes"

#Iter through all stations and print callsign and frequency.
puts "Iter through radio stations?(y|n)"
input = gets.chomp

i = 0
IFP::FM_STATIONS.times do
	stationary = p.get_station(i)
	puts "Radiostation ##{i}"
	puts "Callsign: #{stationary[0]}"
	puts "Frequency: #{stationary[1]}"
	i += 1
end if input == "y" or input == "yes"

#To set a station use set_station
#p.set_station(n, callsign, frequency)
