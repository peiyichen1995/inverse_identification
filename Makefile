##################################################################################
# Inverse identification
##################################################################################

EXECUTABLE := optimize

src := $(shell find src -name '*.cpp')
obj := $(src:.cpp=.o)

##################################################################################
# PETSc configurations
##################################################################################

# The following variable must either be a path to petsc.pc or just "petsc" if petsc.pc
# has been installed to a system location or can be found in PKG_CONFIG_PATH.
PETSC_DIR ?= $(HOME)/.local
PETSC_ARCH ?=
petsc.pc := $(PETSC_DIR)/$(PETSC_ARCH)/lib/pkgconfig/petsc.pc

# Additional libraries that support pkg-config can be added to the list of PACKAGES below.
PACKAGES := $(petsc.pc)

CC := $(shell pkg-config --variable=ccompiler $(PACKAGES))
CXX := $(shell pkg-config --variable=cxxcompiler $(PACKAGES))
FC := $(shell pkg-config --variable=fcompiler $(PACKAGES))
CFLAGS_OTHER := $(shell pkg-config --cflags-only-other $(PACKAGES))
CFLAGS := $(shell pkg-config --variable=cflags_extra $(PACKAGES)) $(CFLAGS_OTHER)
CXXFLAGS := $(shell pkg-config --variable=cxxflags_extra $(PACKAGES)) $(CFLAGS_OTHER)
FFLAGS := $(shell pkg-config --variable=fflags_extra $(PACKAGES))
CPPFLAGS := $(shell pkg-config --cflags-only-I $(PACKAGES))
LDFLAGS := $(shell pkg-config --libs-only-L --libs-only-other $(PACKAGES))
LDFLAGS += $(patsubst -L%, $(shell pkg-config --variable=ldflag_rpath $(PACKAGES))%, $(shell pkg-config --libs-only-L $(PACKAGES)))
LDLIBS := $(shell pkg-config --libs-only-l $(PACKAGES)) -lm
CUDAC := $(shell pkg-config --variable=cudacompiler $(PACKAGES))
CUDAC_FLAGS := $(shell pkg-config --variable=cudaflags_extra $(PACKAGES))
CUDA_LIB := $(shell pkg-config --variable=cudalib $(PACKAGES))
CUDA_INCLUDE := $(shell pkg-config --variable=cudainclude $(PACKAGES))

##################################################################################
# Other dependencies and additional flags
##################################################################################

METAPHYSICLROOT ?= $(HOME)/.local
EIGENROOT ?= $(shell pwd)

CXXFLAGS += -Iinclude
CXXFLAGS += -Iinclude/models
CXXFLAGS += -Iinclude/problems
CXXFLAGS += -I$(METAPHYSICLROOT)/include
CXXFLAGS += -I$(EIGENROOT)/eigen
LDLIBS += -lstdc++

##################################################################################
# make rules
##################################################################################

$(EXECUTABLE): $(obj)
	@echo 'Linking executable $@'
	@$(LINK.cc) -o $@ $^ $(LDLIBS)

%.o: %.cpp
	@echo 'Compiling with $(CC) $@'
	@$(COMPILE.cc) $(OUTPUT_OPTION) $<

.PHONY: clean print

clean:
	@echo Removing object files
	@rm -f src/*.o
	@rm -f src/**/*.o
	@echo Removing the executable
	@rm -f $(EXECUTABLE)

print:
	@echo CC=$(CC)
	@echo CXX=$(CXX)
	@echo FC=$(FC)
	@echo CFLAGS=$(CFLAGS)
	@echo CXXFLAGS=$(CXXFLAGS)
	@echo FFLAGS=$(FFLAGS)
	@echo CPPFLAGS=$(CPPFLAGS)
	@echo LDFLAGS=$(LDFLAGS)
	@echo LDLIBS=$(LDLIBS)
	@echo CUDAC=$(CUDAC)
	@echo CUDAC_FLAGS=$(CUDAC_FLAGS)
	@echo CUDA_LIB=$(CUDA_LIB)
	@echo CUDA_INCLUDE=$(CUDA_INCLUDE)
