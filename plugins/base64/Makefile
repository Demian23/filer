CPPC=clang++
COMMON_FLAGS=-Wall -Werror -Wshadow -Wconversion -Wpedantic -std=c++17
DEBUG_FLAGS=-g
OPTIMISATION_FLAGS=-O3
SPECIFIC_FLAGS=$(DEBUG_FLAG)

SRC=src
OBJ=obj
OBJS=$(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SRCS))
SRCS=$(wildcard $(SRC)/*.cpp)
BINDIR=../bin
BIN=$(BINDIR)/base64.so
# test dir -> T_D
T_D=test

all:$(OBJ) $(BINDIR) $(BIN)
release: SPECIFIC_FLAGS=$(OPTIMISATION_FLAGS)
release: all

# o -> just objects files
o: $(OBJS)

test: o
	$(MAKE) -C $(T_D)

$(BIN): $(OBJS)
	$(CPPC) $(COMMON_FLAGS) $(SPECIFIC_FLAGS) -shared $^ $(MAIN_PATH) -o $@ 
$(OBJ)/%.o: $(SRC)/%.cpp
	$(CPPC) $(COMMON_FLAGS) $(SPECIFIC_FLAGS) -fPIC -c $< -o $@

clean:
	$(RM) -r $(OBJ)/* $(BIN) $(BIN).dSYM
	$(MAKE) clean -C $(T_D)

$(OBJ):
	mkdir -p $@
