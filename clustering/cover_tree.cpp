#include<iostream>
#include<vector>
#include<process_pirep.hpp>
#include<cover_tree.hpp>

using Eigen::MatrixXd;
float base = 1.3;
float il2 = 1./ log(base);
inline int get_scale(float d)
{
    return (int) ceilf(il2 * log(d));
}

//considers two vectors of equal size
//reasons for making it inline - small and called often 
inline float distance(const Eigen::VectorXd& p1, const Eigen::VectorXd& p2)
{
    assert(p1.size() == p2.size());
    float sum = 0.0f;
    for (int i=0; i<p1.size(); i++)
    {
        float d1 = p1(i) - p2(i);
        d1 *= d1;
        sum += d1;
    }
    return sqrt(sum);
}

float get_max(std::vector<p_node>& pts)
{
    float max = 0.0f;
    for (unsigned int i=0; i<pts.size(); ++i)
    {
        if (max < pts[i]->dist.back())
            max = pts[i]->dist.back();
    }
    assert(max != 0.0f);
    return max;
}

p_tree_node new_leaf(const Eigen::VectorXd& p)
{
    p_tree_node leaf(new tree_node(p));
    return leaf;
}

void batch_insert(const Eigen::VectorXd& x,
                  int max_scale,
                  int top_scale,
                  std::vector<p_node>& point_set,
                  std::vector<p_node>& consumed_set,
                  int stack) 
{

}

p_node batch_create(MatrixXd& X)
{
    assert(X.size() > 0);
    
    std::vector<p_node> point_set;
    std::vector<p_node> consumed_set;
    for (int i=1; i<X.rows(); ++i)
    {
        p_node n_i(new node);
        n_i->dist.push_back(distance(X.row(0), X.row(i)));
        point_set.push_back(n_i); 
    }

    float max_dist = get_max(point_set);
    batch_insert(X.row(0), 
                            get_scale(max_dist),
                            get_scale(max_dist),
                            point_set,
                            consumed_set,
                            0);
                            
    p_node n;
    return n;
}

int main(int argc, char **argv)
{
   // currently this method is parsing based and at all dynamic to consider many kinds of files.
   // it would be preferred to have a generic parser... 
   MatrixXd X = process_pirep(std::string("../data/small_cluster.csv")); 
   std::cout<<"parsing done\n";
   
   // here again this has data specific columns and it makes the usage limited. Need to expand here too...
   p_node top = batch_create(X);
   std::cout<<"batch creation done\n";

   return 0;
}

