@echo off
g++ bot/main.cpp -o bot.exe -L. -static -lwsock32 -lws2_32 -Wall
