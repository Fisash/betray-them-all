CC = gcc
OPT = -O2
STRIP_FLAG = -s

CFLAGS = -Wall -Wextra -std=c89 -MMD -MP \
         -fno-asynchronous-unwind-tables -fno-ident \
         -ffunction-sections -fdata-sections

ifdef DEBUG
OPT = -O0
STRIP_FLAG =
CFLAGS += -g -DDEBUG
endif

CFLAGS += $(OPT) -Isrc
LDFLAGS = -lm -Wl,--gc-sections $(STRIP_FLAG)

SRCDIR   = src
BUILDDIR = build

CORE_SRC = $(shell find $(SRCDIR)/core -name '*c')
CORE_OBJ = $(patsubst $(SRCDIR)/core/%.c,$(BUILDDIR)/core/%.o,$(CORE_SRC))

GUI_FRONTEND_SRC = $(shell find $(SRCDIR)/gui/gui-frontend/ -name '*.c')
GUI_FRONTEND_OBJ = $(patsubst $(SRCDIR)/gui/gui-frontend/%.c,$(BUILDDIR)/gui/frontend/%.o,$(GUI_FRONTEND_SRC))

CLI_SRC = $(wildcard $(SRCDIR)/cli/*.c)
CLI_OBJ = $(patsubst $(SRCDIR)/cli/%.c,$(BUILDDIR)/cli/%.o,$(CLI_SRC))
MAIN_CLI_OBJ = $(BUILDDIR)/cli/main.o

XLIB_PORT_SRC = $(wildcard $(SRCDIR)/gui/xlib-port/*.c)
XLIB_PORT_OBJ = $(patsubst $(SRCDIR)/gui/xlib-port/%.c,$(BUILDDIR)/xlib/port/%.o,$(XLIB_PORT_SRC))
MAIN_XLIB_OBJ = $(BUILDDIR)/xlib/main.o

$(CORE_OBJ): $(BUILDDIR)/core/%.o: $(SRCDIR)/core/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(GUI_FRONTEND_OBJ): $(BUILDDIR)/gui/frontend/%.o: $(SRCDIR)/gui/gui-frontend/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(CLI_OBJ): CFLAGS += -DCLI
$(CLI_OBJ): $(BUILDDIR)/cli/%.o: $(SRCDIR)/cli/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(MAIN_CLI_OBJ): CFLAGS += -DCLI
$(MAIN_CLI_OBJ): $(SRCDIR)/main.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(XLIB_PORT_OBJ): CFLAGS += -DXLIB
$(XLIB_PORT_OBJ): $(BUILDDIR)/xlib/port/%.o: $(SRCDIR)/gui/xlib-port/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(MAIN_XLIB_OBJ): CFLAGS += -DXLIB
$(MAIN_XLIB_OBJ): $(SRCDIR)/main.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

DEPS = $(CORE_OBJ:.o=.d) $(GUI_FRONTEND_OBJ:.o=.d) \
       $(CLI_OBJ:.o=.d) $(MAIN_CLI_OBJ:.o=.d) \
       $(XLIB_PORT_OBJ:.o=.d) $(MAIN_XLIB_OBJ:.o=.d)
-include $(DEPS)

.PHONY: core gui cli xlib

core: $(CORE_OBJ)            

gui: $(GUI_FRONTEND_OBJ)    

cli: $(CORE_OBJ) $(CLI_OBJ) $(MAIN_CLI_OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

xlib: LDFLAGS += -lX11
xlib: $(CORE_OBJ) $(GUI_FRONTEND_OBJ) $(XLIB_PORT_OBJ) $(MAIN_XLIB_OBJ)
	  $(CC) $(LDFLAGS) $^ -o $@
