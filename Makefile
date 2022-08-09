dsrs: 
	@echo "Compiling"
	@g++ main.cpp common.cpp util.cpp initialization.cpp -O2 -Werror -Wall -Wextra -Wpedantic -Wformat=2 -Wformat-overflow=2 -Wformat-truncation=2 -Wformat-security -Wnull-dereference -Wstack-protector -Wtrampolines -Walloca -Wvla -Warray-bounds=2 -Wimplicit-fallthrough=3 -Wshift-overflow=2 -Wcast-qual -Wstringop-overflow=4 -Wconversion -Wlogical-op -Wduplicated-cond -Wduplicated-branches -Wformat-signedness -Wshadow -Wstrict-overflow=4 -Wundef -Wswitch-default -Wswitch-enum -Wstack-usage=1000000 -Wcast-align=strict -D_FORTIFY_SOURCE=2 -fstack-protector-strong -fstack-clash-protection -fPIE  -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack -Wl,-z,separate-code -o dsrs
	@./dsrs
windows:
	@echo "Compiling"
	@x86_64-w64-mingw32-g++ main.cpp common.cpp util.cpp initialization.cpp -static -static-libgcc -static-libstdc++ -o dsrs.exe