PRODUCT := ising
DEBUG_PRODUCT := $(PRODUCT)-debug
BINDIR  := bin
INCDIR  := ./includes
INCDIR_EXT := ./external_includes
LIBDIR	:= libs
SRCDIR  := src
OBJDIR  := obj
RELEASE_OBJDIR := $(OBJDIR)/release/
DEBUG_OBJDIR := $(OBJDIR)/debug/

MKDIR_P := mkdir -p
RM_RF := rm -rf
ALL := /*

# Language --------------------------------------------------------------------
EXTENSION := cpp
LANGUAGE_STANDARD := -std=c++11
COMPILER := g++
LINKER := g++

# Flags -----------------------------------------------------------------------
# ----- General ---------------------------------------------------------------
INCLUDES := -I$(INCDIR) -I$(INCDIR_EXT) -I/usr/include/SDL2
LIBRARIES := -L$(LIBDIR) -L/usr/lib -lSDL2 -lGL -lGLU
WARNING_FLAGS := -Wall -Wextra
DEPENDENCY_GENERATION_FLAGS := -MMD -MP

# ----- Release ---------------------------------------------------------------
UNUSED_CODE_COMPILER_FLAGS := -ffunction-sections -fdata-sections -flto
OPTIMIZATION_LEVEL := -O3
RELEASE_FLAGS := $(OPTIMIZATION_LEVEL) $(UNUSED_CODE_COMPILER_FLAGS) -fopenmp
RELEASE_LINKER_FLAGS := -Wl,--gc-sections
RELEASE_MACROS :=

# ----- Debug -----------------------------------------------------------------
DEBUG_FLAGS := -O0 -g -DCIRCLE_TESTS
DEBUG_MACROS :=






# -----------------------------------------------------------------------------
# DON'T MESS WITH ANYTHING AFTER THIS UNLESS YOU KNOW WHAT YOU'RE DOING -------
# -----------------------------------------------------------------------------

GENERAL_COMPILER_FLAGS := $(LANGUAGE_STANDARD) $(WARNING_FLAGS) $(DEPENDENCY_GENERATION_FLAGS)

LINKER_FLAGS := $(RELEASE_LINKER_FLAGS)
COMPILER_FLAGS := $(RELEASE_FLAGS) $(GENERAL_COMPILER_FLAGS) $(RELEASE_MACROS)

# Finds all .$(EXTENSION) files and puts them into SRC
SRC := $(wildcard $(SRCDIR)/*.$(EXTENSION))
# Creates .o files for every .$(EXTENSION) file in SRC (patsubst is pattern substitution)
RELEASE_OBJ := $(patsubst $(SRCDIR)/%.$(EXTENSION),$(RELEASE_OBJDIR)/%.o,$(SRC))
DEBUG_OBJ := $(patsubst $(SRCDIR)/%.$(EXTENSION),$(DEBUG_OBJDIR)/%.o,$(SRC))
# Creates .d files (dependencies) for every .$(EXTENSION) file in SRC
DEP := $(patsubst $(SRCDIR)/%.$(EXTENSION),$(OBJDIR)/%.d,$(SRC))
# Finds all lib*.a files and puts them into LIB
LIB := $(wildcard $(LIBDIR)/lib*.a)

# $^ is list of dependencies and $@ is the target file
# Link all the object files or make a library
$(BINDIR)/$(PRODUCT): directories $(RELEASE_OBJ) $(LIB)
# 	Make a library
#	ar rcs $(BINDIR)/$(PRODUCT) $(RELEASE_OBJ) $(LIB)
#	Make a program
	$(LINKER) $(LINKER_FLAGS) $(COMPILER_FLAGS) $(RELEASE_OBJ) $(LIB) $(LIBRARIES) -o $@

$(BINDIR)/$(DEBUG_PRODUCT): directories $(DEBUG_OBJ) $(LIB)
#	Make a library
#	ar rcs $(BINDIR)/$(DEBUG_PRODUCT) $(DEBUG_OBJ) $(LIB)
#	Make a program
	$(LINKER) $(LINKER_FLAGS) $(COMPILER_FLAGS) $(DEBUG_OBJ) $(LIB) $(LIBRARIES) -o $@

# Compile individual .$(EXTENSION) source files into object files
$(RELEASE_OBJDIR)/%.o: $(SRCDIR)/%.$(EXTENSION)
	$(COMPILER) $(COMPILER_FLAGS) $(INCLUDES) -c $< -o $@

$(DEBUG_OBJDIR)/%.o: $(SRCDIR)/%.$(EXTENSION)
	$(COMPILER) $(COMPILER_FLAGS) $(INCLUDES) -c $< -o $@

-include $(DEP)

.PHONY: directories

directories: $(OBJDIR) $(RELEASE_OBJDIR) $(DEBUG_OBJDIR)

$(OBJDIR):
	$(MKDIR_P) $(OBJDIR)

$(RELEASE_OBJDIR):
	$(MKDIR_P) $(RELEASE_OBJDIR)

$(DEBUG_OBJDIR):
	$(MKDIR_P) $(DEBUG_OBJDIR)

.PHONY: clean

clean:
	$(RM_RF) $(OBJDIR)$(ALL) $(PRODUCT) $(DEBUG_PRODUCT)

.PHONY: release

release: COMPILER_FLAGS := $(RELEASE_FLAGS) $(GENERAL_COMPILER_FLAGS)
release: LINKER_FLAGS := $(RELEASE_LINKER_FLAGS)
release: directories $(RELEASE_OBJ) $(BINDIR)/$(PRODUCT)

.PHONY: debug

debug: COMPILER_FLAGS := $(DEBUG_FLAGS) $(GENERAL_COMPILER_FLAGS)
debug: LINKER_FLAGS :=
debug: directories $(DEBUG_OBJ) $(BINDIR)/$(DEBUG_PRODUCT)
