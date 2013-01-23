############################################################################### 
################### MOOSE Application Standard Makefile #######################
###############################################################################
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
CURR_DIR   ?= $(shell pwd)
ROOT_DIR   ?= $(shell dirname `pwd`)

ifeq ($(MOOSE_DEV),true)
	MOOSE_DIR ?= $(ROOT_DIR)/devel/moose
else
	MOOSE_DIR ?= $(ROOT_DIR)/moose
endif
ELK_DIR     ?= $(ROOT_DIR)/elk
FALCON_DIR  ?= $(ROOT_DIR)/falcon

APPLICATION_NAME := falcon

DEP_APPS    ?= $(shell $(MOOSE_DIR)/scripts/find_dep_apps.py $(APPLICATION_NAME))

################################## ELK MODULES ################################
FLUID_MASS_ENERGY_BALANCE := yes
SOLID_MECHANICS := no:
###############################################################################

include $(MOOSE_DIR)/build.mk
include $(MOOSE_DIR)/moose.mk
include $(ELK_DIR)/elk.mk
include $(FALCON_DIR)/falcon.mk

###############################################################################
# Additional special case targets should be added here

