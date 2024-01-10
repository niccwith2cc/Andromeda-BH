#ifndef TREE_NODE
#define TREE_NODE

#include<array>
#include<memory>
#include"CelestialBody.h"
using std::array;

class TreeNode{
    private:
        int totalMass;
        int depth;


    public:
        std::unique_ptr<CelestialBody> external;
        array<std::unique_ptr<TreeNode>, 8> internal;
        array<double, 3> centerOfMass = array<double,3>();
        array<double, 3> centerOfOctant = array<double,3>();

        TreeNode();

        TreeNode(std::unique_ptr<CelestialBody> ext, int d = 1);

        int getDepth() const;

        int getTotalMass() const;

        int getOctant(const std::unique_ptr<CelestialBody>& body);

        array<double, 3> calculateCenterOfOctant(int octant);

        void updateCenterOfMass(std::unique_ptr<CelestialBody>& body);

        void insertBody(std::unique_ptr<CelestialBody>& body);

        void traverseTree(std::unique_ptr<TreeNode>& root);
};

#endif