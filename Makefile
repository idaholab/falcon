###############################################################################
################### MOOSE Application Standard Makefile #######################
###############################################################################
#
# Optional Environment variables
# MOOSE_DIR        - Root directory of the MOOSE project 
# HERD_TRUNK_DIR   - Location of the HERD repository
# FRAMEWORK_DIR    - Location of the MOOSE framework
#
###############################################################################
MOOSE_DIR          ?= $(shell dirname `pwd`)/moose
HERD_TRUNK_DIR     ?= $(shell dirname `pwd`)
FRAMEWORK_DIR      ?= $(MOOSE_DIR)/framework
###############################################################################

################################## ELK MODULES ################################
ALL_ELK_MODULES := yes
###############################################################################

# framework
include $(FRAMEWORK_DIR)/build.mk
include $(FRAMEWORK_DIR)/moose.mk

# TODO: Falcon STILL has several copies of Elk objects... This is sooooo wrong,
# when will this ever get fixed?  You have list Falcon before Elk so that the 
# proper includes will be found.  *sigh*

# dep apps
APPLICATION_DIR    := $(HERD_TRUNK_DIR)/falcon
APPLICATION_NAME   := falcon
BUILD_EXEC         := yes
DEP_APPS           := $(shell $(FRAMEWORK_DIR)/scripts/find_dep_apps.py $(APPLICATION_NAME))
include            $(FRAMEWORK_DIR)/app.mk

# modules
ELK_DIR ?= $(HERD_TRUNK_DIR)/elk
include $(ELK_DIR)/elk.mk

###############################################################################
# Additional special case targets should be added here
