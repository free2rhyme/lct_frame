
SERVICES_LIST := protobuf 
SERVICES_LIST += protobufcpp
SERVICES_LIST += lctprol

all debug release build rebuild:

	@for item in $(SERVICES_LIST); \
	do \
		$(MAKE) -C $$item $@ || exit "$$?"; \
	done

clean:
	@for item in $(SERVICES_LIST); \
	do \
		$(MAKE) -C $$item $@ ; \
		rm -fr $(BUILD_DIR) ; \
	done


