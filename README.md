# MTM_Final

The goal of the project is to make a calculator for graphs. The calculator cand be used in two forms either a command-line like system or a "batch" version with a input and output file.

## Parts of the project:

### Basic information
 - Graph names are all alpha numeric strings and the first character needs to be a letter
 - Vertex names are alpha numeric and can also include (,),[,],; where all parentheses and brackets must be legal for example this is ok: ([]()) while this isn't: ((])))[ and ; must show up between [ and ]
 - The graphs have no requirements such as connected
 - Spaces don't matter if they don't change a command so print(    G1  ) is the same as print(G1)
 
### Basic operations:
 - [ ] decaring a graph: G1 = {x1, x2, x3 | <x1,x2> , <x3,x2>} this is a graph with three vertices x1, x2, x3 and two edges x1->x2 and x3->x2
 - [ ] copying a graph: G1 = G2 copies G2 into G1
 - [ ] Union (+): G3 = G1 + G2
 - [ ] Intersection (^): G3 = G1 ^ G2
 - [ ] Product (*): G3 = G1 * G2
 - [ ] Subtraction (-): G3 = G1 - G2
 - [ ] Complement (!): G3 = G1!
 
### Basic commands:
 - [ ] print -- for example print(G1) print the graph named G1. Print in the from of the vertices row after row, then $, then the edges. (see in example below)
 - [ ] who -- prints all graphs currently in the calculator
 - [ ] delete -- for example delete(G1) deletes the graph named G1
 - [ ] reset -- deletes all graphs currently in calculator
 - [ ] quit -- exits program
 
 ### Advance operations:
 - [ ] add order of operations as follows: '!' is always done first and then everything from left to right
 - [ ] add parantheses -- makes whatever is in parantheses first in order of operations
 - [ ] allow to use graph literals -- for example allow expressions such as G1 = G2 + {a,b|<a,b>}
 - [ ] save -- for example save(G, graph.gc) would save the graph G into the file graph.gc (use binary files)
 - [ ] load -- for example G=load(graph_file) would load the graph in the file into G (use binary files)
 
 Form of file: (each parameter has a <> around it in the file)
 num_vertices num_edges vertex1 vertex1 vertex2 ... vertexN edge1 edge2 ... edgeM
 
 - [ ] complex operations -- allows for print and save to handle more then just a graph for example print(G1+G2) or save(G1 + {a,b|<a,b>}
 

 ### Example: 
 - Gcalc> G1={x1,x2,x3,x4|<x4,x1>, <x3,x4>,<x2,x3>,<x1,x2>}
 - Gcalc> G2 = { x1,y1 | <x1,y1> }
 - Gcalc> H = G1 + G2
 - Gcalc> print(H)
 - x1
 - x2
 - x3
 - x4
 - y1
 - $
 - x1 x2
 - x1 y1
 - x2 x3
 - x3 x4
 - x4 x1
 - Gcalc> who
 - G1
 - G2
 - H
 - Gcalc> reset
 - Gcalc> print(H)
 - Error: Undefined variable 'H'
 - Gcalc> G1 {x,y|<y,x>}
 - Error: Unrecognized command 'G1 {x,y|<y,x>}'
 - Gcalc> quit
 
