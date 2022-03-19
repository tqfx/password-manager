MAKEFLAGS += --silent

SOURCE_CLANG += include src
SOURCE_CLANG += src/cli
SOURCE_CLANG += test

all: format

.PHONY: format clean

SOURCE_C := $(foreach v,$(SOURCE_CLANG),$(wildcard $(v)/*.[ch]))
SOURCE_CC := $(foreach v,$(SOURCE_CLANG),$(wildcard $(v)/*.[ch][ch]))
SOURCE_CXX := $(foreach v,$(SOURCE_CLANG),$(wildcard $(v)/*.[ch][xp][xp]))
format: $(SOURCE_C) $(SOURCE_CC) $(SOURCE_CXX)
	@clang-format -style=file -i $^ --verbose

clean:
	@-git clean -fdX
