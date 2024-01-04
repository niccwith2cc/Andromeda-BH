#ifndef BARNES_HUT
#define BARNES_HUT

#include<array>
#include "TreeNode.h"

using std::array;

class BarnesHut{
    private:
        double theta;

    public:
        TreeNode root;

        BarnesHut(CelestialBody* body, double th = 0.5);

        void insert(CelestialBody* body);

        array<double, 3> calculateForce(CelestialBody body, TreeNode node);
};

#endif