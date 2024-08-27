CC = g++
CFLAGS =


# Target executable
TARGET = CapstoneGroupCalculator

# Source files
SRCS = main.cpp FilterFunctions.cpp PositiveTable.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

# Link object files to creaste executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile source files into objecr files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $< -o $@

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(TARGET)