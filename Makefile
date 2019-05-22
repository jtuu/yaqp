space := $(empty) $(empty)
shift-list = $(wordlist 2,$(words $1),$1)
shift-path = $(subst $(space),/,$(call shift-list,$(subst /, ,$1)))

CC := gcc
GEN_DEPS := src/mon.h src/areas.h
DEPS := $(GEN_DEPS) ./src/areas.h ./src/bin.h ./src/bp.h ./src/dat.h ./src/items.h ./src/list.h ./src/mon.h ./src/obj.h ./src/qst.h ./src/utils.h ./lib/include/PRS-decomp.h ./lib/include/psoarchive-error.h
SRC := ./lib/PRS-decomp.c ./src/bin.c ./src/dat.c ./src/list.c ./src/obj.c ./src/qst.c ./src/utils.c ./src/yaqp.c
ODIR := obj
OBJ := $(foreach path,$(SRC),$(patsubst %.c,$(ODIR)/%.o,$(call shift-path,$(path))))
CFLAGS_DIAG := -Werror -fmax-errors=5 -Wextra -Wall -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -Wcast-qual -Wswitch-default -Wconversion -Wunreachable-code
CFLAGS := -march=native -std=c99 -Ilib/include -Isrc -g

bin/yaqp: $(OBJ) $(DEPS)
	$(CC) -o $@ $^ $(CFLAGS)

$(ODIR)/src/%.o: src/%.c
	@mkdir -p $(ODIR)/src
	$(CC) -c -o $@ $< $(CFLAGS_DIAG) $(CFLAGS)

$(ODIR)/lib/%.o: lib/%.c
	@mkdir -p $(ODIR)/lib
	$(CC) -c -o $@ $< $(CFLAGS)

src/mon.h: scripts/gen_mon_data.py
	python3 $< > $@

src/areas.h: scripts/gen_area_data.py
	python3 $< > $@

.PHONY: clean

clean:
	rm $(ODIR)/**/*.o
