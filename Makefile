CXXSTD   := c++17

CXX      := clang++
CXXFLAGS  = -std=$(CXXSTD) -fno-rtti -Wno-unused-variable -Wno-unused-but-set-variable -Wno-parentheses -Wall -O3
LDFLAGS   =
LDLIBS    = $(LDFLAGS) -lstdc++ -lpthread -latomic -lm

CPPTEST_SRC := test.cpp features.cpp
CPPTEST_OBJ := $(CPPTEST_SRC:.cpp=.o)

all: test
build: all

test.o: test.cpp features.hpp

$(CPPTEST_OBJ): Makefile

test: $(CPPTEST_OBJ)

clean:
	rm -f $(CPPTEST_OBJ) test

rebuild: clean all

run: all
	@./test

.PHONY: all clean rebuild run
