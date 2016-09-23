require 'socket'
include Socket::Constants

READ_CONF = 'READ_CONF' 
WRITE_CONF = 'WRITE_CONF' 
GET_MEANSUREMENT = 'GET_MEANSUREMENT' 
SYNC = 'SYNC'

socket = Socket.new(AF_INET, SOCK_STREAM, 0)
sockaddr = Socket.sockaddr_in(5000, 'localhost')
socket.bind(sockaddr)
socket.listen(5)

begin # emulate blocking accept
  client_socket, client_addrinfo = socket.accept_nonblock
rescue IO::WaitReadable, Errno::EINTR
  IO.select([socket])
  retry
end
puts "The client said, '#{client_socket.readline.chomp}'"
client_socket.puts "Hello from script one!"
socket.close
