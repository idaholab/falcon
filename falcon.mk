falcon_SRC_DIRS := $(FALCON_DIR)/src/*/*

falcon_INC_DIRS := $(shell find $(FALCON_DIR)/include -type d -not -path "*/.svn*")
falcon_INCLUDE  := $(foreach i, $(falcon_INC_DIRS), -I$(i))

libmesh_INCLUDE := $(falcon_INCLUDE) $(libmesh_INCLUDE)

falcon_LIB := $(FALCON_DIR)/libfalcon-$(METHOD)$(libext)

falcon_APP := $(FALCON_DIR)/falcon-$(METHOD)

# source files
falcon_srcfiles    := $(shell find $(falcon_SRC_DIRS) -name *.C)
falcon_csrcfiles   := $(shell find $(falcon_SRC_DIRS) -name *.c)
falcon_fsrcfiles   := $(shell find $(falcon_SRC_DIRS) -name *.f)
falcon_f90srcfiles := $(shell find $(falcon_SRC_DIRS) -name *.f90)
# object files
falcon_objects	:= $(patsubst %.C, %.$(obj-suffix), $(falcon_srcfiles))
falcon_objects	+= $(patsubst %.c, %.$(obj-suffix), $(falcon_csrcfiles))
falcon_objects += $(patsubst %.f, %.$(obj-suffix), $(falcon_fsrcfiles))
falcon_objects += $(patsubst %.f90, %.$(obj-suffix), $(falcon_f90srcfiles))

falcon_app_objects := $(patsubst %.C, %.$(obj-suffix), $(FALCON_DIR)/src/main.C)

# plugin files
falcon_plugfiles   := $(shell find $(FALCON_DIR)/plugins/ -name *.C 2>/dev/null)
falcon_cplugfiles  := $(shell find $(FALCON_DIR)/plugins/ -name *.c 2>/dev/null)
falcon_fplugfiles  := $(shell find $(FALCON_DIR)/plugins/ -name *.f 2>/dev/null)
falcon_f90plugfiles:= $(shell find $(FALCON_DIR)/plugins/ -name *.f90 2>/dev/null)

# plugins
falcon_plugins     := $(patsubst %.C, %-$(METHOD).plugin, $(falcon_plugfiles))
falcon_plugins     += $(patsubst %.c, %-$(METHOD).plugin, $(falcon_cplugfiles))
falcon_plugins     += $(patsubst %.f, %-$(METHOD).plugin, $(falcon_fplugfiles))
falcon_plugins     += $(patsubst %.f90, %-$(METHOD).plugin, $(falcon_f90plugfiles))

all:: $(falcon_LIB)

# build rule for lib FALCON
ifeq ($(enable-shared),yes)
# Build dynamic library
$(falcon_LIB): $(falcon_objects) $(falcon_plugins)
	@echo "Linking "$@"..."
	@$(libmesh_CC) $(libmesh_CXXSHAREDFLAG) -o $@ $(falcon_objects) $(libmesh_LDFLAGS)
else
# Build static library
ifeq ($(findstring darwin,$(hostos)),darwin)
$(falcon_LIB): $(falcon_objects)
	@echo "Linking "$@"..."
	@libtool -static -o $@ $(falcon_objects)
else
$(falcon_LIB): $(falcon_objects)
	@echo "Linking "$@"..."
	@$(AR) rv $@ $(falcon_objects)
endif
endif

# include FALCON dep files
-include $(FALCON_DIR)/src/*/*.d


# how to build FALCON application
ifeq ($(APPLICATION_NAME),falcon)
all:: falcon

falcon: $(falcon_APP)

$(falcon_APP): $(moose_LIB) $(elk_MODULES) $(falcon_LIB) $(falcon_app_objects)
	@echo "Linking "$@"..."
	@$(libmesh_CXX) $(libmesh_CXXFLAGS) $(falcon_app_objects) -o $@  $(falcon_LIB) $(elk_MODULES) $(moose_LIB) $(libmesh_LIBS) $(libmesh_LDFLAGS) $(ADDITIONAL_LIBS)

-include $(FALCON_DIR)/src/*.d
endif

delete_list := $(falcon_APP) $(falcon_LIB)

cleanall:: 
	make -C $(FALCON_DIR) clean 

###############################################################################
# Additional special case targets should be added here

# F90 module dependancy rules
$(FALCON_DIR)/src/Water_Steam_EOS/water_steam_phase_prop.f90: $(FALCON_DIR)/src/Water_Steam_EOS/water_eos.$(obj-suffix)
$(FALCON_DIR)/src/Water_Steam_EOS/water_steam_phase_prop.f90: $(FALCON_DIR)/src/Water_Steam_EOS/IAPWS97.$(obj-suffix)
