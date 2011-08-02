#!/usr/bin/env python2.5

import os, sys, math
# MAC Path
sys.path.append('/Applications/Cubit-13.0/Cubit.app/Contents/MacOS')
if os.environ.has_key('CUBIT_HOME'):
  cubit_home = os.environ['CUBIT_HOME']
  sys.path.append(cubit_home + '/bin')
import cubit

def build_fracture_network():
  filename = "foo.e"
  

  cubit.init([''])
  cubit.cmd('## Cubit Version 11.0')
  cubit.cmd('## -warning = On')
  cubit.cmd('## -information = On')
  cubit.cmd('reset')
  cubit.cmd('create brick x 361 y 181 z 1')
  
  myVol = cubit.get_last_id('volume')
  print "Last Vol: " + str(myVol)

  cubit.cmd('mesh volume 1')
  cubit.cmd('block 1 volume 1')

  cubit.cmd('export genesis "' + filename + '" dimension 3 overwrite')
  

# This function swaps the Y/Z coordinates if the pellet stack is extruded along the Y axis instead of the Z axis
def point(xyz):
  if global_rotate_to_y:
    return (xyz[0], xyz[2], xyz[1])
  else:
    return xyz

def closest_node(node_list, point):
  closest_node = -1
  closest_dist = sys.maxint
  for node in node_list:
    coords = cubit.get_nodal_coordinates(node)
    dist = math.sqrt((coords[0]-point[0])**2+(coords[1]-point[1])**2+(coords[2]-point[2])**2)
    if dist < closest_dist:
      closest_dist = dist
      closest_node = node
  return closest_node

def get_all_curve_nodes(curve):
  nodes = []
  for node in cubit.get_curve_nodes(curve):
    nodes.append( node )
  for vertex in cubit.get_relatives("curve", curve, "vertex"):
    nodes.append(cubit.get_vertex_node(vertex))
  return nodes

def get_all_surface_nodes(surface):
  nodes = []
  for node in cubit.get_surface_nodes(surface):
    nodes.append( node )
  for curve in cubit.get_relatives("surface", surface, "curve"):
    for node in get_all_curve_nodes(curve):
      nodes.append(node)
  return nodes

def get_all_volume_nodes(volume):
  nodes = []
  for node in cubit.get_volume_nodes(volume):
    nodes.append( node )
  for surface in cubit.get_relatives("volume", volume, "surface"):
    for node in get_all_surface_nodes( surface ):
      nodes.append(node)
  return nodes

def get_node_on_surface_with_point(surface_id, point, include_sub_entries=False):
  if include_sub_entries:
    nodes = get_all_surface_nodes(surface_id)
  else:
    nodes = list(cubit.get_surface_nodes(surface_id))
  return closest_node(nodes, point)

def get_node_in_volume_with_point(volume_id, point):
  nodes = get_all_volume_nodes(volume_id)
  return closest_node(nodes, point)

def get_nodes_in_sideset_with_coord(sideset_id, coord_value, coord_direction):
  nodes = ''
  for node in get_all_surface_nodes(sideset_id):
    coords = cubit.get_nodal_coordinates(node)
    if abs(coords[coord_direction] - coord_value) <= 1e-4 and coords[0] > 0: # hack
      nodes += ' ' + str(node)
  return nodes


def get_node_with_max_value_in_dim_on_surface(surface_ids, dim):
  # Takes two surfaces to find the common curve on which to find the node
  saved_node = -1
  saved_max = -1.0
#
  curve_id = cubit.get_common_curve_id(surface_ids[0], surface_ids[1])
  nodes = get_all_curve_nodes(curve_id)
  for node in nodes:
    coords = cubit.get_nodal_coordinates(node)
    if coords[dim] > saved_max:
      saved_node = node
      saved_max = coords[dim]
  return saved_node

def main():
  build_fracture_network()

# When running this command on the command line __name__ will equal __main__
# When running this command through the cubit interface __name__ will equal __console__
if __name__ == '__main__' or __name__ == '__console__':
  main()
