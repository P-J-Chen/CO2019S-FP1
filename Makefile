CXX = g++ -std=c++11
TARGET = max_overlap

all: code.cpp
	$(CXX) $< -o $(TARGET)

.PHONY: clean run

clean:
	rm -f $(TARGET) 

run:
	./$(TARGET)
