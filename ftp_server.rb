require 'ftpd'
require 'tmpdir'

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

dir = '/home/tallys/foo/bar'
FileUtils::mkdir_p dir

driver = Driver.new(dir)
server = Ftpd::FtpServer.new(driver)
server.port = 5000
server.start
puts "Server listening on port #{server.bound_port}"
gets
