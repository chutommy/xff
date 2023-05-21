.DEFAULT_GOAL := all

TARGET     = chutommy

CXX        = g++
LD         = g++
CXX_FLAGS  = -Wall -pedantic -Wextra -std=c++17 -g -fsanitize=address
LD_FLAGS   = -g -fsanitize=address -lstdc++fs
MKDIR      = mkdir -p

STDERR_OUT = /dev/null
SOURCE_DIR = src
BUILD_DIR  = build

SOURCES    = $(wildcard $(SOURCE_DIR)/*.cpp)
HEADERS    = $(wildcard $(SOURCE_DIR)/*.h)
OBJECTS    = $(SOURCES:$(SOURCE_DIR)/%.cpp=$(BUILD_DIR)/%.o)

DEPS       = Makefile.d
-include     $(DEPS)

.PHONY: all
all: compile doc

.PHONY: compile
compile: $(DEPS) $(BUILD_DIR) $(TARGET)

.PHONY: run
run: $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJECTS)
	$(LD) -o $@ $^ $(LD_FLAGS)

$(BUILD_DIR):
	$(MKDIR) $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(CXX) $(CXX_FLAGS) $< -c -o $@

$(DEPS):
	$(CXX) -MM $(SOURCE_DIR)/*.cpp | sed -r 's|^(.*\.o)|build/\1|' > Makefile.d

.PHONY: clean
clean:
	rm -rf Makefile.d \
	$(TARGET) .xff \
	$(TARGET).zip .archive \
	$(BUILD_DIR) doc \
	$(BUILD_DIR) 2>$(STDERR_OUT)

.PHONY: doc
doc: Doxyfile README.md $(HEADERS)
	doxygen Doxyfile

.PHONY: zip
zip: $(TARGET).zip

EXAMPLES   = $(wildcard examples/*)
ASSETS     = $(wildcard assets/*)
$(TARGET).zip: README.md zadani.txt prohlaseni.txt Makefile Doxyfile $(HEADERS) $(SOURCES) $(EXAMPLES) $(ASSETS)
	$(MKDIR) -p .archive/$(TARGET)/
	cp -r README.md zadani.txt prohlaseni.txt Makefile Doxyfile src/ examples/ assets/ .archive/$(TARGET)/
	cd .archive/; zip -r ../$(TARGET).zip $(TARGET)/
	rm -r .archive/
