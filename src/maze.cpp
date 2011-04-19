#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <unistd.h>
#include <math.h>
#include "maze.h"

using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::vector;
using std::find;
using std::sort;

MazeNode::MazeNode(){
}

MazeNode::MazeNode(int x, int y){
    x_ = x;
    y_ = y;
}
MazeNode::MazeNode(int x, int y, MazeNodeProperty property){
    x_ = x;
    y_ = y;
    properties_.push_back(property);
}

MazeNode::MazeNode(int x, int y, vector<MazeNodeProperty> properties){
    x_ = x;
    y_ = y;
    properties_ = properties;
}

int MazeNode::get_x(){
    return x_;
}

int MazeNode::get_y(){
    return y_;
}

double MazeNode::get_goal_distance(){
    return goal_distance_;
}

void MazeNode::set_goal_distance(double distance){
    goal_distance_ = distance;
}

bool MazeNode::HasProperty(MazeNodeProperty property){
    for (vector<MazeNodeProperty>::const_iterator i = properties_.begin(); i != properties_.end(); ++i){
        if ((*i) == property) {
            return true;
        }
    }
    return false;
}
void MazeNode::AddProperty(MazeNodeProperty property){
    if (!HasProperty(property)){
        properties_.push_back(property);
    }
}

void MazeNode::RemoveProperty(MazeNodeProperty property){
    for (vector<MazeNodeProperty>::iterator i = properties_.begin(); i != properties_.end(); ++i){
        // No need to care for iterator validity after erase cause we have max 1 property per type
        if ((*i) == property) {
            properties_.erase(i);
            break; // Property erased, continuing the loop is pointless
        }
    }
}

void MazeNode::ClearProperties(){
    properties_.clear();
}

void MazeNode::SetProperty(MazeNodeProperty property) {
    ClearProperties();
    AddProperty(property);
}

MazeArc::MazeArc(MazeNode* node, MazeNode* parent){
    node_ = node;
    parent_ = parent;
}

MazeNode* MazeArc::get_node(){
    return node_;
}

MazeNode* MazeArc::get_parent(){
    return parent_;
}

Maze::Maze(){
}
Maze::Maze(string name){
    name_ = name;
}
void Maze::LoadFromFile(string path){
    ifstream infile(path.c_str());
    if (infile) {
        string s;
        nodes_.clear();
        //Populate the _maze
        int row = 0;
        while (getline(infile,s)) {
            vector<MazeNode> row_vector;
            string::const_iterator string_iter = s.begin();
            int col = 0;
            while (string_iter != s.end()){
                char c = (*string_iter);
                switch (c) {
                    case '#': {
                        MazeNode n = MazeNode(row,col,WALL);
                        row_vector.push_back(n);
                    } break;
                    case 'S':{
                        MazeNode n = MazeNode(row,col,START);
                        row_vector.push_back(n);
                    } break;
                    case 'E': {
                        MazeNode n = MazeNode(row,col,GOAL);
                        row_vector.push_back(n);
                    } break;
                    default: {
                        MazeNode n = MazeNode(row,col,WALKABLE);
                        row_vector.push_back(n);
                    } break;
                }
                ++string_iter;
                ++col;
            }
            nodes_.push_back(row_vector);
            ++row;
        }   
    } else {
        cout << "File " << path << " doesn't exist." << endl;
        exit(1);
    }
    // Initialize interesting nodes for direct access
    InitNodesOfInterest();
}

void Maze::PrintMaze(){
    const string red_color_start ="\033[1;31m";
    const string red_color_end ="\033[0m";
    const string green_color_start = "\033[1;32m";
    const string green_color_end ="\033[0m";
    const string yellow_color_start = "\033[1;33m";
    const string yellow_color_end ="\033[0m";
    const string blue_color_start = "\033[1;34m";
    const string blue_color_end ="\033[0m";

    for (vector<vector<MazeNode> >::const_iterator row_iterator = nodes_.begin(); row_iterator != nodes_.end(); ++row_iterator){
        for (vector<MazeNode>::const_iterator col_iterator = row_iterator->begin(); col_iterator != row_iterator->end(); ++col_iterator){
            MazeNode node = (*col_iterator);
            if (node.HasProperty(WALL)){
                cout << "#";
            }
            if (node.HasProperty(START)){
                cout << yellow_color_start << "S" << yellow_color_end;
            }
            if (node.HasProperty(GOAL)){
                cout << green_color_start << "G" << green_color_end;
            }
            if (node.HasProperty(WALKABLE)){
                cout << " ";
            }
            if (node.HasProperty(VISITED)){
                cout << red_color_start << "*" << red_color_end;
            }
            if (node.HasProperty(SOLUTION)){
                cout << blue_color_start << "o" << blue_color_end;
            }
        }
        cout << endl;
    }
}

void Maze::set_name(const std::string name) {
    name_ = name;
}

std::string Maze::get_name() {
    return name_;
}

MazeNode* Maze::get_goal_node() {
    return goal_node_;
}

MazeNode* Maze::get_start_node() {
    return start_node_;
}

MazeNode* Maze::GetNodeAt(unsigned int row, unsigned int col){
    if ((row < nodes_.size()) && (col < nodes_[row].size())) {
        return &nodes_[row][col];
    }
    return NULL;
}

vector<MazeNode*> Maze::GetSurroundingAt(int row, int col) {
    vector<MazeNode*> surrounding;
    // check if central node exists
    MazeNode* target = GetNodeAt(row,col);
    if (target == NULL) {
        return surrounding;
    }
    // fetch surrounding nodes
    MazeNode* top = GetNodeAt(row-1,col);
    MazeNode* right = GetNodeAt(row,col+1);
    MazeNode* bottom = GetNodeAt(row+1,col);
    MazeNode* left = GetNodeAt(row, col-1);
    // check for null pointers and return not null node references
    if (top != NULL) { surrounding.push_back(top); }
    if (right != NULL) { surrounding.push_back(right); }
    if (bottom != NULL) { surrounding.push_back(bottom); }
    if (left != NULL) { surrounding.push_back(left); }
    return surrounding;
}

vector<MazeNode*> Maze::GetLegalSurroundingAt(int row, int col) {
    vector<MazeNode*> surrounding = GetSurroundingAt(row,col);
    vector<MazeNode*> legal_surrounding;
    for (vector<MazeNode*>::const_iterator iter = surrounding.begin(); iter != surrounding.end(); ++iter ){
        if ((*(*iter)).HasProperty(WALKABLE)) {
            legal_surrounding.push_back((*iter));
        } else if ((*(*iter)).HasProperty(GOAL)) {
            legal_surrounding.push_back((*iter));
        }
    }
    return legal_surrounding;
}

void Maze::InitNodesOfInterest() {
    //Set start node and goal node
    for (vector<vector<MazeNode> >::iterator row_iterator = nodes_.begin(); row_iterator != nodes_.end(); ++row_iterator){
        for (vector<MazeNode>::iterator col_iterator = row_iterator->begin(); col_iterator != row_iterator->end(); ++col_iterator){
            if (col_iterator->HasProperty(GOAL)){
                goal_node_ = &(*col_iterator);
            }
            if (col_iterator->HasProperty(START)){
                start_node_ = &(*col_iterator);
            }
        }
    }
    // Set distance from goal for every node
    for (vector<vector<MazeNode> >::iterator row_iterator = nodes_.begin(); row_iterator != nodes_.end(); ++row_iterator){
        for (vector<MazeNode>::iterator col_iterator = row_iterator->begin(); col_iterator != row_iterator->end(); ++col_iterator){
            int current_x,current_y,goal_x,goal_y;
            current_x = col_iterator->get_x();
            current_y = col_iterator->get_y();
            goal_x = goal_node_->get_x();
            goal_y = goal_node_->get_y();
            double distance = sqrt(pow((current_x-goal_x),2)+pow((current_y-goal_y),2));
            col_iterator->set_goal_distance(distance);
        }
    }
}

vector<MazeArc> Maze::Solve(SearchStrategy strategy, bool interactive){
    return Solve(start_node_, strategy, interactive);
}

vector<MazeArc> Maze::Solve(MazeNode* start_node, SearchStrategy strategy, bool interactive){
    vector<MazeArc> closed_arcs;
    switch (strategy){
        case DFS:{
            vector<MazeArc> open_arcs;
            MazeArc current_arc = MazeArc(start_node, NULL);
            open_arcs.push_back(current_arc);
            while (open_arcs.size() != 0) {
                PopArcDfs(current_arc, open_arcs);
                closed_arcs.push_back(current_arc);
                if (!(current_arc.get_node()->HasProperty(START) ||current_arc.get_node()->HasProperty(GOAL))){
                    current_arc.get_node()->SetProperty(VISITED);
                }
                if (interactive) PrintAndWaitFor(40000);
                if (current_arc.get_node()->HasProperty(GOAL)) {
                    closed_arcs.push_back(current_arc);
                    return closed_arcs;
                } else {
                    ExpandArcs(current_arc, open_arcs, closed_arcs);
                }
            }
        } break;
        case BFS:{
            //TODO: needs optimizations... vector is not the appropriate container
            vector<MazeArc> open_arcs;
            vector<MazeArc> closed_arcs;
            MazeArc current_arc = MazeArc(start_node, NULL);
            open_arcs.push_back(current_arc);
            while (open_arcs.size() != 0) {
                PopArcBfs(current_arc, open_arcs);
                closed_arcs.push_back(current_arc);
                if (!(current_arc.get_node()->HasProperty(START) ||current_arc.get_node()->HasProperty(GOAL))){
                    current_arc.get_node()->SetProperty(VISITED);
                }
                // If in interactive mode print the maze and wait
                if (interactive) PrintAndWaitFor(40000);
                if (current_arc.get_node()->HasProperty(GOAL)) {
                    closed_arcs.push_back(current_arc);
                    return closed_arcs;
                } else {
                    ExpandArcs(current_arc, open_arcs, closed_arcs);
                }
            }
        };break;
        case BEFS:{
            vector<MazeArc> open_arcs;
            vector<MazeArc> closed_arcs;
            MazeArc current_arc = MazeArc(start_node, NULL);
            open_arcs.push_back(current_arc);
            while (open_arcs.size() != 0) {
                PopArcsBefs(current_arc, open_arcs);
                closed_arcs.push_back(current_arc);
                if (!(current_arc.get_node()->HasProperty(START) ||current_arc.get_node()->HasProperty(GOAL))){
                    current_arc.get_node()->SetProperty(VISITED);
                }
                if (interactive) PrintAndWaitFor(40000);
                if (current_arc.get_node()->HasProperty(GOAL)) {
                    closed_arcs.push_back(current_arc);
                    return closed_arcs;
                } else {
                    ExpandArcs(current_arc, open_arcs, closed_arcs);
                }
            }
        } break;
    }
    closed_arcs.clear();
    return closed_arcs;
}

bool Maze::CompareArcDistance(MazeArc first, MazeArc second) {
    return second.get_node()->get_goal_distance() < first.get_node()->get_goal_distance();
}

void Maze::PopArcDfs(MazeArc& current_arc, vector<MazeArc>& open_arcs){
    current_arc = (*(open_arcs.end()-1));
    open_arcs.erase(open_arcs.end()-1);
}

void Maze::PopArcBfs(MazeArc& current_arc, vector<MazeArc>& open_arcs){
    current_arc = (*(open_arcs.begin()));
    open_arcs.erase(open_arcs.begin());
}

void Maze::PopArcsBefs(MazeArc& current_arc, vector<MazeArc>& open_arcs){
    sort(open_arcs.begin(), open_arcs.end(), Maze::CompareArcDistance);
    current_arc = (*(open_arcs.end()-1));
    open_arcs.erase(open_arcs.end()-1);
}

void Maze::PrintAndWaitFor(unsigned int useconds){
    usleep(useconds);
    for (int i = 0; i!=50; ++i){
        cout << endl;
    }
    PrintMaze();
}

void Maze::ExpandArcs(MazeArc &current_arc, std::vector<MazeArc> &open_arcs, std::vector<MazeArc> &closed_arcs){
    vector<MazeNode*> surrounding_nodes = GetLegalSurroundingAt(current_arc.get_node()->get_x(), current_arc.get_node()->get_y());
    for (vector<MazeNode*>::iterator nodes_iter = surrounding_nodes.begin() ; nodes_iter != surrounding_nodes.end(); ++nodes_iter){
        bool visited = false;
        for (vector<MazeArc>::iterator open_arcs_iter = open_arcs.begin(); open_arcs_iter != open_arcs.end(); ++open_arcs_iter){
            if ((*nodes_iter) == (open_arcs_iter->get_node())){
                visited = true;
            }
        }
        for (vector<MazeArc>::iterator closed_arcs_iter = closed_arcs.begin(); closed_arcs_iter != closed_arcs.end(); ++closed_arcs_iter){
            if ((*nodes_iter) == (closed_arcs_iter->get_node())){
                visited = true;
            }
        }
        if (!visited) {
            MazeArc arc = MazeArc((*nodes_iter), current_arc.get_node());
            open_arcs.push_back(arc);
        }
    }
}

void Maze::SetSolution(vector<MazeArc> closed_arcs, bool interactive) {
    if (closed_arcs.size() != 0) {
        solution_path_lenght_ = 0;
        MazeArc current_arc = *(closed_arcs.rbegin());
        while (current_arc.get_parent() != NULL) {
            if (!(current_arc.get_node()->HasProperty(START) ||current_arc.get_node()->HasProperty(GOAL))){
                current_arc.get_node()->SetProperty(SOLUTION);
                if (interactive) PrintAndWaitFor(40000);
                ++solution_path_lenght_;
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
    }
}

int Maze::get_solution_path_lenght(){
    return solution_path_lenght_;
}
