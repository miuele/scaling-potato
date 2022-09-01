CXXSTD   := c++17

CXX      := clang++
#CXXFLAGS  = -std=$(CXXSTD) -fno-rtti -Wno-unused-variable -Wno-unused-but-set-variable -Wno-parentheses -Wall -Og -g -pedantic -fsanitize=address -fsanitize=leak
CXXFLAGS  = -std=$(CXXSTD) -fno-rtti -Wno-unused-variable -Wno-unused-but-set-variable -Wno-parentheses -Wall -O3
LDFLAGS   =
#LDLIBS    = $(LDFLAGS) -lstdc++ -lpthread -latomic -lm -fsanitize=address -fsanitize=leak
LDLIBS    = $(LDFLAGS) -lstdc++ -lpthread -latomic -lm

CPPTEST_SRC := cpptest.cpp features.cpp
CPPTEST_OBJ := $(CPPTEST_SRC:.cpp=.o)

all: cpptest
build: all

cpptest.o: cpptest.cpp features.hpp
features.o: features.cpp features.hpp

$(CPPTEST_OBJ): Makefile

cpptest: $(CPPTEST_OBJ)

clean:
	rm -f $(CPPTEST_OBJ) cpptest

rebuild: clean all

run: all
	@./cpptest

.PHONY: all clean rebuild run
