# Discrete Fracture Network Multiapp

Example models of discrete fracture networks (DFN) loosely coupled with the surrounding matrix using the Multiapps system are given in this section.  These examples are based on the multiapp fracture flow example given in [porous_flow module](/porous_flow/index.md).  The first example provides a comparison to an analytic solution by [!cite](gringarten1975) for a single infinite crack in an infinite domain.  The second example demonstrates the use of automatic mesh refinement to refine the matrix mesh around the region containing the DFN from the [porous_flow](/porous_flow/index.md) multiapp fracture flow example.  


## Gringarten solution

This fracture-flow validation problem was created by Koenraad Beckers at NREL to ensure that FALCON for fracture-based reservoirs is set up correctly by comparing FALCON simulation results with the analytic solution by [!cite](gringarten1975). [tab:GringartenParams] lists the FALCON parameter values for this validation problem. The FALCON matrix domain and embedded fracture, with temperature after 3 years of operation, are shown in [fig:GringartenSetup]. Given that the Gringarten solution neglects heat transfer effects in the vertical direction, only one mesh layer was assumed in the vertical direction with a no-flux boundary condition on top and bottom. [fig:GringartenSoln] indicates that the FALCON simulation result for the production temperature is in good agreement with the analytical solution (i.e., Gringarten solution for heat transfer with fluid flow in a single fracture).  Input files for these simulations are found here:

[Matrix Mainapp Input File](/examples/gringarten_multiapp/mrect1.i)

[Fracture Subapp Input File](/examples/gringarten_multiapp/frect1.i)

!table
  id=tab:GringartenParams
    caption=Parameter values for fracture flow validation problem with FALCON
| Parameter	|  Value |
|-----------|--------|
| Rock initial temperature | 90$^{\circ}$C |
| Rock density | 2875 kg/m$^3$ |
| Rock heat capacity | 825 J/kg-K |
| Rock thermal conductivity | 2.83 W/m-K |
| Rock permeability | 1e-16 m$^2$ |
| Rock porosity | 0.1 |
| Water flow rate | 0.1 kg/s |
| Water injection temperature | 30$^{\circ}$C |
| Domain length | 100 m |
| Domain width | 100 m |
| Domain height | 10 m |
| Well spacing | 100 m |

!media media/GringartenSetup.png
  id=fig:GringartenSetup
  style=width:60%;
  caption=(a) FALCON matrix domain for fracture-flow validation problem with temperature (in K) after 3 years of operation. A fracture is embedded in the center of this matrix domain. (b) Embedded fracture mesh with temperature profile (in K) after 3 years of operation. Fluid injection in the fracture occurs at the right-hand side and fluid production is from the left-hand side.

!media media/GringartenSoln.png
  id=fig:GringartenSoln
  style=width:60%;
  caption=Production temperature of fracture-flow validation problem as simulated with FALCON is in good agreement with analytical solution by Gringarten.


## Automatic mesh refinement of matrix around DFN

In this example, automatic mesh refinement is added to the multiapp fracture flow example given in [porous_flow module](/porous_flow/index.md).  
Content to be added
