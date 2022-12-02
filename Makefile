###############################################################################
################### MOOSE Application Standard Makefile #######################
###############################################################################
#
# Optional Environment variables
# MOOSE_DIR        - Root directory of the MOOSE project
#
###############################################################################
# Use the MOOSE submodule if it exists and MOOSE_DIR is not set
MOOSE_SUBMODULE    := $(CURDIR)/moose
ifneq ($(wildcard $(MOOSE_SUBMODULE)/framework/Makefile),)
  MOOSE_DIR        ?= $(MOOSE_SUBMODULE)
else
  MOOSE_DIR        ?= $(shell dirname `pwd`)/moose
endif

# check that MOOSE is available
MOOSE_CONTENT      := $(shell ls $(MOOSE_DIR) 2> /dev/null)
ifeq ($(MOOSE_CONTENT),)
  $(error MOOSE framework does not seem to be available. Make sure that either the submodule is checked out or that your MOOSE_DIR points to the correct location)
endif

# check that IAPWS95 is available
IAPWS95_DIR ?= $(CURDIR)/iapws95
IAPWS95_CONTENT := $(shell ls $(IAPWS95_DIR) 2> /dev/null)

# framework
FRAMEWORK_DIR      := $(MOOSE_DIR)/framework
CURRENT_DIR        := $(shell pwd)
FALCON_DIR         ?= $(CURRENT_DIR)

# moose submodule status
moose_status := $(shell git -C $(FALCON_DIR) submodule status 2>/dev/null | grep moose | cut -c1)
ifneq (,$(findstring +,$(moose_status)))
  ifneq ($(origin FALCON_DIR),environment)
    moose_status_msg = "WARNING: Your MOOSE submodule is out of date.\n"
  endif
endif

all: moose_submodule_status

moose_submodule_status:
	@if [ x$(moose_status_msg) != "x" ]; then echo $(moose_status_msg); fi

include $(FRAMEWORK_DIR)/build.mk
include $(FRAMEWORK_DIR)/moose.mk

################################## MODULES ####################################
CHEMICAL_REACTIONS      := yes
FLUID_PROPERTIES        := yes
GEOCHEMISTRY            := yes
HEAT_CONDUCTION         := yes
MISC                    := yes
NAVIER_STOKES           := yes
POROUS_FLOW             := yes
RAY_TRACING             := yes
RDG                     := yes
STOCHASTIC_TOOLS        := yes
TENSOR_MECHANICS        := yes
THERMAL_HYDRAULICS      := yes
SOLID_PROPERTIES        := yes
USE_TEST_LIBS           := yes
RICHARDS                := yes
include           $(MOOSE_DIR)/modules/modules.mk
###############################################################################

# IAPWS95
ifneq ($(IAPWS95_CONTENT),)
  APPLICATION_DIR    := $(IAPWS95_DIR)
  APPLICATION_NAME   := iapws95
  GEN_REVISION       := no
  libmesh_CXXFLAGS   += -DIAPWS95_ENABLED
  include            $(FRAMEWORK_DIR)/app.mk
  include            $(IAPWS95_DIR)/libSBTL.mk
endif

# dep apps
APPLICATION_DIR    := $(CURDIR)
APPLICATION_NAME   := falcon
BUILD_EXEC         := yes
GEN_REVISION       := no
include            $(FRAMEWORK_DIR)/app.mk

###############################################################################
# Additional special case targets should be added here
