require 'fileutils'
require 'ftpd'

class Driver

  def initialize(temp_dir)
    @temp_dir = temp_dir
  end

  def authenticate(user, password)
    true
  end

  def file_system(user)
    Ftpd::DiskFileSystem.new(@temp_dir)
  end
end

dir = '/usr/share/tortuga/'
FileUtils::mkdir_p dir

driver = Driver.new(dir)
server = Ftpd::FtpServer.new(driver)
server.port = 5000
server.interface = '0.0.0.0'
server.start
puts "Server listening on port #{server.bound_port}"
loop {sleep 90}
