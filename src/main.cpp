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
    // Todo USE some sort of option parser library!!!
    string maze_path;
    SearchStrategy strategy;
    if (argc > 2) {
        maze_path = (string)argv[1];
        string strategy_str = (string)argv[2];
        if (strategy_str == "DFS") {
            strategy = DFS;
        } else if(strategy_str == "BFS") {
            strategy = BFS;
        } else if(strategy_str == "BEFS") {
            strategy = BEFS;
        }
        else {
            cout << argv[2] << " is not a valid strategy." << endl;
            return 1;
        }
    } else {
       cout << "Missing path or strategy!" << endl;
       return 1;
    }
    Maze itaca = Maze("Itaca");
    itaca.LoadFromFile(maze_path);
    //itaca.PrintMaze();
    static struct timeval startTime;
    static struct timeval stopTime;
    gettimeofday(&startTime, NULL);
    vector<MazeArc> closed_arcs = itaca.Solve(strategy);
    gettimeofday(&stopTime, NULL);
    long ticks = (stopTime.tv_sec - startTime.tv_sec) * 1000 + (stopTime.tv_usec - startTime.tv_usec) / 1000;

    if (closed_arcs.size() != 0) {
        int path_length = 0;
        MazeArc current_arc = *(closed_arcs.rbegin());
        while (current_arc.get_parent() != NULL) {
            if (!(current_arc.get_node()->HasProperty(START) ||current_arc.get_node()->HasProperty(GOAL))){
                current_arc.get_node()->SetProperty(SOLUTION);
                /*usleep(30000);
                for (int i = 0; i!=50; ++i){
                    cout << endl;
                }
                itaca.PrintMaze();
                ++path_length;
                */
                ++path_length;
            }
            for (vector<MazeArc>::iterator iter = closed_arcs.begin(); iter != closed_arcs.end(); ++iter){
                MazeNode* current_parent = current_arc.get_parent();
                MazeNode* parent_node = iter->get_node();
                if (current_parent == parent_node) {
                    current_arc = (*iter);
                    break;
                }
            }
        }
        itaca.PrintMaze();
        cout << "Time to solve: " << ticks << "ms"<< endl ;
        cout << "Path lenght: " << path_length << endl;
    }
    return 0;
}
