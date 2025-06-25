# CapsuleApertureAux

!syntax description  /AuxKernels/CapsuleApertureAux

The apertures are calculated and scaled according to a generalized sigmoid function.
This kernel creates a line given by the two points, [!param](/AuxKernels/CapsuleApertureAux/start_point) and [!param](/AuxKernels/CapsuleApertureAux/end_point). 
It then computes a value for a sigmoid function centered at [!param](/AuxKernels/CapsuleApertureAux/midpoint_of_sigmoid) which dies off at a rate of [!param](/AuxKernels/CapsuleApertureAux/slope_at_midpoint) based on the closest distance of the current point from the line segment. The maximum and minumum initial apertures are specified by [!param](/AuxKernels/CapsuleApertureAux/a_max) and [!param](/AuxKernels/CapsuleApertureAux/a_min) respectively.

!media media/CapsuleApertureAux.png
  id=fig:capsule_aperture_aux
  style=width:90%;
  caption= Fracture Aperture for a [!param](/AuxKernels/CapsuleApertureAux/midpoint_of_sigmoid) of 50, [!param](/AuxKernels/CapsuleApertureAux/a_max) of 3e-04, [!param](/AuxKernels/CapsuleApertureAux/a_min) of 1e-05 and [!param](/AuxKernels/CapsuleApertureAux/slope_at_midpoint) of 1.00 (left), 0.10 (center) and 0.01 (right)

## Example Input File Syntax

!listing test/tests/auxkernels/capsuleApertureAux/capsuleAperture.i block=AuxKernels


!syntax parameters /AuxKernels/CapsuleApertureAux

!syntax inputs /AuxKernels/CapsuleApertureAux

!syntax children /AuxKernels/CapsuleApertureAux
