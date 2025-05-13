CC = g++

debug:
	$(CC) main.cpp -o sudocker -g

release:
	$(CC) main.cpp -o sudoker -O3
