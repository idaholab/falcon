import os
import argparse
import typing
import math
import dataclasses
import pandas
import numpy
import scipy.interpolate
import gmsh

@dataclasses.dataclass
class TerrainData:
    """
    Class for storing terrain data supplied by build_terrain_surface function.
    """
    surface: int          # intrger tags of the generated surface
    boundaries: list      # integer tags of the boundaries of the surface
    points: numpy.ndarray # integer tags of the points that defined the surface

def get_arguments():
    """
    Return the parsed command line arguments.
    """

    parser = argparse.ArgumentParser(description="Create 3D mesh from terrain defined by surfaces.")
    parser.add_argument("file_names", metavar='XYZ', type=str, nargs='+',
                        help="List of file names that define terrain surfaces from bottom to top.")

    parser.add_argument("-o", "--output", type=str,
                        help="The name of the Gmsh mesh file to create.")

    parser.add_argument("--show", action='store_true',
                        help="Open the interactive Gmsh window.")

    parser.add_argument("--num_elements", metavar='n', type=int, nargs='+',
                        help=("The desired number of elements within a grid (n by n) for the "
                              "surface(s) If a single value is given it shall be applied to all "
                              "surfaces, otherwise one value per 'file_name' must be given. This "
                              "option must NOT be used with the '--mesh_size' option. If "
                              "'--mesh_size' is not provided this option defaults to 50 elements, "
                              "resulting in a 50 by 50 grid."))

    parser.add_argument("--mesh_size", metavar='h', type=float, nargs='+',
                        help=("The desired element size for the surface(s). If a single value "
                              "is given it shall be applied to all surfaces, otherwise one value "
                              "per 'file_name' must be given. This option must NOT be used with "
                              "the '--num_elements' option."))

    parser.add_argument("--surface_names", metavar='name', type=str, nargs='+',
                        help=("List of the surface names, if provided it must be the same length as "
                              "'file_names'. If not provided the supplied 'file_names' (without the "
                              "extension) are utilized."))

    parser.add_argument("--volume_names", metavar='name', type=str, nargs='+',
                        help=("The desired volume subdomain name(s). If not supplied names are "
                              "automatically added using the file name(s). If supplied the number "
                              "entries must be one less than the number of files."))

    args = parser.parse_args()

    # Need two files
    n_files = len(args.file_names)
    if n_files < 2:
        msg = "At least two file names are required, {} provided: {}"
        raise IOError(msg.format(n_files, args.file_names))

    # Files must exist
    no_exist = [file_name for file_name in args.file_names if not os.path.isfile(file_name)]
    if no_exist:
        msg = "The following supplied 'file_names' do not exist: {}"
        raise IOError(msg.format(' '.join(no_exist)))

    # Output or show should be supplied
    if not args.output:
        args.show = True

    # Element/mesh size
    if args.num_elements and args.mesh_size:
        msg = "The 'num_elements' and 'mesh_size' options must not be used together."
        raise IOError(msg)
    elif not args.num_elements and not args.mesh_size:
        args.num_elements = [50]*n_files

    if args.num_elements:
        if len(args.num_elements) == 1:
            args.num_elements = [args.num_elements[0]]*n_files
        elif len(args.num_elements) != n_files:
            msg = ("The number of entries in 'num_elements' must be one or the same as the number "
                   "'file_names'.")
            raise IOError(msg)
    else:
        args.num_elements = [None]*n_files # see main

    if args.mesh_size:
        if len(args.mesh_size) == 1:
            args.mesh_size = [args.mesh_size[0]]*n_files
        elif len(args.mesh_size) != n_files:
            msg = ("The number of entries in 'mesh_size' must be one or the same as the number "
                   "'file_names'.")
            raise IOError(msg)
    else:
        args.mesh_size = [None]*n_files # see main

    # Surface names
    n_names = len(args.surface_names) if args.surface_names else 0
    if n_names > 0 and (n_files != n_names):
        msg = "The number of 'surface_names' must be the same as the number of 'file_names'."
        raise IOError(msg)
    elif n_names == 0:
        args.surface_names = [os.path.splitext(file_name)[0] for file_name in args.file_names]

    # Volume names
    n_volumes = len(args.volume_names) if args.volume_names else 0
    if n_volumes > 0 and (n_files - 1 != n_volumes):
        msg = "The number of 'volume_names' must be one less then the number of 'file_names'."
        raise IOError(msg)
    elif n_volumes == 0:
        args.volume_names = [f"{os.path.splitext(args.file_names[i])[0]}_{os.path.splitext(args.file_names[i+1])[0]}" for i in range(n_files-1)]

    return args


def main():
    args = get_arguments()

    gmsh.initialize()
    gmsh.model.add(args.output or 'terrain')

    # Build terrain surfaces
    tdata = list()
    for file_name, num_elements, mesh_size in zip(args.file_names, args.num_elements, args.mesh_size):
        tdata.append(build_terrain_surface(file_name, num_elements=num_elements, mesh_size=mesh_size))

    # Build side surface
    n_volumes = len(tdata) - 1
    sides = [None]*n_volumes
    for i in range(n_volumes):
        sides[i] = build_side_surfaces(tdata[i].boundaries, tdata[i].points, tdata[i+1].boundaries, tdata[i+1].points)

    # Build volumes
    gmsh.model.occ.removeAllDuplicates()
    volumes = [None]*n_volumes
    for i in range(n_volumes):
        loop = gmsh.model.occ.addSurfaceLoop(sides[i] + [tdata[i].surface, tdata[i+1].surface])
        volumes[i] = gmsh.model.occ.addVolume([loop])

    gmsh.model.occ.synchronize()

    # Volume labels
    for i in range(len(args.volume_names)):
        tag = gmsh.model.addPhysicalGroup(3, [volumes[i]])
        gmsh.model.setPhysicalName(3, tag, args.volume_names[i])

    # Terrain surfaces
    for i in range(len(args.surface_names)):
        tag = gmsh.model.addPhysicalGroup(2, [tdata[i].surface])
        gmsh.model.setPhysicalName(2, tag, args.surface_names[i])

    # Side surfaces
    for i in range(len(args.volume_names)):
        for name, side in zip(['front', 'right', 'back', 'left'], sides[i]):
            tag = gmsh.model.addPhysicalGroup(2, [side])
            gmsh.model.setPhysicalName(2, tag, f'{args.volume_names[i]}_{name}')

    gmsh.model.mesh.generate()

    gmsh.write(args.output)
    if args.show:
        gmsh.fltk.run()

    gmsh.finalize()
    return 0

def get_nodes(file_name: str, *,
              num_elements: typing.Optional[int] = None,
              mesh_size: typing.Optional[float] = None) -> numpy.ndarray:
    """
    Compute the coordinates from XYZ data on a regular grid.

    The *file_name* should be a CSV data, with headings of 'x', 'y', and 'z' that defines a
    terrain surface. The data is sampled onto a regular grid with spacing approximately equal to
    the supplied *elem_size*.
    """
    if (num_elements is None) and (mesh_size is None):
        num_elements = 50

    # Create coordinate grids
    points = pandas.read_csv(file_name)

    xmin = points['x'].min()
    xmax = points['x'].max()
    ymin = points['y'].min()
    ymax = points['y'].max()

    if num_elements is not None:
        nx = num_elements if (mesh_size is None) else math.ceil((xmax - xmin) / mesh_size)
        ny = num_elements if (mesh_size is None) else math.ceil((ymax - ymin) / mesh_size)

        sz_x = (xmax - xmin) / nx
        sz_y = (ymax - ymin) / ny
        mesh_size = (sz_x + sz_y) / 2
    else:
        nx = math.ceil((xmax - xmin) / mesh_size)
        ny = math.ceil((ymax - ymin) / mesh_size)

    xi = numpy.linspace(xmin, xmax, nx)
    yi = numpy.linspace(ymin, ymax, ny)
    grid_x, grid_y = numpy.meshgrid(xi, yi)
    grid_z = scipy.interpolate.griddata((points['x'], points['y']), points['z'], (grid_x, grid_y), method='nearest')

    nodes = numpy.ndarray((nx, ny), dtype=tuple)
    for i in range(nx):
        for j in range(ny):
            nodes[i,j] = (grid_x[i, j], grid_y[i, j], grid_z[i, j])

    return nodes, mesh_size

def build_terrain_surface(file_name: str,
                          *,
                          num_elements: typing.Optional[int] = None,
                          mesh_size: typing.Optional[float] = None) -> TerrainData:
    """
    Create a GMSH surface of the terrain.

    The function returns the integer tag for the surface, a list of integer tags for the bounding
    spline lines, and array of integers with the points defining the surface.

    See get_nodes for input.
    """

    nodes, mesh_size = get_nodes(file_name, num_elements=num_elements, mesh_size=mesh_size)

    nx, ny = nodes.shape
    points = numpy.ndarray((nx, ny), dtype=int)
    for i in range(nx):
        for j in range(ny):
            points[i,j] = gmsh.model.occ.addPoint(*nodes[i,j], meshSize=mesh_size)


    surface = gmsh.model.occ.addBSplineSurface(list(points.flatten()), nx)
    gmsh.model.occ.synchronize()

    boundaries = gmsh.model.getBoundary([(2, surface)])
    return TerrainData(surface, [b[1] for b in boundaries], points)

def build_side_surfaces(bot_lines: list, bot_points: numpy.ndarray,
                top_lines: list, top_points: numpy.ndarray) -> list: # list[int]
    """
    Given integer tags that define the two surfaces, add the surrounding surfaces.

    bot_lines: list of integer tags for the boundaries extracted from the lower surface
    bot_points: array of integer ids for the points that define the upper surface
    top_lines: list of integer tags for the boundaries extracted from the upper surface
    top_points: array of integer ids for the points that define the upper surface

    Returns a list of integer tags for the created surfaces.
    """

    surfaces = list()
    vert_lines = [gmsh.model.occ.addLine(bot_points[-1,0], top_points[-1,0]),
                  gmsh.model.occ.addLine(bot_points[0,0], top_points[0,0]),
                  gmsh.model.occ.addLine(bot_points[0,-1], top_points[0,-1]),
                  gmsh.model.occ.addLine(bot_points[-1,-1], top_points[-1,-1])]

    # YZ-plane at X=0
    curve = gmsh.model.occ.addCurveLoop([vert_lines[0], top_lines[0], -vert_lines[1], -bot_lines[0]])
    surfaces.append(gmsh.model.occ.addPlaneSurface([curve]))

    # XZ-plane at Y=0
    curve = gmsh.model.occ.addCurveLoop([vert_lines[1], top_lines[1], -vert_lines[2], -bot_lines[1]])
    surfaces.append(gmsh.model.occ.addPlaneSurface([curve]))

    # YZ-plane at X=-1
    curve = gmsh.model.occ.addCurveLoop([vert_lines[2], top_lines[2], -vert_lines[3], -bot_lines[2]])
    surfaces.append(gmsh.model.occ.addPlaneSurface([curve]))

    # XZ-plane at Y=-1
    curve = gmsh.model.occ.addCurveLoop([vert_lines[3], top_lines[3], -vert_lines[0], -bot_lines[3]])
    surfaces.append(gmsh.model.occ.addPlaneSurface([curve]))

    gmsh.model.occ.synchronize()
    return surfaces
