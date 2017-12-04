
SERVICES_LIST := lct_id_table_prep

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

	./script/rpm_cleanup.sh

