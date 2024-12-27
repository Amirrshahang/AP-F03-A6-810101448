CXX = g++
CXXFLAGS = -std=c++20 -I./include -Wall -Wextra -fsanitize=address -fsanitize=undefined -Wno-unused-parameter

SRCDIR = src
OBJDIR = obj
BINDIR = .
EXECUTABLE = $(BINDIR)/UTaste

SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)/*.o $(EXECUTABLE)
	