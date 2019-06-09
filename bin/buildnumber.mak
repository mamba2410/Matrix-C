
BUILD_DATE = $$(date +'%Y%m%d')
BUILD_NUMBER = $$(cat $(BUILD_NUMBER_FILE))

# Currently does not print proper numbers with source file as
# extern char __BUILD_NUMBER;
# extern char __BUILD_DATE;
# ..
# printf("Build date\t: %u\n", (unsigned long int) &__BUILD_DATE);
# printf("Build number\t: %u\n", (unsigned long int) &__BUILD_NUMBER);
#
BUILD_NUMBER_LDFLAGS=
#BUILD_NUMBER_LDFLAGS =  -Xlinker --defsym -Xlinker __BUILD_DATE=$(BUILD_DATE)
#BUILD_NUMBER_LDFLAGS += -Xlinker --defsym -Xlinker __BUILD_NUMBER=$(BUILD_NUMBER)

$(BUILD_NUMBER_FILE): $(OBJECTS)
	@if ! test -f $(BUILD_NUMBER_FILE); then echo 0 > $(BUILD_NUMBER_FILE); fi
	@echo $$(( $$(cat $(BUILD_NUMBER_FILE)) + 1)) > $(BUILD_NUMBER_FILE)
