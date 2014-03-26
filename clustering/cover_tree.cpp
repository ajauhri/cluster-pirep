#include<iostream>
#include<vector>
#include<process_pirep.hpp>
#include<cover_tree.hpp>
#include<algorithm>
#include<random>


using Eigen::MatrixXd;
const float g_scale = 1.3;
float il2 = 1./ log(g_scale);
std::mt19937 gen;

inline int get_scale(float d)
{
    return (int) ceilf(il2 * log(d));
}

unsigned int rand(unsigned int a, unsigned int b)
{
    std::uniform_int_distribution<> dis(a,b);
    return dis(gen);
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

p_tree_node create_tree_node(const Eigen::VectorXd& p)
{
    p_tree_node leaf(new tree_node(p));
    return leaf;
}

void get_children(const Eigen::VectorXd& p, std::vector<p_ds_node>& Qi_p_ds, int scale)
{
    for (unsigned int i=0; i<Qi_p_ds.size(); ++i)
    {
        if (Qi_p_ds[i]->node->children.find(scale) != Qi_p_ds[i]->node->children.end()) //if not empty key slot
        {
            for (unsigned int j=0; j<Qi_p_ds[i]->node->children[scale].size(); ++j)
            {
                p_tree_node& node = Qi_p_ds[i]->node->children[scale][j];
                p_ds_node ds(new ds_node(node, distance(node->point, p)));
                Qi_p_ds.push_back(ds);
            }
        }
    }
}

float get_min(std::vector<p_ds_node>& Qi_p_ds)
{
    float min = pow(g_scale, 100);
    for (unsigned int i=0; i<Qi_p_ds.size(); ++i)
    {
        if (Qi_p_ds[i]->dist < min)
            min = Qi_p_ds[i]->dist;
    }
    return min;
}

void insert(const Eigen::VectorXd& p, p_tree_node& root, int max_scale)
{
    int i = max_scale;
    std::vector<p_ds_node> Qi_p_ds;
    p_ds_node ds(new ds_node(root, distance(root->point, p)));
    Qi_p_ds.push_back(ds);
    int pi;
    p_tree_node parent; 
    while (1)
    {
        get_children(p, Qi_p_ds, i);
        // if all the cover set nodes have distance > 2^i
        float min_d_p_Q = get_min(Qi_p_ds);
        if (min_d_p_Q == 0)
            return;
        else if (min_d_p_Q > pow(g_scale, i))
            break;
        else 
        {
            float scale_dist = pow(g_scale, i);
            if (min_d_p_Q <= pow(g_scale, i))
            {
                while (1)
                {
                    int pos = rand(0, Qi_p_ds.size() - 1);
                    if (Qi_p_ds[pos]->dist <= scale_dist)
                    {
                        parent = Qi_p_ds[rand(0, Qi_p_ds.size() - 1)]->node;
                        pi = i;
                        break;
                    }
                }
            }
            
            // construct Q_{i-1}
            for (unsigned int i=0; i<Qi_p_ds.size(); ++i)
            {
                if (Qi_p_ds[i]->dist > scale_dist)
                    Qi_p_ds.erase(Qi_p_ds.begin() + i);
            }
            i--;
        }
    }
    parent->children[pi].push_back(create_tree_node(p));
    root->min_scale = std::min(root->min_scale, pi-1);
}


int main(int argc, char **argv)
{
    // currently this method is parsing based and at all dynamic to consider many kinds of files.
    // it would be preferred to have a generic parser... 
    MatrixXd X = process_pirep(std::string("../data/small_cluster.csv")); 
    std::cout<<"parsing done\n";

    float max_dist = 0.0f;
    for (int i=1; i<X.rows(); ++i)
    {
        float dist = distance(X.row(0), X.row(i));
        max_dist = max_dist < dist ? dist : max_dist;
    }
    p_tree_node root = create_tree_node(X.row(0));
    root->max_scale = get_scale(max_dist);
    root->min_scale = root->max_scale;

    for (int i = 1; i<X.rows(); ++i)
        insert(X.row(i), root, root->max_scale);

    p_tree_node temp = root;    

    // here again this has data specific columns and it makes the usage limited. Need to expand here too...
    //p_tree_node top = batch_create(X);
    std::cout<<"batch creation done\n";
    std::cout<<root->children.size(); 
    return 0;
}

