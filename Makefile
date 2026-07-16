CC       = gcc
DEBUG   ?= 0

OPT      = -Os
STRIP_FLAG = -s

CFLAGS   = -Wall -Wextra -std=c89 $(OPT) -MMD -MP \
           -fno-asynchronous-unwind-tables -fno-ident \
           -ffunction-sections -fdata-sections

LDFLAGS  = -lm -Wl,--gc-sections $(STRIP_FLAG)

ifeq ($(DEBUG), 1)
    OPT        = -O0 -g
    STRIP_FLAG = 
    CFLAGS     = -Wall -Wextra -std=c89 $(OPT) -MMD -MP -DDEBUG
    LDFLAGS    = -lm
endif

# Проверяем, передан ли аргумент MODE, и добавляем его как -DMODE
ifdef MODE
    CFLAGS += -D$(MODE)
endif

IFLAGS   = -Isrc
SRCDIR   = src
BUILDDIR = build

SOURCES  = $(shell find $(SRCDIR) -name "*.c")
SRC_SUBDIRS = $(shell find $(SRCDIR) -type d)
vpath %.c $(SRC_SUBDIRS)

OBJECTS  = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SOURCES))
DEPS     = $(OBJECTS:.o=.d)

TARGET   = main

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $^ $(LDFLAGS) -o $@

$(BUILDDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR) $(TARGET)

-include $(DEPS)
