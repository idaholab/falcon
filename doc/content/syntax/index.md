!syntax complete

!!!
So this command will build the syntax for Falcon, modules built by Falcon and all the modules
they depend on, and finally the framework. Thus, since Falcon is currently (as of 4/6/21) building
GC, PF, and TM modules, the syntax groups included are as follows:

MooseApp
ChemicalReactionsApp
FalconApp
FluidPropertiesApp
GeochemsitryApp
PorousFlowApp
TensorMechanicsApp

The reason that CR and FP modules are included is because PF is set to depend on them in
$(MOOSE_DIR)/modules/modules.mk. We can exclude objects from these lower level modules by listing
the syntax groups explicitly, e.g.,

!syntax complete groups=MooseApp
                        FalconApp
                        GeochemistryApp
                        PorousFlowApp
                        TensorMechanicsApp

This will also force the groups to be listed in the order specified here (except that MooseApp is
always listed first and overridden if not set to), where as with just `!syntax complete` they really
could come in any order.
!!!
