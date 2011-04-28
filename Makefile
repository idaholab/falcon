############################################################################### 
################### MOOSE Application Standard Makefile #######################
###############################################################################
#
# Required Environment variables
# LIBMESH_DIR	- location of the libMesh library
#
# Required Make variables
# APPLICATION_NAME  - the name of this application (all lower case)
# MOOSE_DIR	- location of the MOOSE framework
# ELK_DIR	- location of ELK (if enabled)
#
# Optional Environment variables
# CURR_DIR	- current directory (DO NOT MODIFY THIS VARIABLE)
#
# Note: Make sure that there is no whitespace after the word 'yes' if enabling
# an application
###############################################################################
CURR_DIR	?= $(shell pwd)
ROOT_DIR  ?= $(shell dirname `pwd`)
MOOSE_DIR	?= $(ROOT_DIR)/moose

MAKE_LIBRARY := no
APPLICATION_NAME := falcon
include $(MOOSE_DIR)/build.mk

# deps
include $(MOOSE_DIR)/moose.mk

# F90 module dependancy rules
$(CURR_DIR)/src/Water_Steam_EOS/water_steam_phase_prop.f90: $(CURR_DIR)/src/Water_Steam_EOS/water_eos.$(obj-suffix)

###############################################################################
# Additional special case targets should be added here



