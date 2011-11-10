falcon_SRC_DIRS := $(FALCON_DIR)/src/*/*

falcon_INC_DIRS := $(shell find $(FALCON_DIR)/include -type d -not -path "*/.svn*")
falcon_INCLUDE  := $(foreach i, $(falcon_INC_DIRS), -I$(i))

libmesh_INCLUDE := $(falcon_INCLUDE) $(libmesh_INCLUDE)

falcon_LIB := $(FALCON_DIR)/libfalcon-$(METHOD)$(libext)
LIBS += $(falcon_LIB)

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

all:: $(falcon_LIB)

# build rule for lib RAT 
ifeq ($(enable-shared),yes)
# Build dynamic library
$(falcon_LIB): $(falcon_objects)
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

# include RAT dep files
-include $(FALCON_DIR)/src/*/*.d


# how to build RAT application
ifeq ($(APPLICATION_NAME),falcon)
all:: falcon

falcon: $(falcon_APP)

$(falcon_APP): $(moose_LIB) $(falcon_LIB) $(falcon_app_objects)
	@echo "Linking "$@"..."
	@$(libmesh_CXX) $(libmesh_CXXFLAGS) $(falcon_app_objects) -o $@ $(LIBS) $(libmesh_LIBS) $(libmesh_LDFLAGS) $(ADDITIONAL_LIBS)

-include $(FALCON_DIR)/src/*.d
endif


clean::
	@rm -fr $(falcon_APP)
	@rm -fr $(falcon_LIB)
	@find . -name "*~" -or -name "*.o" -or -name "*.d" -or -name "*.pyc" \
                -or -name "*.gcda" -or -name "*.gcno" -or -name "*.gcov" -exec rm '{}' \;
	@rm -fr *.mod

cleanall::
	make -C $(FALCON_DIR) clean


###############################################################################
# Additional special case targets should be added here

# F90 module dependancy rules
$(FALCON_DIR)/src/Water_Steam_EOS/water_steam_phase_prop.f90: $(FALCON_DIR)/src/Water_Steam_EOS/water_eos.$(obj-suffix)
$(FALCON_DIR)/src/Water_Steam_EOS/water_steam_phase_prop.f90: $(FALCON_DIR)/src/Water_Steam_EOS/IAPWS97.$(obj-suffix)
