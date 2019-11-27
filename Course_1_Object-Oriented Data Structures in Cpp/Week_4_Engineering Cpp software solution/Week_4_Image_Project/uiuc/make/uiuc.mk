# Created by Wade Fagen-Ulmschneider <waf@illinois.edu>
# A few tweaks for CS 400 by Eric Huber

ZIP_FILE = ImageTransform_submission.zip
COLLECTED_FILES = uiuc/HSLAPixel.h uiuc/HSLAPixel.cpp ImageTransform.h ImageTransform.cpp

# Add standard object files (HSLAPixel, PNG, and LodePNG)
OBJS += uiuc/HSLAPixel.o uiuc/PNG.o uiuc/lodepng/lodepng.o

# Use ./.objs to store all .o file (keeping the directory clean)
OBJS_DIR = .objs

# Use all .cpp files in /tests/
OBJS_TEST = $(filter-out $(EXE_OBJ), $(OBJS))
CPP_TEST = $(wildcard tests/*.cpp)
CPP_TEST += uiuc/catch/catchmain.cpp
OBJS_TEST += $(CPP_TEST:.cpp=.o)

# Config
CXX_CLANG = clang++
CXX_GNU = g++
CXX_WHICH = $(CXX_GNU)
CXX = $(CXX_WHICH)
LD = $(CXX_WHICH)
# STDVERSION = -std=c++1y # deprecated nomenclature
STDVERSION = -std=c++14 # proper but requires newer compiler versions (for better or worse)
STDLIBVERSION_CLANG = -stdlib=libc++ # Clang's version; not present on default AWS Cloud9 instance
STDLIBVERSION_GNU =   # blank on purpose; default GNU library
STDLIBVERSION = $(STDLIBVERSION_GNU)
WARNINGS = -pedantic -Wall -Wfatal-errors -Wextra -Wno-unused-parameter -Wno-unused-variable
CXXFLAGS = $(CS400) $(STDVERSION) $(STDLIBVERSION) -g -O0 $(WARNINGS) -MMD -MP -msse2 -c
LDFLAGS = $(CS400) $(STDVERSION) $(STDLIBVERSION) -lpthread
ASANFLAGS = -fsanitize=address -fno-omit-frame-pointer

#  Rules for first executable
$(EXE):
	$(LD) $^ $(LDFLAGS) -o $@
	@echo ""
	@echo " Built the main executable program file for the project: " $(EXE)
	@echo " (Make sure you try \"make test\" too!)"
	@echo ""

# Rule for `all`
all: $(EXE) $(TEST)

# Pattern rules for object files
$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)
	@mkdir -p $(OBJS_DIR)/uiuc
	@mkdir -p $(OBJS_DIR)/uiuc/catch
	@mkdir -p $(OBJS_DIR)/uiuc/lodepng
	@mkdir -p $(OBJS_DIR)/tests

$(OBJS_DIR)/%.o: %.cpp | $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

# Rules for executables
$(TEST):
	$(LD) $^ $(LDFLAGS) -o $@
	@echo ""
	@echo " Built the test suite program: " $(TEST)
	@echo ""

# Executable dependencies
$(EXE): $(patsubst %.o, $(OBJS_DIR)/%.o, $(OBJS))
$(TEST): $(patsubst %.o, $(OBJS_DIR)/%.o, $(OBJS_TEST))

# Include automatically generated dependencies
-include $(OBJS_DIR)/*.d
-include $(OBJS_DIR)/uiuc/*.d
-include $(OBJS_DIR)/uiuc/catch/*.d
-include $(OBJS_DIR)/uiuc/lodepng/*.d
-include $(OBJS_DIR)/tests/*.d

clean:
	rm -rf $(EXE) $(TEST) $(OBJS_DIR) $(CLEAN_RM) $(ZIP_FILE)

tidy: clean
	rm -rf doc

zip:
	@echo "!!! Preparing submission zip with student code..."
	@echo "!!! Make sure you have already tried compiling and testing your code"
	@echo "!!! thoroughly before submitting the zip on Coursera!"
	@echo ""
	@echo "Removing any previous version of zip file..."
	rm -rf $(ZIP_FILE)
	@echo "Creating new file..."
	zip $(ZIP_FILE) $(COLLECTED_FILES)
	@echo "Created zip file: " $(ZIP_FILE)

.PHONY: all tidy clean zip
