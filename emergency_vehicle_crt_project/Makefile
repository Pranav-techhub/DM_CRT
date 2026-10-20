CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2
TARGET = emergency_crt

all: $(TARGET)

$(TARGET): src/main.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) src/main.cpp

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
