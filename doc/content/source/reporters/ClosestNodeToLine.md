# ClosestNodeToLine

!syntax description /Reporters/ClosestNodeToLine

## Example Input File Syntax

`ClosestNodeToLine` finds all element centroids within a `projection_tolerance` of a line segement.  The two points defining the line segment are read from a reporter.  The `domain_var` is the variable used for screening the elements i.e. each element selected must have a unique `domain_var`.  `domain_var` are round to their nearest integer value.

!listing /test/tests/reporters/closest_node_to_line/constant_line_projection.i block=Reporters/differentID_out

!syntax parameters /Reporters/ClosestNodeToLine

!syntax inputs /Reporters/ClosestNodeToLine

!syntax children /Reporters/ClosestNodeToLine
