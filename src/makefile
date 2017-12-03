
export LCT_SVC_PRJ_ROOT = $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/..)

VERSION_ARGS := phony
ifeq ($(firstword $(MAKECMDGOALS)), version)
	VERSION_ARGS := $(wordlist 2, $(words $(MAKECMDGOALS)), $(MAKECMDGOALS))
endif

all debug release build:
	$(MAKE) -C common   $@ || exit "$$?"
	$(MAKE) -C log      $@ || exit "$$?"
	$(MAKE) -C network  $@ || exit "$$?"
	$(MAKE) -C event    $@ || exit "$$?"
	$(MAKE) -C wrapper  $@ || exit "$$?"
	$(MAKE) -C service  $@ || exit "$$?"
	$(MAKE) -C tutorial $@ || exit "$$?"

clean rebuild:
	$(MAKE) -C version  $@ || exit "$$?"
	$(MAKE) -C 3rdparty $@ || exit "$$?"
	$(MAKE) -C protocol $@ || exit "$$?"
	$(MAKE) -C common   $@ || exit "$$?"
	$(MAKE) -C log      $@ || exit "$$?"
	$(MAKE) -C network  $@ || exit "$$?"
	$(MAKE) -C event    $@ || exit "$$?"
	$(MAKE) -C wrapper  $@ || exit "$$?"
	$(MAKE) -C tool     $@ || exit "$$?"
	$(MAKE) -C service  $@ || exit "$$?"
	$(MAKE) -C tutorial $@ || exit "$$?"

tool:
	$(MAKE) -C tool    all || exit "$$?"
	
free:
	$(MAKE) -C version all || exit "$$?"

version:
	-@../build/bin/version-tool $(VERSION_ARGS)

$(VERSION_ARGS):
	@cd .
	
rpm:
	@./tool/rpm/rpm_build.sh

.PHONY: tool free version rpm
