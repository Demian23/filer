CPPC=clang++
COMMON_FLAGS=-Wall -Werror -Wshadow -Wconversion -Wpedantic -std=c++17 -g
ADDRSAN=-fsanitize=address
SRC=src
OBJ=obj
OBJ_TESTED=../obj/
SRCS=$(wildcard $(SRC)/*.cpp)
OBJS=$(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SRCS))
OBJS_TESTED=$(wildcard $(OBJ_TESTED)/*.o)
BINDIR=bin
BIN=$(BINDIR)/base64Test
LIBS=-lgtest

all:clean $(OBJ) $(BINDIR) $(BIN) run
addr:SPECIFIC_FLAGS=$(ADDRSAN)
addr:all

run: 
	./$(BIN)

$(BIN): $(OBJS_TESTED) $(OBJS)
	$(CPPC) $(COMMON_FLAGS) $(SPECIFIC_FLAGS) $(LIBS) $^ -o $@ 

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CPPC) $(COMMON_FLAGS) $(SPECIFIC_FLAGS) -c $< -o $@

clean:
	$(RM) -r $(OBJ)/* $(BIN) $(BIN).dSYM

$(OBJ):
	mkdir -p $@
$(BINDIR):
	mkdir -p $@
