dsrs_debug: 
	@echo "Compiling"
	@g++ main.cpp common.cpp util.cpp initialization.cpp initialTesting.cpp -Og -Wall -Wextra -Wpedantic -pthread -o dsrs
	@./dsrs
dsrs: 
	@echo "Compiling"
	@g++ main.cpp common.cpp util.cpp initialization.cpp initialTesting.cpp -Ofast -pthread -o dsrs
	@./dsrs
windows:
	@echo "Compiling"
	@x86_64-w64-mingw32-g++ main.cpp common.cpp util.cpp initialization.cpp initialTesting.cpp -Ofast -static -static-libgcc -static-libstdc++ -o dsrs.exe