require 'mkmf'

PACKAGE_NAME = "ifp"

req_deps = [ "usb", "ruby", "ifp" ]
req_deps.each do |dep|

    if !have_library(dep) or !have_header(dep + ".h")
        printf "!!!Error.\n"*2
        printf "\n#{dep} is an important dependancy.\n"
        exit
    end
end

create_makefile("ifp")
