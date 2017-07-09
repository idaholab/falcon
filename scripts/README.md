# A Workflow of Tetrahedral Mesh Generation with Nodal Attributes from a Point Cloud

## Developers ##

* Miu-Lun (Andy) Lau
* Yidong Xia

## Introduction

This document describes a workflow of how to generate a tetrahedral mesh with nodal attributes from a point cloud. In reservoir engieering, it is flexible to use a tetrahedral mesh for representing a geological subsurface domain, and  the nodal attributes in a mesh can be used to represent __material properties__ of interest, e.g., permeability, porosity, rock density and thermal conductivity. Usually, those properties can be provided from the output of reservoir modeling software (e.g. [RockWorks](https://www.rockware.com/product/overview.php?id=165) or [Petrel](https://www.software.slb.com/products/petrel)) in the form of (x, y, z) node coordinates with nodal attributes.

The workflow consists of two parts:

* **Part 1. From Point Cloud to VTK Mesh** describes the generation of a VTK mesh from a point cloud.

* **Part 2. From VTK Mesh to Exodus Mesh** describes the conversion from a VTK mesh to an Exodus mesh.

Currently **Part 1** is automated with a Python script, whereas **Part 2** mostly invovles manual operation. Automation of **Part 2** will be developed in future.

## Prerequisites

A list of software packages required in this workflow is as below:

* [Python](https://www.python.org/downloads/) (Version 2.7.2)
* C++ compiler
* [TetGen](http://wias-berlin.de/software/tetgen/) (Version 1.5)
* [ParaView](https://www.paraview.org/)
* [Cubit](https://cubit.sandia.gov)

### Compile TetGen

In this workflow, the open-source mesh generator TetGen is used to generate a VTK tetrahedral mesh containing nodal attributes from a point cloud. The FALCON repository provides the source code of TetGen in directory __falcon/tpl/tetgen/__. In the command-line window, navigate to directory __falcon/tpl/tetgen/__, and enter the command

```
make
```
to compile the source code and generate an executable file __falcon/tpl/tetgen/tetgen__. Users do not need to call TetGen directly by themselves. Instead, TetGen is called in a Python script __falcon/scripts/points2tets.py__. The GNU C++ compiler **g++** is the default compiler in file __falcon/tpl/tetgen/makefile__; see line 14:

```
CXX = g++
```
 
## Workflow Part 1: From Point Cloud to VTK Mesh

Currently, a working directory must be one located under __falcon/test/__, which is a limitation as a trade-off for requiring minimum user's effort. Two examples can be found in FALCON repository: one is to start from *RockWorks* data, and the other is to start from *Petrel* data.

### Example: Start from *RockWorks* Data 

Follow the steps:

* Navigate to directory __falcon/tests/PT\_TH\_injection\_csvreader\_RockWorks__, and enter the command:

```
../../scripts/generate_vtk_tet_mesh_from_point_cloud.sh
```

* Enter __1__ at the prompt:

```
----------------------------------------------------------------------
[1] ROCKWORKS OR [2] PETREL
ENTER FORMAT:
```

* Enter __example.permeability.txt__ at the prompt:

```
Enter Input File Name:
```

* A tetrahedral mesh file named __example.vtk__ is generated at the end.

Note:

 * __example.permeability.txt__ is one of the five *RockWorks* data files in the directory, with the other four named __example.porosity.txt__, __example.rock\_density.txt__, __example.rock\_specific\_heat.txt__ and __example.thermal\_conductivity.txt__. They have to be individual files because *RockWorks* outputs them individually.

 * Each data file contains only one type of nodal material property, as indicated by the file names. In each file, the data  looks like *`<x> <y> <z> <attribute>`* in each line. 

 * It is OK to enter any of the five file names, because the Python script will read through all files named in the form of __example.*.txt__. The resulting __example.vtk__ file contains all the five sets of material properties.

 * Users are responsible for ensuring that such a file is provided in the correct format.

### Example: Start from *Petrel* Data

Follow the steps:

* Navigate to directory __falcon/tests/PT\_TH\_injection\_csvreader\_Petrel__, and enter the command:

```
../../scripts/generate_vtk_tet_mesh_from_point_cloud.sh
```

* Enter __2__ at the prompt:

```
----------------------------------------------------------------------
[1] ROCKWORKS OR [2] PETREL
ENTER FORMAT:
```

* Enter __example.txt__ at the prompt:

```
Enter Input File Name:
```

* A tetrahedral mesh file named __example.vtk__ is generated at the end.

Note:

* __example.txt__ is a *Petrel* data file containing five sets of nodal material properties: __permeability__, __porosity__, __rock\_density__, __rock\_specific\_heat__ and __thermal\_conductivity__.

* Users are responsible for ensuring that such a file is provided in the correct format.

## Workflow Part 2: VTK Mesh to Exodous Mesh

To convert VTK Mesh into Exodous:

* Load the VTK mesh **example.vtk** into ParaView, and "save data" in  Exodus format as **example.e**.
* Load the **example.e** back into ParaView and "save data" in **example.csv** as CSV format with the reordered nodal attributes
* Remove all the double commas on the first line in **"example.csv"**, and save. 

	If you are using vi as editor, type the following in the vi command environment:
	
		:%s/,/ /g
	This replaces all commas from the 1st line and replace it with space. 

* **example.e** does not contain Sideset info. Load **"example.e"** in Cubit and assign Sideset IDs to the boundaries. Set the element of type from **"TERA"** to **"TERA4"**, and overwrite **"example.e"**

At the end, there should be two files if you follow all the steps above:

* **"example.e"** contains an Exodus mesh file with Subset IDs, but without any nodal material properties. FALCON reads this file in the [mesh] keyword block in the FALCON input script.
* **"example.csv"** a CSV file containing the nodal material properties. FALCON reads this file in the [VectorPostprocessors] keyword block.


