#ifndef TREE_NODE
#define TREE_NODE

#include <array>
#include <memory>
#include"CelestialBody.h"
using std::array;
using std::unique_ptr;

// Represents the nodes of the octree 

class TreeNode{
    private:
        int totalMass;
        int depth;
        static double BOUNDARY;

    public:
        unique_ptr<CelestialBody> external;
        array<unique_ptr<TreeNode>, 8> internal;
        array<double, 3> centerOfMass = array<double,3>();
        array<double, 3> centerOfOctant = array<double,3>();

        TreeNode();

        TreeNode(unique_ptr<CelestialBody> ext, int d = 1);

        int getDepth();

        int getTotalMass();

        int getOctant(const unique_ptr<CelestialBody>& body);

        array<double, 3> calculateCenterOfOctant(int octant);

        void updateCenterOfMass(const unique_ptr<CelestialBody>& body);

        void insertBody(unique_ptr<CelestialBody> body);

        void setBoundary(double boundary);

        void traverseTree(unique_ptr<TreeNode> root);
};

#endif

