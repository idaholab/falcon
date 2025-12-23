This directory contains a 3-fracture example of how to couple 1D injection
and production wells modeled using MOOSE's thermal hydraulics module to
a porous flow model in the fractures.

There are two options for the main input file:

- `fracs_main.i`: models the wells and fractures using a MultiApp approach, with the fractures being the main app.
- `wells_main.i`: models the wells and fractures using a MultiApp approach, with the wells being the main app.

There is a plot script that can be run as follows:

```
python plot_transient.py
```
