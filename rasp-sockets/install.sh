#!/bin/bash

	echo "Installing dependencies"
  apt-get install iptables
  sudo apt-get install curl -y
  curl -sSL https://rvm.io/mpapis.asc | gpg --import -
  \curl -sSL https://get.rvm.io | sudo bash -s stable --ruby

	echo "Sourcing RVM: "
  source ~/.rvm/scripts/rvm

	echo  "Installing Bundler"
  gem install bundler

	echo  "Installing FTPD"
  gem install ftpd

	echo  "Adding services to initd"
  mkdir -p /usr/share/tortuga
	cp tortuga-server.rb /usr/share/tortuga/
  cp tortuga-server.service /lib/systemd/system/
  cp tortuga-server /etc/init.d/
	
