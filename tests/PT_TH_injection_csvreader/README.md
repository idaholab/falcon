# A Tutorial on Using Point Cloud Data
A concise tutorial is presented with an example for explaining how to use point cloud data in FALCON simulation.

## From Point Cloud Data to a VTK Mesh
Given a group of scatterd nodes with nodal attributes "example.node", use TetGen to generate a tetrahedral mesh in the VTK-format "example.vtk". This VTK mesh does not retain the nodal attributes. So there needs a handy code (e.g. written in Python) to append the nodal attributes into the VTK mesh, and save the updated mesh as "example.vtk".

## From a VTK Mesh to an Exodus Mesh
After a VTK mesh file with nodal attributes is generated e.g. "example.vtk", it will be used to generate two files that will be provided as input for a FALCON simulation:

1. "example.e": a pure Exodus mesh file with Sideset IDs,
2. "example.csv": a CSV file containing the nodal attributes.

The detailed steps are:

1. Load the VTK mesh "example.vtk" in ParaView, and do "save data" in Exodus format "example.e". Notice that the nodes in the resulted Exodus mesh are reordered.
2. Load "example.e" in ParaView, and do "save data" in CSV format as "example.csv" to obtain the reordered list of nodal attributes.
3. Remove all the double commas on the first line in "example.csv", and save.
4. "example.e" does not contain Sideset info. Load "example.e" in Cubit, and assign Sideset IDs to the boundaries. Set the element type from "TETRA" to "TETRA4", and overwrite "example.e"

## Remarks

This whole procedure invovles a few steps that needs manual manituplation, which is not typically convenient. Better automated approaches will be explored in future.
