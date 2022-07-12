# ClosestElemsToLine

!syntax description /Reporters/ClosestElemsToLine

## Example Input File Syntax

`ClosestElemsToLine` finds all element centroids within a `projection_tolerance` of a line segement.  The two points defining the line segment are read from a reporter.  The `domain_var` is the variable used for screening the elements i.e. each element selected must have a unique `domain_var`.  `domain_var` are round to their nearest integer value.

!listing /test/tests/reporters/closest_elems_to_line/constant_line_projection.i block=Reporters/differentID_out

!syntax parameters /Reporters/ClosestElemsToLine

!syntax inputs /Reporters/ClosestElemsToLine

!syntax children /Reporters/ClosestElemsToLine
