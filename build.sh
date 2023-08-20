#!/bin/bash

rm -f ./client ./server

g++ -o ./client.exe ./client.cpp -lpthread
g++ -o ./server.exe ./server.cpp -lpthread
