#!/bin/bash

rm -f ./client ./server

gcc -o ./client ./client.c
gcc -o ./server ./server.c