space := $(empty) $(empty)
shift-list = $(wordlist 2,$(words $1),$1)
shift-path = $(subst $(space),/,$(call shift-list,$(subst /, ,$1)))

CC=gcc
GEN_DEPS := src/mon.h src/areas.h
DEPS := $(shell echo ./**/*.h ./**/**/*.h)
DEPS := $(GEN_DEPS)
SRC := $(shell echo ./**/*.c)
ODIR=obj
OBJ = $(foreach path,$(SRC),$(patsubst %.c,$(ODIR)/%.o,$(call shift-path,$(path))))
CFLAGS = -Wextra -Wall -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code -march=native -std=c99 -Ilib/include -Isrc

$(shell mkdir -p $(ODIR))
$(shell for word in ./**/*.c; do mkdir -p $(ODIR)/$$(dirname $$word); done)

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

bin/yaqp: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

src/mon.h: scripts/gen_mon_data.py
	python3 $< > $@

src/areas.h: scripts/gen_area_data.py
	python3 $< > $@

.PHONY: clean

clean:
	rm $(ODIR)/**/*.o
