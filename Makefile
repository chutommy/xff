.DEFAULT_GOAL := all

TARGET     = chutommy

CXX        = g++
CXX_FLAGS  = -Wall -pedantic -Wextra -std=c++17 -g -fsanitize=address
MKDIR      = mkdir -p

STDERR_OUT = /dev/null
SOURCE     = src
BUILD      = build

SOURCES    = $(wildcard $(SOURCE)/*.cpp $(SOURCE)/*/*.cpp)
HEADERS    = $(wildcard $(SOURCE)/*.h $(wildcard $(SOURCE)/*/*.h))
OBJECTS    = $(SOURCES:$(SOURCE_DIR)/%.cpp=$(BUILD_DIR)/%.o)

DEPS       = Makefile.d
-include     $(DEPS)

.PHONY: all
all: compile doc

.PHONY: compile
compile: $(DEPS) $(TARGET)

.PHONY: run
run: $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $^ -o $@

$(BUILD)/%.o: $(SOURCE)/%.cpp
	$(MKDIR) $(BUILD)
	$(CXX) $(CXXFLAGS) $< -c -o $@

$(DEPS):
	$(CXX) -MM $(SOURCE)/*.cpp | sed -r 's|^(.*\.o)|build/\1|' > Makefile.d

.PHONY: clean
clean:
	rm -rf Makefile.d \
	$(BUILD) doc/ \
	$(TARGET) .xff \
	$(TARGET).zip .archive \
	$(BUILD) 2>$(STDERR_OUT)

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
