#!/usr/bin/env ruby -w
require "socket"
class Server
  def initialize( port, ip )
    @server = TCPServer.open( ip, port )
    @navigation_path = "./a.out"
    run
  end

  def run
    loop {
      client = @server.accept
      client.puts "Connection established!"
      listen_user_messages(client)
    }
  end

  def listen_user_messages(client)
    loop {
      begin
        msg = client.gets.chomp
        puts "MESAGE: #{msg}\n"

        case msg

        when "close"
          client.puts "closing"
          client.close
          break
        when "start"
          puts "start navigation"
          exec(@navigation)
          break
        else
        end
      rescue 
        break
      end
    }
  end
end

Server.new( 3000, "localhost" )
