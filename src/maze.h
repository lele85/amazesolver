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
        int getX() const;
        int getY() const;
        double getGoalDistance() const;
        void setGoalDistance(double distance);
        void addProperty(MazeNodeProperty property);
        bool hasProperty(MazeNodeProperty property);
        void removeProperty(MazeNodeProperty property);
        void clearProperties();
        void setProperty(MazeNodeProperty property);

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
        MazeNode* getNode() const;
        MazeNode* getParent() const;

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
        void setName(std::string name);
        std::string getName() const;
        MazeNode* getGoalNode() const;
        MazeNode* getStartNode() const;
        static bool compareArcDistance(MazeArc first, MazeArc second);
        std::vector<MazeArc> solve(SearchStrategy strategy, bool interactive);
        void setSolution(std::vector<MazeArc> closed_arcs, bool interactive);
        int getSolutionSathLenght() const;

    private:
        std::string name_;
        unsigned int solution_path_lenght_;
        std::vector<std::vector<MazeNode> > nodes_;
        MazeNode* goal_node_;
        MazeNode* start_node_;
        void initNodesOfInterest();
        std::vector<MazeArc> solve(MazeNode* current_node, SearchStrategy strategy, bool interactive);
        std::vector<MazeNode*> getSurroundingAt(int row, int col);
        std::vector<MazeNode*> getLegalSurroundingAt(int row, int col);
        MazeNode* getNodeAt(unsigned int row,unsigned int col);
        void popArcDfs(MazeArc& current_arc, std::vector<MazeArc>& open_arcs);
        void popArcBfs(MazeArc& current_arc, std::vector<MazeArc>& open_arcs);
        void popArcsBefs(MazeArc& current_arc, std::vector<MazeArc>& open_arcs);
        void printAndWaitFor(unsigned int useconds);
        void expandArcs(MazeArc& current_arc, std::vector<MazeArc>& open_arcs, std::vector<MazeArc>& closed_arcs);
};

#endif
