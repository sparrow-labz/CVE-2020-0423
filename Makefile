# toolchain path
TOOLCHAIN=
# Only choose one of these, depending on your device...
TARGET64=aarch64-linux-android-gcc
# 32 bix x86
# CC=~/Android/Sdk/ndk/26.2.11394342/toolchains/llvm/prebuilt/linux-x86_64/bin/i686-linux-android29-clang 
CC=~/Android/Sdk/ndk/26.2.11394342/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android26-clang

all:
	# 32 bit poc
	$(CC) 2020-0423.c -o 2020-0423 -g -pthread
	$(CC) 2020-0423_spray1.c -o spray1 -g -pthread


clean:
	rm 2020-0423
	rm spray1
