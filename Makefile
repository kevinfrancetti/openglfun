.SUFFIXES:
MAKEFLAGS += --no-builtin-rules
#Last modification 17.11.2025

#config
GCC := gcc
AR := ar


#Directory structure
SRC_DIR := src
BUILD_DIR := build
BUILD_OBJ_DIR := $(BUILD_DIR)/obj
INCLUDE_DIR := include
GLAD_INCLUDE_DIR := glad/include
GLFW_INCLUDE_DIR := glfw3/include




#Compiler flags
INCLUDES := -I$(INCLUDE_DIR) -I$(GLAD_INCLUDE_DIR) -I$(GLFW_INCLUDE_DIR) -I../libkevmatrix/include
CFLAGS := -Wall -MMD $(INCLUDES) -c

#Linker flags
LDLIBS := -L../libkevmatrix/build/static/lib -L./glfw3
LDFLAGS := -lkevmatrix -lm -lglfw3

PROGRAM_NAME := $(BUILD_DIR)/main

srcs:=$(wildcard $(SRC_DIR)/*.c)

objs := $(patsubst $(SRC_DIR)/%.c,$(BUILD_OBJ_DIR)/%.o, $(srcs))
deps := $(patsubst $(BUILD_DIR)/%.o,$(BUILD_DIR)/%.d, $(objs))

#Bug here **make: *** No rule to make target 'build/obj/glad.o', needed by 'build/main'.  Stop.**
srcs +=glad/src/glad.c
objs +=$(BUILD_OBJ_DIR)/glad.o


$(PROGRAM_NAME): $(objs)
	$(GCC) $^ $(LDLIBS) $(LDFLAGS) -o $@


.PHONY: object_files
object_files: $(objs)

.PHONY: debug
debug: CFLAGS += -g

.PHONY: create_directory
create_directory: | $(BUILD_OBJ_DIR) $(SRC_DIR) $(INCLUDE_DIR)


$(BUILD_OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_OBJ_DIR)
	$(GCC) $(CFLAGS)  $< -o $@


$(BUILD_OBJ_DIR) $(SRC_DIR) $(INCLUDE_DIR):
	mkdir -p $@


.PHONY: clean
clean:
	rm  $(objs) $(STATIC_LIB_NAME) $(deps) $(PROGRAM_NAME)

print:
	@echo "srcs:$(srcs)"
	@echo "objs:$(objs)"
	@echo "deps:$(deps)"
	@echo $(STATIC_DIR)
	@echo $(SRC_DIR)
	@echo $(BUILD_DIR)



-include $(deps)
