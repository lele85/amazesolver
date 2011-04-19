#include <iostream>
#include <vector>
#include <list>
#include <string>
#include "search.h"
#include "maze.h"
#include <unistd.h>
#include <algorithm>
#include <sys/time.h>

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::list;
using std::string;

int main(int argc, char** argv) {

    string maze_path;
    SearchStrategy strategy = BFS;
    int c;
    string strategy_str;
    bool interactive = false;
    // Get options
    while ((c = getopt (argc, argv, "is:")) != -1){
        switch(c) {
            case's': {
                strategy_str = optarg;
                if (strategy_str == "DFS") {
                    strategy = DFS;
                } else if(strategy_str == "BFS") {
                    strategy = BFS;
                } else if(strategy_str == "BEFS") {
                    strategy = BEFS;
                } else {
                    cout << strategy_str << " is not a valid strategy." << endl;
                    return 1;
                } break;
            }
            case 'i': {
                    interactive = true;
            } break;
        }
    }
    // Get filename
    if (optind < argc){
        maze_path = (string)argv[optind];
    } else {
        cout << "No filename passed" << endl;
        return 1;
    }

    Maze itaca = Maze("Itaca");
    itaca.LoadFromFile(maze_path);
    static struct timeval startTime;
    static struct timeval stopTime;
    gettimeofday(&startTime, NULL);
    vector<MazeArc> closed_arcs = itaca.solve(strategy, interactive);
    gettimeofday(&stopTime, NULL);
    long ticks = (stopTime.tv_sec - startTime.tv_sec) * 1000 + (stopTime.tv_usec - startTime.tv_usec) / 1000;
    itaca.setSolution(closed_arcs, interactive);
    itaca.PrintMaze();
    cout << "Time to solve: " << ticks << "ms"<< endl ;
    cout << "Path lenght: " << itaca.getSolutionSathLenght() << endl;
    return 0;
}
