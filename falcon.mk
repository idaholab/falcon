falcon_INC_DIRS := $(shell find $(FALCON_DIR)/include -type d -not -path "*/.svn*")
falcon_INCLUDE  := $(foreach i, $(falcon_INC_DIRS), -I$(i))

libmesh_INCLUDE := $(falcon_INCLUDE) $(libmesh_INCLUDE)

falcon_LIB := $(FALCON_DIR)/libfalcon-$(METHOD).la

falcon_APP := $(FALCON_DIR)/falcon-$(METHOD)

# source files
falcon_srcfiles    := $(shell find $(FALCON_DIR)/src -name "*.C")
falcon_csrcfiles   := $(shell find $(FALCON_DIR)/src -name "*.c")
falcon_fsrcfiles   := $(shell find $(FALCON_DIR)/src -name "*.f")
falcon_f90srcfiles := $(shell find $(FALCON_DIR)/src -name "*.f90")

# object files
falcon_objects	:= $(patsubst %.C, %.$(obj-suffix), $(falcon_srcfiles))
falcon_objects	+= $(patsubst %.c, %.$(obj-suffix), $(falcon_csrcfiles))
falcon_objects += $(patsubst %.f, %.$(obj-suffix), $(falcon_fsrcfiles))
falcon_objects += $(patsubst %.f90, %.$(obj-suffix), $(falcon_f90srcfiles))

# plugin files
falcon_plugfiles   := $(shell find $(FALCON_DIR)/plugins/ -name "*.C" 2>/dev/null)
falcon_cplugfiles  := $(shell find $(FALCON_DIR)/plugins/ -name "*.c" 2>/dev/null)
falcon_fplugfiles  := $(shell find $(FALCON_DIR)/plugins/ -name "*.f" 2>/dev/null)
falcon_f90plugfiles:= $(shell find $(FALCON_DIR)/plugins/ -name "*.f90" 2>/dev/null)

# plugins
falcon_plugins     := $(patsubst %.C, %-$(METHOD).plugin, $(falcon_plugfiles))
falcon_plugins     += $(patsubst %.c, %-$(METHOD).plugin, $(falcon_cplugfiles))
falcon_plugins     += $(patsubst %.f, %-$(METHOD).plugin, $(falcon_fplugfiles))
falcon_plugins     += $(patsubst %.f90, %-$(METHOD).plugin, $(falcon_f90plugfiles))

# falcon main
falcon_main_src    := $(FALCON_DIR)/src/main.C
falcon_app_objects := $(patsubst %.C, %.$(obj-suffix), $(falcon_main_src))

# dependency files
falcon_deps := $(patsubst %.C, %.$(obj-suffix).d, $(falcon_srcfiles)) \
               $(patsubst %.c, %.$(obj-suffix).d, $(falcon_csrcfiles)) \
               $(patsubst %.C, %.$(obj-suffix).d, $(falcon_main_src))

# If building shared libs, make the plugins a dependency, otherwise don't.
ifeq ($(libmesh_shared),yes)
  falcon_plugin_deps := $(falcon_plugins)
else
  falcon_plugin_deps :=
endif

all:: $(falcon_LIB)

$(falcon_LIB): $(falcon_objects) $(falcon_plugin_deps)
	@echo "Linking "$@"..."
	@$(libmesh_LIBTOOL) --tag=CXX $(LIBTOOLFLAGS) --mode=link --quiet \
	  $(libmesh_CXX) $(libmesh_CXXFLAGS) -o $@ $(falcon_objects) $(libmesh_LIBS) $(libmesh_LDFLAGS) $(EXTERNAL_FLAGS) -rpath $(FALCON_DIR)
	@$(libmesh_LIBTOOL) --mode=install --quiet install -c $(falcon_LIB) $(FALCON_DIR)

# include FALCON dep files
-include $(falcon_deps)


# how to build FALCON application
ifeq ($(APPLICATION_NAME),falcon)
all:: falcon

falcon: $(falcon_APP)

$(falcon_APP): $(moose_LIB) $(elk_MODULES) $(falcon_LIB) $(falcon_app_objects)
	@echo "Linking "$@"..."
	@$(libmesh_LIBTOOL) --tag=CXX $(LIBTOOLFLAGS) --mode=link --quiet \
          $(libmesh_CXX) $(libmesh_CXXFLAGS) -o $@ $(falcon_app_objects) $(falcon_LIB) $(elk_MODULES) $(moose_LIB) $(libmesh_LIBS) $(libmesh_LDFLAGS) $(ADDITIONAL_LIBS)

endif

delete_list := $(falcon_APP) $(falcon_LIB) $(FALCON_DIR)/libfalcon-$(METHOD).*

cleanall:: 
	make -C $(FALCON_DIR) clean 

###############################################################################
# Additional special case targets should be added here

# F90 module dependency rules
$(FALCON_DIR)/src/Water_Steam_EOS/water_steam_phase_prop.f90: $(FALCON_DIR)/src/Water_Steam_EOS/water_eos.$(obj-suffix)
$(FALCON_DIR)/src/Water_Steam_EOS/water_steam_phase_prop.f90: $(FALCON_DIR)/src/Water_Steam_EOS/IAPWS97.$(obj-suffix)
