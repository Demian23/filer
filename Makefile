CPPC=clang++
COMMON_FLAGS=-Wall -Werror -Wshadow -Wconversion -Wpedantic -std=c++17 
DEBUG_FLAGS=-g
OPTIMISATION_FLAGS=-O3
SPECIFIC_FLAGS=$(DEBUG_FLAG)
MAIN_PATH=$(SRC)/main/main.cpp

SRC=src
OBJ=obj
OBJS=$(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SRCS))
SRCS=$(wildcard $(SRC)/*.cpp)
BINDIR=bin
BIN=$(BINDIR)/filer
# test dir -> T_D
T_D=tests
PLUGINS_D=plugins

all:$(OBJ) $(BINDIR) $(BIN)
release: SPECIFIC_FLAGS=$(OPTIMISATION_FLAGS)
release: all

# o -> just objects files
o: $(OBJS)

test: o
	$(MAKE) -C $(T_D)

all_test: test
	$(MAKE) test -C $(PLUGINS_D)

all_clean: clean
	$(MAKE) clean -C $(PLUGINS_D)
all_: release
	$(MAKE) -C $(PLUGINS_D)
$(BIN): $(OBJS)
	$(CPPC) $(COMMON_FLAGS) $(SPECIFIC_FLAGS) $^ $(MAIN_PATH) -o $@ 
$(OBJ)/%.o: $(SRC)/%.cpp
	$(CPPC) $(COMMON_FLAGS) $(SPECIFIC_FLAGS) -c $< -o $@

clean:
	$(RM) -r $(OBJ)/* $(BIN) $(BIN).dSYM
	$(MAKE) clean -C $(T_D)

$(OBJ):
	mkdir -p $@
$(BINDIR):
	mkdir -p $@
