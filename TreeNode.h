#ifndef TREE_NODE
#define TREE_NODE

#include<array>
#include"CelestialBody.h"
using std::array;

// Represents the nodes of the octree 

class TreeNode{
    private:
        int totalMass;
        int depth;


    public:
        CelestialBody* external;
        array<TreeNode*, 8> internal;
        array<double, 3> centerOfMass = array<double,3>();
        array<double, 3> centerOfOctant = array<double,3>();

        TreeNode();

        TreeNode(CelestialBody* ext, int d = 1);

        int getDepth();

        int getTotalMass();

        int getOctant(CelestialBody* body);

        array<double, 3> calculateCenterOfOctant(int octant);

        void updateCenterOfMass(CelestialBody* body);

        void insertBody(CelestialBody* body);

        void traverseTree(TreeNode* root);
};

#endif

