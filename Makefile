BINDIR=bin
BIN=$(BINDIR)/filer
PLUGINS_D=plugins
MAIN_PATH=src/main/main.cpp

-include MakeCommon

all_test: t
	$(MAKE) t -C $(PLUGINS_D)

all_clean: clean
	$(MAKE) clean -C $(PLUGINS_D)
all_: release
	$(MAKE) -C $(PLUGINS_D)
