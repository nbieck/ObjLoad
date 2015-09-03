# ObjLoad 

This loads .obj files and produces a list of vertices and a list of indices.
## Restrictions and Assumptions:

- The file must exist, if it does not, the function will silently fail
- The file should be in a format that can be handled, otherwise catastrophic failure might occur (i.e. proper .obj format and conforming to the mentioned restrictions)
- Each face is triangular
