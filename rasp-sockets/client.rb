# In another script, start this second
require 'socket'
include Socket::Constants
socket = Socket.new(AF_INET, SOCK_STREAM, 0)
sockaddr = Socket.sockaddr_in(5000, 'localhost')
socket.connect(sockaddr)
socket.puts "Hello from script 2."
puts "The server said, '#{socket.readline.chomp}'"
socket.close
