#ifndef COVER_TREE_INCLUDED
#define COVER_TREE_INCLUDED
#include<boost/shared_ptr.hpp>
#include<vector>
#include<Eigen/Dense>

struct node
{
  //point p;
    std::vector<float> dist;
};

struct tree_node
{
    Eigen::VectorXd point;
    float max_dist; // the max distance to any grandchild
    float parent_dist; // the distance to any parent 
    std::vector<boost::shared_ptr<tree_node>> children;
    short int scale; // an upper bound on the distance to any child

    tree_node(Eigen::VectorXd p) : point(p), max_dist(0.0f), parent_dist(0.0f), children(NULL), scale(100) 
            { }
};

typedef boost::shared_ptr<node> p_node;
typedef boost::shared_ptr<tree_node> p_tree_node;
#endif

