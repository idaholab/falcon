#  $Id: Makefile 2501 2007-11-20 02:33:29Z benkirk $

#If MOOSE_DIR isn't found in the environment it's assumed
MOOSE_DIR ?= $(shell pwd)/../moose

HYPRE_LINK_FLAGS=

# The location of the mesh library
meshdir := $(LIBMESH_DIR)

# include the library options determined by configure.  This will
# set the variables INCLUDE and LIBS that we will need to build and
# link with the library.
#include $(meshdir)/Make.common

include $(MOOSE_DIR)/Makefile.export

mpif77_command := $(libmesh_F77)

# If $(libmesh_f77) is an mpiXXX compiler script, use -show
# to determine the base compiler
ifneq (,$(findstring mpi,$(mpif77_command)))
  mpif77_command := $(shell $(libmesh_F77) -show)
endif

# Set certain flags based on compiler

# ifort
ifneq (,$(findstring ifort,$(mpif77_command)))
  libmesh_FFLAGS += -r8
endif

# gfortran
ifneq (,$(findstring gfortran,$(mpif77_command)))
  libmesh_FFLAGS += -fdefault-real-8
endif

# g95
ifneq (,$(findstring g95,$(mpif77_command)))
  libmesh_FFLAGS += -r8
endif

###############################################################################
# File management.  This is where the source, header, and object files are
# defined

#
# source files
srcfiles 	:= $(wildcard src/*.C) $(wildcard src/*/*.C)
fsrcfiles       := $(wildcard src/steam_tables/*.f90)

#
# object files
objects		:= $(patsubst %.C, %.$(obj-suffix), $(srcfiles))
fobjects        := $(patsubst %.f90, %.$(obj-suffix), $(fsrcfiles))

#
# phasefield objects and build rule
falconPFobjects 	:= $(patsubst %.C, %-pf.$(obj-suffix), $(srcfiles))

# include dirs
libmesh_INCLUDE += -Iinclude -Iinclude/kernels -Iinclude/user -Iinclude/materials -Iinclude/steam_tables
###############################################################################


.PHONY: clean doc

###############################################################################
# Target:
#
target 	   := ./falcon-$(METHOD)


all:: $(target)

# Production rules:  how to make the target - depends on library configuration
$(target): $(fobjects) $(objects) $(moose_LIB) $(mesh_library)
	@echo "Linking "$@"..."
	@$(libmesh_CXX) $(libmesh_CXXFLAGS) $(objects) $(fobjects) -o $@ $(libmesh_LIBS) $(libmesh_LDFLAGS)

doc:
	doxygen ./doc/Doxyfile

# Useful rules.
clean:
	@rm -f falcon-*
	@find . -name "*~" -or -name "*.o" | xargs rm
	@rm -f .depend

# include the dependency list
include .depend

print:
	@echo $(mpif77_command)
	@echo $(libmesh_F77)
	@echo $(libmesh_FFLAGS)

#
# Dependencies
#
.depend:
	@$(perl) $(meshdir)/contrib/bin/make_dependencies.pl -I. $(libmesh_INCLUDE) "-S\$$(obj-suffix)" $(srcfiles) > .depend
	@echo "Updated .depend"

###############################################################################
