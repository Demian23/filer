PLUGINSDIRS=$(shell ls -d */)

define MAKERELEASE
$(MAKE) release -C $(1);
endef

define MAKEDEBUG
$(MAKE) -C $(1);
endef

define MAKETEST 
$(MAKE) t -C $(1);
endef

define MAKECLEAN
$(MAKE) clean -C $(1);
endef

all: $(PLUGINSDIRS)
	$(foreach plugin, $^, $(call  MAKERELEASE, $(plugin)))
debug: $(PLUGINSDIRS)
	$(foreach plugin, $^, $(call  MAKEDEBUG, $(plugin)))
t: $(PLUGINSDIRS)
	$(foreach plugin, $^, $(call  MAKETEST, $(plugin)))
clean:$(PLUGINSDIRS)
	$(foreach plugin, $^, $(call  MAKECLEAN, $(plugin)))
