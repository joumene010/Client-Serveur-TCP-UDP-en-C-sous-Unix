#!/bin/bash

# Compile and link the client
gcc clientUDP.c -o clientUDP -Wall

# Compile and link the server
gcc serveurUDP.c -o serveurUDP -Wall

echo "Compilation and linking completed successfully."
echo "You can now run the client and server using:"
echo "./clientUDP <server_hostname> <server_port>"
echo "./serveurUDP <server_port>"

