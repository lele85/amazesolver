#ifndef MAZE_H
#define MAZE_H

#include <iostream>
#include <vector>
#include <string>

short enum MazeNodeProperty {
    WALKABLE,
    WALL,
    START,
    GOAL,
    VISITED,
    SOLUTION
};

short enum SearchStrategy {
    DFS,
    BFS,
    BEFS
};

class MazeNode{
    public:
        MazeNode();
        MazeNode(int x, int y);
        MazeNode(int x, int y, std::vector<MazeNodeProperty> properties);
        MazeNode(int x, int y, MazeNodeProperty property);
        int get_x();
        int get_y();
        double get_goal_distance();
        void set_goal_distance(double distance);
        void AddProperty(MazeNodeProperty property);
        bool HasProperty(MazeNodeProperty property);
        void RemoveProperty(MazeNodeProperty property);
        void ClearProperties();
        void SetProperty(MazeNodeProperty property);

    private:
        std::vector<MazeNodeProperty> properties_;
        unsigned int x_;
        unsigned int y_;
        double goal_distance_;

};

class MazeArc{
    public:
        MazeArc();
        MazeArc(MazeNode* node, MazeNode* parent);
        MazeNode* get_node();
        MazeNode* get_parent();

    private:
        MazeNode* node_;
        MazeNode* parent_;
};

class Maze {
    public:
        Maze();
        Maze(std::string name);
        void LoadFromFile(std::string path);
        void PrintMaze();
        void set_name(std::string name);
        std::string get_name();
        MazeNode* get_goal_node();
        MazeNode* get_start_node();
        static bool CompareArcDistance(MazeArc first, MazeArc second);
        std::vector<MazeArc> Solve(SearchStrategy strategy, bool interactive);
        void SetSolution(std::vector<MazeArc> closed_arcs, bool interactive);
        int get_solution_path_lenght();

    private:
        std::string name_;
        unsigned int solution_path_lenght_;
        std::vector<std::vector<MazeNode> > nodes_;
        MazeNode* goal_node_;
        MazeNode* start_node_;
        void InitNodesOfInterest();
        std::vector<MazeArc> Solve(MazeNode* current_node, SearchStrategy strategy, bool interactive);
        std::vector<MazeNode*> GetSurroundingAt(int row, int col);
        std::vector<MazeNode*> GetLegalSurroundingAt(int row, int col);
        MazeNode* GetNodeAt(unsigned int row,unsigned int col);
        void PopArcDfs(MazeArc& current_arc, std::vector<MazeArc>& open_arcs);
        void PopArcBfs(MazeArc& current_arc, std::vector<MazeArc>& open_arcs);
        void PopArcsBefs(MazeArc& current_arc, std::vector<MazeArc>& open_arcs);
        void PrintAndWaitFor(unsigned int useconds);
        void ExpandArcs(MazeArc& current_arc, std::vector<MazeArc>& open_arcs, std::vector<MazeArc>& closed_arcs);
};

#endif
