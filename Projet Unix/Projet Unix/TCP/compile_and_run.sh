#!/bin/bash

# Compile and link the client
gcc clientTCP.c -o clientTCP -Wall

# Compile and link the server
gcc serveurTCP.c -o serveurTCP -Wall

echo "Compilation and linking completed successfully."
echo "You can now run the client and server using:"
echo "./clientTCP <server_hostname> <server_port>"
echo "./serveurTCP <server_port>"

