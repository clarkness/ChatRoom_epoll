# ===============================
# Makefile for epoll
# ===============================

# ===============================
# Set include directories
INCLUDEDIR = 

# ===============================
# Compile flags
CPPFLAGS = 
CFLAGS = -std=c++11
CC = g++

# ===============================
# TARGET/SOURCE definitions
SRCDIR = .
TARGET = server
SRC = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/*.cpp, $(SRCDIR)/*.o, $(SRC))

# ===============================
# Make all option.
LDLIBS = 
LIBS = -l pthread

.PHONY:all #declare phony target
#all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(CPPFALGS) $(CFLAGS) &^ -g -o $@ $(LSLIBS) $(libs)
%.o: %.c
	$(CC) $(CPPFALGS) $(CFLAGS) &< -g -c $@ $(LSLIBS) $(libs)





rm:
	rm -f server customer
server:
	g++ -std=c++11 -o server server.cpp -lpthread
run: 
	g++ -std=c++11 -o  customer customer.cpp
all:rm run 
