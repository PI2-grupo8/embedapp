#!/bin/bash

  sudo apt-get install curl -y
  curl -sSL https://rvm.io/mpapis.asc | gpg --import -
  \curl -sSL https://get.rvm.io | sudo bash -s stable --ruby
  source /home/$USER/.rvm/scripts/rvm
  gem install bundler
  gem install ftpd
  apt-get install iptables
	echo "========="
	echo $0
  mkdir -p /usr/share/tortuga
	cp tortuga-server.rb /usr/share/tortuga/
  cp tortuga-server.service /lib/systemd/system/
  cp tortuga-server /etc/init.d/
  source ~/.rvm/scripts/rvm
	
