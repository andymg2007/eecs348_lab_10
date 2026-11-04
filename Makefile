CXX      = g++
CXXFLAGS = -Wall -Wextra -std=c++11
TARGET   = matrix_lab

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.cpp

clean:
	rm -f $(TARGET)

.PHONY: all clean
