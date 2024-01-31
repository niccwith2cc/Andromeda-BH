#ifndef BARNES_HUT
#define BARNES_HUT

#include<array>
#include "TreeNode.h"

using std::array;
using std::unique_ptr;

class BarnesHut{
    private:
        double theta;

    public:
        unique_ptr<TreeNode> root; 

        BarnesHut(unique_ptr<CelestialBody> body, double th = 0.5);

        void insert(unique_ptr<CelestialBody> body);

        array<double, 3> calculateForce(CelestialBody body, const unique_ptr<TreeNode>& node);
};

#endif