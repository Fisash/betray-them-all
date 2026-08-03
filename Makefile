CC := gcc
AR := ar

BUILD := build
SRC := src

TARGET := betray

OPT := -O2
STRIP := -s

CFLAGS := \
    -std=c89 \
    -Wall -Wextra -Werror \
    -MMD -MP \
    -ffunction-sections \
    -fdata-sections \
    -fno-asynchronous-unwind-tables \
    -flto \
    -fno-ident \
    -Isrc \
    $(OPT)

LDFLAGS := -lm

ifdef DEBUG
CFLAGS += -O0 -g -DDEBUG
else
CFLAGS += -O2
LDFLAGS += -s
endif

CORE_SRC := $(shell find $(SRC)/core -name '*.c')

GUI_SRC := \
    $(shell find $(SRC)/gui/gui-frontend -name '*.c') \
    $(SRC)/gui/frame_buffer.c

CLI_SRC := \
    $(filter-out $(SRC)/cli/main.c,\
        $(shell find $(SRC)/cli -name '*.c'))

CLI_MAIN := $(SRC)/cli/main.c

XLIB_SRC := \
    $(shell find $(SRC)/gui/xlib-port -name '*.c')

XLIB_MAIN := $(SRC)/gui/main.c

# dont used now. check when will make tui
TUI_SRC := \
    $(filter-out $(SRC)/tui/main.c,\
        $(shell find $(SRC)/tui -name '*.c'))


CORE_OBJ := $(CORE_SRC:$(SRC)/%.c=$(BUILD)/%.o)

GUI_OBJ := $(GUI_SRC:$(SRC)/%.c=$(BUILD)/%.o)

CLI_OBJ := $(CLI_SRC:$(SRC)/%.c=$(BUILD)/%.o)
CLI_MAIN_OBJ := $(BUILD)/cli/main.o

XLIB_OBJ := $(XLIB_SRC:$(SRC)/%.c=$(BUILD)/%.o)
XLIB_MAIN_OBJ := $(BUILD)/gui/main.o

$(BUILD)/%.o: $(SRC)/%.c
	@mkdir -p $(@D)
	@echo compiling $<
	@$(CC) $(CFLAGS) -c $< -o $@

CORE_LIB := $(BUILD)/libcore.a
GUI_LIB := $(BUILD)/libgui.a

$(CORE_LIB): $(CORE_OBJ)
	@mkdir -p $(@D)
	@echo archiving libcore.a
	@$(AR) rcs $@ $^

$(GUI_LIB): $(GUI_OBJ)
	@mkdir -p $(@D)
	@echo archiving libgui.a
	@$(AR) rcs $@ $^


$(CLI_MAIN_OBJ): CFLAGS += -DCLI
$(CLI_OBJ):      CFLAGS += -DCLI

$(XLIB_MAIN_OBJ): CFLAGS += -DXLIB
$(XLIB_OBJ):      CFLAGS += -DXLIB
$(GUI_OBJ):       CFLAGS += -DXLIB

.PHONY: all
all: cli

.PHONY: core
core: $(CORE_LIB)

.PHONY: gui
gui: core $(GUI_LIB)

.PHONY: cli
cli: core $(CLI_OBJ) $(CLI_MAIN_OBJ)
	@echo linking $(TARGET)
	@$(CC) \
	    $(CLI_OBJ) \
	    $(CLI_MAIN_OBJ) \
	    $(CORE_LIB) \
	    $(LDFLAGS) \
	    -o $(TARGET)

.PHONY: xlib
xlib: gui $(XLIB_OBJ) $(XLIB_MAIN_OBJ)
	@echo linking  $(TARGET)
	@$(CC) \
	    $(XLIB_OBJ) \
	    $(XLIB_MAIN_OBJ) \
	    $(GUI_LIB) \
	    $(CORE_LIB) \
	    $(LDFLAGS) \
	    -lX11 \
	    -o $(TARGET)

.PHONY: tui
tui:
	@echo "not implemented yet."

.PHONY: clean
clean:
	rm -rf $(BUILD) $(TARGET)

DEPS := $(shell find $(BUILD) -name '*.d' 2>/dev/null)

-include $(DEPS)
