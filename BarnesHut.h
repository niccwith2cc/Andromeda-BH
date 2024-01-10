#ifndef BARNES_HUT
#define BARNES_HUT

#include<array>
#include "TreeNode.h"

using std::array;

class BarnesHut{
    private:
        double theta;

    public:
        std::unique_ptr<TreeNode> root;

        BarnesHut(std::unique_ptr<CelestialBody> body, double th = 0.5);

        void insert(std::unique_ptr<CelestialBody> body);

        array<double, 3> calculateForce(std::unique_ptr<CelestialBody>& body, std::unique_ptr<TreeNode>& node);
};

#endif