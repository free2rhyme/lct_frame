
SERVICES_LIST := lct_id_service

all debug release build rebuild:

	@for item in $(SERVICES_LIST); \
	do \
		$(MAKE) -C $$item $@ || exit "$$?"; \
	done

clean:
	@for item in $(SERVICES_LIST); \
	do \
		$(MAKE) -C $$item $@ ; \
	done
