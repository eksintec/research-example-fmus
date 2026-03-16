OUT_FMU_NAME=autosimconfig-cfp-manual-reset.fmu

SOURCES = $(wildcard src/*.c)
SOURCES_CPP = $(wildcard src/*.cpp)

BIN_LINUX = $(SOURCES:%.c=%.linux.o) $(SOURCES_CPP:%.cpp=%.linux.o)
BIN_WINDOWS = $(SOURCES:%.c=%.windows.o) $(SOURCES_CPP:%.cpp=%.windows.o)

ALL_OBJECTS = $(BIN_LINUX) $(BIN_WINDOWS)
ALL_DEPS = $(ALL_OBJECTS:%.o=%.d)

SO_LINUX = model.so
SO_WINDOWS = model.dll
MANIFEST = modelDescription.xml

ALL_SO = $(SO_LINUX) $(SO_WINDOWS)
ALL_BINS = $(ALL_OBJECTS) $(ALL_SO)

PREFIX_LIN = x86_64-linux-gnu
PREFIX_WIN = x86_64-w64-mingw32

LIN_C = $(PREFIX_LIN)-gcc
LIN_CPP = $(PREFIX_LIN)-g++
WIN_C = $(PREFIX_WIN)-gcc
WIN_CPP = $(PREFIX_WIN)-g++

CCPPFLAGS = -Wall -Wextra -fPIC -Werror -ggdb -O2 -MMD -Iinclude -Ifmi-include
CFLAGS = $(CCPPFLAGS) -std=gnu17
CPPFLAGS = $(CCPPFLAGS) -std=gnu++17
LDFLAGS = -ggdb -Wall -Wextra

LDFLAGS_WIN = -static-libgcc -static-libstdc++
LDFLAGS_LIN =

.PHONY: all
all: fmu

.PHONY: fmu
fmu: xmllint
	$(MAKE) $(OUT_FMU_NAME)

.PHONY: clean
clean:
	rm -rf $(OUT_FMU_NAME) .build $(ALL_BINS) $(ALL_DEPS)

.PHONY: xmllint
xmllint:
	xmllint --schema fmi-xsd/fmi2ModelDescription.xsd --noout $(MANIFEST)

$(OUT_FMU_NAME): $(MANIFEST) $(ALL_SO)
	rm -rf .build
	mkdir .build

	cp modelDescription.xml .build

	mkdir .build/extra .build/sources .build/binaries .build/resources

	$(MAKE) documentation

	cp -r terminalsAndIcons .build

	mkdir .build/binaries/win64
	cp model.dll .build/binaries/win64/$(FMU_NAME).dll

	mkdir .build/binaries/linux64
	cp model.so .build/binaries/linux64/$(FMU_NAME).so

	rm -f "$@"
	cd .build && zip -r "$(abspath $@)" .

.PHONY: documentation
documentation:
	cp -r documentation .build
	mkdir .build/documentation/licenses
	cp LICENSE .build/documentation/licenses/license.txt

%.windows.o: %.c
	$(WIN_C) $(CFLAGS) -c $< -o $@

%.windows.o: %.cpp
	$(WIN_CPP) $(CPPFLAGS) -c $< -o $@

$(SO_WINDOWS): $(BIN_WINDOWS)
	$(WIN_CPP) $(LDFLAGS) $(LDFLAGS_WINDOWS) -shared -o $@ $^

%.linux.o: %.c
	$(LIN_C) $(CFLAGS) -c $< -o $@

%.linux.o: %.cpp
	$(LIN_CPP) $(CPPFLAGS) -c $< -o $@

$(SO_LINUX): $(BIN_LINUX)
	$(LIN_CPP) $(LDFLAGS) $(LDFLAGS_LINUX) -shared -o $@ $^

-include $(ALL_DEPS)
