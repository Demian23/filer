CPPC=clang++
CPPFLAGS=-g -Wall -Werror -Wshadow -Wconversion -Wpedantic -std=c++14
SAN_ADDR=-fsanitize=address 
SAN_MEM=-fsanitize=memory
SRC=src
OBJ=obj
OBJ_RLE=../obj/

OBJS=$(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SRCS))
OBJS_RLE=$(wildcard $(OBJ_RLE)/*.o)

SRCS=$(wildcard $(SRC)/*.cpp)
BINDIR=bin
BIN=$(BINDIR)/rletest
LIBTEST=-lgtest

all:clean $(OBJ) $(BINDIR) $(BIN) run

addr: SANITIZE=$(SAN_ADDR)
addr: all
mem: SANITIZE=$(SAN_MEM)
mem: all

run: 
	./$(BIN)

$(BIN): $(OBJS_RLE) $(OBJS)
	$(CPPC) $(CPPFLAGS) $(SANITIZE) $(LIBTEST) $^ -o $@ 

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CPPC) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) -r $(OBJ)/* $(BIN) $(BIN).dSYM

$(OBJ):
	mkdir -p $@
$(BINDIR):
	mkdir -p $@
