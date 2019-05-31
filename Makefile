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
OBJ_RELEASE := $(foreach path,$(SRC),$(patsubst %.c,$(ODIR)/release/%.o,$(call shift-path,$(path))))
OBJ_DEBUG := $(foreach path,$(SRC),$(patsubst %.c,$(ODIR)/debug/%.o,$(call shift-path,$(path))))

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
-Isrc
# end CFLAGS

CFLAGS_DEBUG := -g

.DEFAULT_GOAL: bin/release/yaqp

# link release
bin/release/yaqp: $(OBJ_RELEASE) $(DEPS)
	@mkdir -p bin/release
	$(CC) -o $@ $^ $(CFLAGS)

# compile src release
$(ODIR)/release/src/%.o: src/%.c
	@mkdir -p $(ODIR)/release/src
	$(CC) -c -o $@ $< $(CFLAGS_DIAG) $(CFLAGS)

# compile lib release
$(ODIR)/release/lib/%.o: lib/%.c
	@mkdir -p $(ODIR)/release/lib
	$(CC) -c -o $@ $< $(CFLAGS)

debug: bin/debug/yaqp

# link debug
bin/debug/yaqp: $(OBJ_DEBUG) $(DEPS)
	@mkdir -p bin/debug
	$(CC) -o $@ $^ $(CFLAGS) $(CFLAGS_DEBUG)

# compile src debug
$(ODIR)/debug/src/%.o: src/%.c
	@mkdir -p $(ODIR)/debug/src
	$(CC) -c -o $@ $< $(CFLAGS_DIAG) $(CFLAGS) $(CFLAGS_DEBUG)

# compile lib debug
$(ODIR)/debug/lib/%.o: lib/%.c
	@mkdir -p $(ODIR)/debug/lib
	$(CC) -c -o $@ $< $(CFLAGS) $(CFLAGS_DEBUG)

# generate sources
src/mon.h src/mon.c: scripts/gen_mon_data.py scripts/common.py
	python3 $< $@

src/areas.h src/areas.c: scripts/gen_area_data.py scripts/common.py
	python3 $< $@

.PHONY: clean

clean:
	rm $(ODIR)/**/**/*.o
