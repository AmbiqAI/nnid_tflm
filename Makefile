rootdir := .
include $(rootdir)/make/helpers.mk
include $(rootdir)/make/neuralspot_config.mk
include $(rootdir)/make/neuralspot_toolchain.mk

build := build
includes_api :=
pp_defines   := $(DEFINES)

# libraries := lib/nnid_8_lib.a
# libraries := lib/libtensorflow-microlite-cm4-gcc-release.a
# includes_api += $(rootdir)/neuralspot/ns-model/includes-api
includes_api += lib

include src/0264234_Nov_15_2023/module.mk
objects := $(build)/nnid_8_model.o
objects += $(build)/nnid_8_example.o
ifeq ($(TOOLCHAIN),arm)
objects += $(build)/armclang/startup_armclang.o
else
objects += $(build)/gcc/startup_gcc.o
endif

CFLAGS     += $(addprefix -D,$(pp_defines))
CFLAGS     += $(addprefix -I ,$(includes_api))

all: $(build) $(build)/nnid_8_example.bin $(build)/nnid_8_example.axf

$(build):
	$(Q) $(MKD) -p $@

lib/nnid_8_lib.a: $(build)/nnid_8_model.o
	@echo " Archiving $< to make $@"
	$(Q) $(AR) rsc $@ $^

$(build)/%.o: src/%.c
	@echo " Compiling $(COMPILERNAME) $< to make $@"
	$(Q) $(MKD) -p $(@D)
	$(Q) $(CC) -c $(CFLAGS) $(CONLY_FLAGS) $< -o $@

$(build)/%.o: src/%.cc
	@echo " Compiling $(COMPILERNAME) $< to make $@"
	$(Q) $(MKD) -p $(@D)
	$(Q) $(CC) -c $(CFLAGS) $(CCFLAGS) $< -o $@

$(build)/%.o: %.s
	@echo " Assembling $(COMPILERNAME) $<"
	$(Q) $(MKD) -p $(@D)
	$(Q) $(CC) -c $(ASMFLAGS) $< -o $@

$(build)/%.axf: $(objects) $(libraries)
	@echo " Linking $(COMPILERNAME) $@"
	$(Q) $(CC) -Wl,-T,src/linker_script.ld -o $@ $(objects) $(LFLAGS)

$(build)/%.bin: $(build)/%.axf
	@echo " Copying $(COMPILERNAME) $@..."
	$(Q) $(MKD) -p $(@D)
	$(Q) $(CP) $(CPFLAGS) $< $@
	$(Q) $(OD) $(ODFLAGS) $< > $*.lst
	$(Q) $(SIZE) $(objects) $(lib_prebuilt) $< > $*.size

.PHONY: clean
clean:
	$(Q) $(RM) -rf $(build)
	$(Q) $(RM) -f lib/nnid_8_lib.a
