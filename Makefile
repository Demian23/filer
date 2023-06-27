CPPC=clang++
CPPFLAGS=-g -Wall -Werror -Wshadow -Wconversion -Wpedantic -std=c++14
SRC=src
OBJ=obj
OBJS=$(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SRCS))
SRCS=$(wildcard $(SRC)/*.cpp)
BINDIR=bin
BIN=$(BINDIR)/filer

all:$(OBJ) $(BINDIR) $(BIN)
temp: $(OBJ) $(OBJS)

$(BIN): $(OBJS_FILER) $(OBJS)
	$(CPPC) $(CPPFLAGS) $^ $(SRC)/main/main.cpp -o $@ 
$(OBJ)/%.o: $(SRC)/%.cpp
	$(CPPC) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) -r $(OBJ)/* $(BIN) $(BIN).dSYM

$(OBJ):
	mkdir -p $@
$(BINDIR):
	mkdir -p $@
