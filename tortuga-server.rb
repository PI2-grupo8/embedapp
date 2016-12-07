#!/usr/bin/env ruby -w

require "socket"

class Server

  def CMD_START
    10
  end

  def CMD_CLOSe
    11
  end

  def initialize( port, ip )
    @server = TCPServer.open( ip, port )
    @navigation_path = "./a.out"
    run
  end

  def run
    loop {
      begin
        client = @server.accept
        client.puts "Connection established!"
        listen_user_messages(client)
      rescue
      end
    }
  end

  def listen_user_messages(client)
    loop {
      begin
        msg = client.gets.chomp
        puts "MESAGE: #{msg}\n"

        case msg

        when "11"
          client.close
          break
        when "10"
          client.puts "kill"
          exec("/usr/share/tortuga/tortuga-navigation")
          break
        else
        end
      rescue 
        break
      end
    }
  end
end

Server.new( 4000, "0.0.0.0" )
