CC = g++
CFLAGS = -O0 -Wall -mavx2 -fopenmp

SRC = $(wildcard *.cpp)
TARGET = $(SRC:.cpp=)

all: $(TARGET)

fall_through: CFLAGS = -O2 -Wall 
unroll: CFLAGS = -O1 -Wall 

%: %.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -f $(TARGET)