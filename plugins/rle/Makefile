CPPC=clang++
CPPFLAGS=-g -Wall -Werror -Wshadow -Wconversion -Wpedantic -std=c++14
SRC=src
OBJ=obj/
OBJS=$(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SRCS))
SRCS=$(wildcard $(SRC)/*.cpp)

BINDIR=../bin
BIN=$(BINDIR)/rle.so

all:$(OBJ) $(BIN)

release: CPPFLAGS=-Wall -Werror -Wshadow -Wconversion -Wpedantic -std=c++14 -O3
release: all

object: $(OBJS)

test: object
	$(MAKE) -C test

$(BIN): $(OBJS)
	$(CPPC) -shared $(CPPFLAGS) $^ -o $@ 

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CPPC) -fPIC $(CPPFLAGS) -c $< -o $@
clean:
	$(RM) -r $(OBJ)/* $(BIN)
$(OBJ):
	mkdir -p $@
