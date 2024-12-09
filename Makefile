BUILDDIR=./build
WORKDIR=$(shell pwd)
BIN_NAME=dilks-revenge
TARGET=$(BUILDDIR)/bin/$(BIN_NAME)

CMAKE_FLAGS := "-DDREV_DRAW_BB=ON"
# CMAKE_FLAGS += "-DDREV_DRAW_TEXT_BB=ON"
# CMAKE_FLAGS += "-DDREV_PRINT_FONT_DATA=ON"
# CMAKE_FLAGS += "-DDREV_SAVE_FONT_BITMAP=ON"

CLANG_FMT_FILES += src/*.c
CLANG_FMT_FILES += include/*.h

.PHONY: build init clean debug run test memcheck

build: init
	cmake -B $(BUILDDIR) $(CMAKE_FLAGS); \
	cd $(BUILDDIR); \
	make; \
	cd $(WORKDIR)

init:
	mkdir -p $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR)

debug:
	gdb $(TARGET)

fmt-check:
	clang-format -n --Werror --style=file --fallback-style="LLVM" $(CLANG_FMT_FILES)

fmt:
	clang-format -i --Werror --style=file --fallback-style="LLVM" $(CLANG_FMT_FILES)

run:
	$(TARGET) font_path="/home/swansong/.local/share/fonts/ProggyVector Regular.ttf"

test:
	cmake -B $(BUILDDIR) $(CMAKE_TEST_FLAGS);
	cd $(BUILDDIR);
	make;
	cd $(WORKDIR);
	$(TARGET)

memcheck:
	valgrind \
		--tool=memcheck                 \
		--leak-check=full               \
		--show-reachable=yes            \
		--num-callers=20                \
		--suppressions=./supfilters.log \
		--gen-suppressions=no           \
		$(TARGET)
