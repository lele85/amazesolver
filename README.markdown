A-MAZE-SOLVER
=============
![BEST first search solved maze](http://dl.dropbox.com/u/848361/amazesolver/amaze_befs.png "BEST first search solved maze")

BUILD PREREQUISITES
-------------------

- Fairly recent GNU/Linux distribution with gcc compiler
  (Tested on ubuntu 10.04)
- Scons Build System http://www.scons.org/
  (Tested with Scons 1.2.0)

BUILD INSTRUCTIONS
------------------

- Enter the project directory (where Sconstruct file is located)
- Type scons
- Profit!

Binary will be in bin directory.

TEST THE PROGRAM
----------------

To test the program you just have to invoke the binary like this:

    ./maze -s strategy maze_path

Where maze_path is the path of the ascii maze and strategy is the search
strategy to find the goal. There are some test maze in res directory.

Strategy shoul be one of these:
  - DFS for depth first search
  - BFS for breadth first search (default)
  - BEFS  for best first search

For example, if you want to solve the first test maze with BEFS
just enter the bin dir and type:

    ./maze -s BEFS ../res/maze1.txt

If you want to see node animated exploration append -i:

    ./maze -s BEFS -i ../res/maze1.txt

CONTACT ME
----------

If you are interested in this little inutility feel free to
contact me at emanuele.rampichini@gmail.com
