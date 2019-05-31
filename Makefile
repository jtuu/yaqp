# helpers
space := $(empty) $(empty)
shift-list = $(wordlist 2,$(words $1),$1)
shift-path = $(subst $(space),/,$(call shift-list,$(subst /, ,$1)))

CC := gcc

GEN_SRC := \
./src/mon.c \
./src/mon.h
# end GEN_SRC

GEN_DEPS := \
./src/mon.h \
./src/areas.h
# end GEN_DEPS

DEPS := \
$(GEN_DEPS) \
./src/areas.h \
./src/bin.h \
./src/bp.h \
./src/dat.h \
./src/items.h \
./src/list.h \
./src/mon.h \
./src/obj.h \
./src/qst.h \
./src/asm.h \
./src/disasm.h \
./src/utils.h \
./lib/include/PRS-decomp.h \
./lib/include/psoarchive-error.h
# end DEPS

SRC := \
$(GEN_SRC) \
./lib/PRS-decomp.c \
./src/bin.c \
./src/bp.c \
./src/dat.c \
./src/list.c \
./src/obj.c \
./src/qst.c \
./src/asm.c \
./src/disasm.c \
./src/utils.c \
./src/yaqp.c
# end SRC

# find corresponding object files for source files
ODIR := obj
OBJ := $(foreach path,$(SRC),$(patsubst %.c,$(ODIR)/%.o,$(call shift-path,$(path))))

CFLAGS_DIAG := \
-Werror \
-fmax-errors=5 \
-Wextra \
-Wall \
-Wundef \
-Wshadow \
-Wpointer-arith \
-Wcast-align \
-Wstrict-prototypes \
-Wstrict-overflow=5 \
-Wwrite-strings \
-Waggregate-return \
-Wcast-qual \
-Wswitch-default \
-Wconversion \
-Wunreachable-code
# end CFLAGS_DIAG

CFLAGS := \
-march=native \
-std=c99 \
-Ilib/include \
-Isrc -g
# end CFLAGS

# link
bin/yaqp: $(OBJ) $(DEPS)
	$(CC) -o $@ $^ $(CFLAGS)

# compile src
$(ODIR)/src/%.o: src/%.c
	@mkdir -p $(ODIR)/src
	$(CC) -c -o $@ $< $(CFLAGS_DIAG) $(CFLAGS)

# compile lib
$(ODIR)/lib/%.o: lib/%.c
	@mkdir -p $(ODIR)/lib
	$(CC) -c -o $@ $< $(CFLAGS)

# generate sources
src/mon.h src/mon.c: scripts/gen_mon_data.py scripts/common.py
	python3 $< $@

src/areas.h src/areas.c: scripts/gen_area_data.py scripts/common.py
	python3 $< $@

.PHONY: clean

clean:
	rm $(ODIR)/**/*.o
