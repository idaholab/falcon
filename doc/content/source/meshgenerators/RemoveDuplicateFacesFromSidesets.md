# RemoveDuplicateFacesFromSidesets

!syntax description /Mesh/RemoveDuplicateFacesFromSidesets

## Overview

This meshgenerator will remove duplicate faces from a sideset.
We often use a gmsh script to mesh discrete fracture networks.
The mesh script produces a 3D mesh with 2D fractures represented as sidesets.
The sidesets will contain faces from both volumetric elements that lie on the fracture.
These duplicated faces must be removed before using the meshgenerator with the `LowerDBlockFromSidesetGenerator` to turn the sidesets into blocks or else duplicated blocks will be created.

## Example Input File Syntax

!listing msh_to_exodus_rm_duplicate_ss.i block=Mesh/rmDuplicateSS

!syntax parameters /Mesh/RemoveDuplicateFacesFromSidesets

!syntax inputs /Mesh/RemoveDuplicateFacesFromSidesets

!syntax children /Mesh/RemoveDuplicateFacesFromSidesets
