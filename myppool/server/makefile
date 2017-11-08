# ===============================
# Makefile for epoll
# ===============================

# ===============================
# Set include directories
INCLUDEDIR = -I ./inc 

# ===============================
# Compile flags
CPPFLAGS = 
CFLAGS = -Wall -std=c++11
CC = g++

# ===============================
# TARGET/SOURCE definitions
SRCDIR = ./src
TARGET = server
SRC = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(SRCDIR)/%.o, $(SRC))

# ===============================
# Make all option.
LDLIBS = 
LIBS = -l pthread

.PHONY: all #declare phony target
.PHONY: run
all: clean run
run: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(CPPFALGS) $(CFLAGS) $^ $(INCLUDEDIR) -g -o $@ $(LSLIBS) $(LIBS)
%.o: %.cpp
	$(CC) $(CPPFALGS) $(CFLAGS) $(INCLUDEDIR) -g -o $@ -c $< $(LSLIBS) $(LIBS)

.PHONY: clean
clean:
	rm -rf $(TARGET) $(OBJS)
