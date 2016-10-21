require 'socket'
hostname = 'localhost'
port = 2000
socket = TCPSocket.new(hostname, port)

begin
	nome_arquivo = gets.chomp
	arquivo = File.new(nome_arquivo, "r")
	tamanho = File::size(nome_arquivo)
	bytes_arquivo = arquivo.read(tamanho)
	socket.puts(nome_arquivo)
	socket.puts(tamanho.to_s)
	socket.syswrite(bytes_arquivo)
ensure

	socket.close
	arquivo.close
end






=begin
socket.puts('soma 5 3')
puts socket.gets

socket.puts('raiz_quadrada 100')
puts socket.gets

socket.puts('maluco 5 3')
puts socket.gets
socket.close
=end



=begin
# In another script, start this second
require 'socket'
include Socket::Constants
socket = Socket.new(AF_INET, SOCK_STREAM, 0)
sockaddr = Socket.sockaddr_in(5000, 'localhost')
socket.connect(sockaddr)
socket.puts "Hello from script 2."
puts "The server said, '#{socket.readline.chomp}'"
socket.close
=end