#!/bin/bash

	echo "Installing dependencies"
  #apt-get install iptables
  #sudo apt-get install curl -y
  #curl -sSL https://rvm.io/mpapis.asc | gpg --import -
  #\curl -sSL https://get.rvm.io | sudo bash -s stable --ruby

	echo "Sourcing RVM: "
	source /etc/profile

	echo "If sudo can not use ruby, log in as sudo and source /etc/profile"

	echo  "Installing Bundler"
  #gem install bundler

	echo  "Installing FTPD"
  #gem install ftpd

	echo  "Adding services to initd"
  mkdir -p /usr/share/tortuga
	cp tortuga-server.rb /usr/share/tortuga/
  cp tortuga-server.service /lib/systemd/system/
  cp tortuga-server /etc/init.d/
	cp tortuga-navigation /usr/share/tortuga
	
