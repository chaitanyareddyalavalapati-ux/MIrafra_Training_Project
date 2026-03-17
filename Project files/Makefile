# Compiler and standard
CXX      = g++
CXXFLAGS = -std=c++17 -Wall

TARGET = sky_airlines

# All source files
SRCS = main.cpp \
	globals.cpp \
	Flight_route.cpp \
	admin.cpp \
	user.cpp \
	Graphviz.cpp

# Object files (auto-generated from SRCS)
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)
	@echo " Run: ./$(TARGET)"

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
	@echo "  Cleaned."
