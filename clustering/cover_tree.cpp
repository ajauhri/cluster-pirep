#include<iostream>
#include<vector>
#include<process_pirep.hpp>
#include<cover_tree.hpp>
#include<algorithm>

using Eigen::MatrixXd;
float base = 1.3;
float il2 = 1./ log(base);

inline int get_scale(float d)
{
    return (int) ceilf(il2 * log(d));
}

inline int dist_of_scale(int scale)
{
    return pow(base, scale);
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

p_tree_node create_tree_node(const Eigen::VectorXd& p)
{
    p_tree_node leaf(new tree_node(p));
    return leaf;
}


// get the distance using the scale, if a point is within the distance put it in near_set else in far_set
void split(std::vector<p_node>& near_set, 
           std::vector<p_node>& far_set,
           int max_scale)
{
    unsigned int new_index = 0;
    float max_radius = dist_of_scale(max_scale); //gets the actual distance using - 2^max_scale or whatever the log base
    for (unsigned int i=0; i<near_set.size(); ++i)
    {
        if (near_set[i]->dist.back() > max_radius)
        {
            far_set.push_back(near_set[i]);
            near_set.erase(near_set.begin() + i);
        }
    }
}

void dist_split(std::vecot<p_node>& point_set,
                std::vector<p_node>& new_point_set,
                const Eigen::VectorXd& new_point,
                int max_scale)
{
    unsigned int new_index = 0;
    float fmax = dist_of_scale(max_scale);
    for (unsigned int i=0; i<point_set.size(); ++i)
    {
        float new_d = distane(new_point, point_set[i]->point, fmax);
        if (new_d <= fmax)
        {
            point_set[i].push_back(new_d);
            new_point_set.push_back(point_set[i]);
        }
        else
            point_set.erase(point_set.begin() + i);
    }
}

p_tree_node batch_insert(const Eigen::VectorXd& x,
                  int max_scale,
                  int top_scale,
                  std::vector<p_node>& point_set,
                  std::vector<p_node>& consumed_set,
                  std::vector<std::vector<p_node>>& stack) 
{
    if (point_set.size() == 0)
       return create_tree_node(x); 
    else
    {
        float max_dist = get_max(point_set);
        int next_scale = std::min(max_scale - 1, get_scale(max_dist));
        if (top_scale - next_scale == 100)
        {

        }
        else
        {
            std::vector<p_node> far = stack.size() != 0 ? stack.back() : stack.push_back(
            stack.pop_back();
            split(point_set, far, max_scale);
            p_tree_node child = batch_insert(x, next_scale, top_scale, point_set, consumed_set, stack);
            if (point_set.size() == 0)
            {
                stack.push_back(point_set);
                point_set = far;
                return child;
            }
            else
            {
                p_tree_node n = create_tree_node(x);
                std::vector<p_tree_node> children; 
                children.push_back(child);

                std::vector<p_node> new_point_set = stack.back(); 
                stack.pop_back();

                std::vector<p_node> new_consumed_set = stack.back();
                stack.pop_back();

                while (point_set.size() != 0) 
                {
                    VectorXd new_x = point_set.back()->point;
                    float new_dist = point_set.back()->dist.back();
                    consumed_set.push_back(point_set.back());
                    point_set.pop_back();

                    dist_split(point_set, new_point_set, new_x, max_scale);

                }
            }


        }
    }
    p_tree_node k;
    return k;
}

p_node batch_create(MatrixXd& X)
{
    assert(X.size() > 0);
    
    std::vector<p_node> point_set;
    std::vector<p_node> consumed_set;
    std::vector<std::vector<p_node>> stack;
    for (int i=1; i<X.rows(); ++i)
    {
        p_node n_i(new node);
        n_i->point = X.row(i);
        n_i->dist.push_back(distance(X.row(0), X.row(i)));
        point_set.push_back(n_i); 
    }

    float max_dist = get_max(point_set);
    /*batch_insert(X.row(0), 
                            get_scale(max_dist),
                            get_scale(max_dist),
                            point_set,
                            consumed_set,
                            stack);
     */                       
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

