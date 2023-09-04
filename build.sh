#!/bin/bash


rm -f server.exe
rm -f client.exe


g++ server.cpp -o server.exe -lpthread
g++ client.cpp -o client.exe -lpthread
