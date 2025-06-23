# CapsuleApertureAux

!syntax description  /AuxKernels/CapsuleApertureAux

## Example Input File Syntax

`CapsuleApertureAux` computes fracture apertures basedon the distance from the center line of the fracture. The apertures are calculated and scaled according to a generalized sigmoid function.

<!--
Test not written yet
!listing /test/tests/reporters/closest_elems_to_line/constant_line_projection.i block=Reporters/differentID_out -->

!media media/CapsuleApertureAux.png
  id=fig:capsule_aperture_aux
  style=width:90%;
  caption= Fracture Aperture for various slopes at midpoint

!syntax parameters /AuxKernels/CapsuleApertureAux

!syntax inputs /AuxKernels/CapsuleApertureAux

!syntax children /AuxKernels/CapsuleApertureAux
