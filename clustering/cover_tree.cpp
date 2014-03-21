#include<iostream>
#include<Eigen/Dense>
#include<process_pirep.hpp>

using Eigen::MatrixXd;

int main(int argc, char **argv)
{
   MatrixXd X = process_pirep(std::string("../data/small.csv")); 
   std::cout<<std::fixed<<X;
   return 0;
}

