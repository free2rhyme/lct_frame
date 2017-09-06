
export LCT_SVC_PRJ_ROOT = $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/..)

all debug release build:
	$(MAKE) -C log      $@ || exit "$$?"
	$(MAKE) -C common   $@ || exit "$$?"
#	$(MAKE) -C wrapper  $@ || exit "$$?"
	$(MAKE) -C network  $@ || exit "$$?"
	$(MAKE) -C wrapper  $@ || exit "$$?"
	$(MAKE) -C event    $@ || exit "$$?"
#	$(MAKE) -C model    $@ || exit "$$?"
#	$(MAKE) -C service  $@ || exit "$$?"
	$(MAKE) -C tutorial $@ || exit "$$?"

clean rebuild:
#	$(MAKE) -C 3rdparty $@ || exit "$$?"
	$(MAKE) -C protocol $@ || exit "$$?"
	$(MAKE) -C log      $@ || exit "$$?"
	$(MAKE) -C common   $@ || exit "$$?"
	$(MAKE) -C network  $@ || exit "$$?"
	$(MAKE) -C wrapper  $@ || exit "$$?"
	$(MAKE) -C event    $@ || exit "$$?"
#	$(MAKE) -C model    $@ || exit "$$?"
#	$(MAKE) -C service  $@ || exit "$$?"
	$(MAKE) -C tutorial $@ || exit "$$?"


