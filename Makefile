space := $(empty) $(empty)
shift-list = $(wordlist 2,$(words $1),$1)
shift-path = $(subst $(space),/,$(call shift-list,$(subst /, ,$1)))

CC := gcc
GEN_DEPS := src/mon.h src/areas.h
DEPS := $(shell echo ./**/*.h ./**/**/*.h) $(GEN_DEPS)
SRC := $(shell echo ./**/*.c)
ODIR := obj
OBJ := $(foreach path,$(SRC),$(patsubst %.c,$(ODIR)/%.o,$(call shift-path,$(path))))
CFLAGS_DIAG := -Werror -fmax-errors=5 -Wextra -Wall -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code
CFLAGS := -march=native -std=c99 -Ilib/include -Isrc

$(shell mkdir -p $(ODIR))
$(shell for word in ./**/*.c; do mkdir -p $(ODIR)/$$(dirname $$word); done)

bin/yaqp: $(OBJ) $(DEPS)
	$(CC) -o $@ $^ $(CFLAGS)

$(ODIR)/src/%.o: src/%.c
	$(CC) -c -o $@ $< $(CFLAGS_DIAG) $(CFLAGS)

$(ODIR)/lib/%.o: lib/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

src/mon.h: scripts/gen_mon_data.py
	python3 $< > $@

src/areas.h: scripts/gen_area_data.py
	python3 $< > $@

.PHONY: clean

clean:
	rm $(ODIR)/**/*.o
