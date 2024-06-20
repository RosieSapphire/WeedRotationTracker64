BUILD_DIR := build

include $(N64_INST)/include/n64.mk

TARGET := weedtracker64
ELF := $(BUILD_DIR)/$(TARGET).elf
ROM := $(TARGET).z64

INC_DIRS :=
SRC_DIRS := .
H_FILES := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.h))
C_FILES := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
O_FILES := $(C_FILES:%.c=$(BUILD_DIR)/%.o)
DEB_FILES := $(O_FILES:%.o=%.d)

N64_CFLAGS := $(INC_DIRS:%=-I%) -O0
ifeq ($(LOGGING_ENABLED), 1)
	CFLAGS += -DLOGGING_ENABLED
endif

final: $(ROM)
$(ROM): N64_ROM_TITLE="WeedRotationTracker"
$(ROM): $(ELF)
$(ELF): $(O_FILES)

clean:
	@echo Cleaning Previous Build...
	@rm -rf $(TARGET) $(BUILD_DIR)

FORMAT_SCAN := $(H_FILES) $(C_FILES)

format:
	clang-format -i --style=file $(FORMAT_SCAN)

-include $(DEB_FILES)
