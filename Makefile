CFLAGS= -Wall -Werror -Wextra -std=c++17 -pedantic -I ./src
SRC= src/main.cc src/parse-driver.cc src/scan-bistro.cc src/parse-bistro.cc
OBJS= src/main.o src/parse-driver.o src/scan-bistro.o src/parse-bistro.o
all: parse bistro

bistro: $(OBJS)
	g++ $(CFLAGS) $^ -o $@

parse:
	flex -f -o src/scan-bistro.cc src/scan-bistro.ll
	bison src/parse-bistro.yy -o src/parse-bistro.cc --defines=src/parse-bistro.hh

clean:
	rm src/*.o bistro
