all:
	g++ main.cpp -o uart.exe
	sudo ./uart.exe
clean:
	sudo rm ./uart.exe
