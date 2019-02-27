@echo off
g++ -c Deck.cpp -o Deck.o
g++ -c Game.cpp -o Game.o
g++ -c Main.cpp -o Main.o
g++ -c RingIndex.cpp -o RingIndex.o
g++ Main.o Game.o Deck.o RingIndex.o -o Schnapsen.exe