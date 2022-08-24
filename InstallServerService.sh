#!/bin/sh

#******************************************************************************
# Copyright (C) 2022 by Mouadh Dahech - Focus Corporation 
#
#*****************************************************************************

echo "\n*** Sytemd MyServer.service creation start ***\n"

cwd=$(pwd)
exdir=$cwd/build/Server
echo "Successfully Get the server directory\n"
sed -i -e'4d' MyServer.service 
sed -i "4i ExecStart=$exdir" MyServer.service
echo "Successfully create Mysever.service file\n"
sudo cp MyServer.service /etc/systemd/system/
sudo systemctl enable MyServer.service
echo "Successfully create systemd service\n"
sudo systemctl start MyServer.service
echo "Successfully start the service\n"
echo "MyServer.service status:\n"
sudo systemctl status MyServer.service
echo "MyServer.service log:\n"
sudo journalctl -f -a -u MyServer.service
