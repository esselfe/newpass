CC ?= gcc
ifeq ($(DEV),1)
  CFLAGS = -std=c11 -Wall -Wextra -O0 -g
else
  CFLAGS = -std=c11 -Wall -O2
endif
LDFLAGS = 
PROGNAME = newpass

.PHONY: default all clean

default: all

all: $(PROGNAME)
	@ls -l --color=auto $(PROGNAME) 2>/dev/null || true

$(PROGNAME): newpass.c
	$(CC) $(CFLAGS) newpass.c -o $(PROGNAME) $(LDFLAGS)

clean:
	@rm -v $(PROGNAME)

