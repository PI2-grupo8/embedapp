#!/bin/bash

	echo "Installing dependencies"
  apt-get update
  apt-get install curl iptables systemd ruby -y

	echo  "Installing Bundler"
  gem install bundler

	echo  "Installing FTPD"
  gem install ftpd

	echo  "Adding services to initd"
  mkdir -p /usr/share/tortuga
	cp tortuga-server.rb /usr/share/tortuga/
	cp tortuga-server-ftp.rb /usr/share/tortuga/
  cp tortuga-server.service /lib/systemd/system/
  cp tortuga-server-ftp.service /lib/systemd/system/
  cp tortuga-server /etc/init.d/
  cp tortuga-server-ftp /etc/init.d/
	cp tortuga-navigation /usr/share/tortuga
	
