CXX = g++
CXXFLAGS = -Wall -g -std=c++11
INCLUDES = -Iinclude
TARGET = dormitory
LDFLAGS = -static

SRCS = main.cpp src/fileio.cpp src/search.cpp src/sort.cpp src/ui.cpp
OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
