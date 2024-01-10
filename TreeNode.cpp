#include<iostream>
#include<cmath>
#include"TreeNode.h"

// Creating an Octree to place all the celestial bodies in them.
constexpr double BOUNDARY = 1000000.0;


TreeNode::TreeNode(){}

TreeNode::TreeNode(std::unique_ptr<CelestialBody> ext, int d)
    : external(std::move(ext)), depth(d), centerOfMass(external->getPosition()),totalMass(external->getMass()){
}

int TreeNode::getDepth() const {
    return depth;
}

int TreeNode::getTotalMass() const{
    return totalMass;
}

//get octant
int TreeNode::getOctant(const std::unique_ptr<CelestialBody>& body){
    array<double, 3> pos = body->getPosition();

    int index = 0;

    for (int i = 0; i < 3; i++){
        index += (pos[i] > centerOfOctant[i]) * pow(2, 2 - i); //I dare you to remember why you did 2-i
    }
    return index;
    
}

array<double, 3> TreeNode::calculateCenterOfOctant(int octant){
    array<double, 3> centerOfChild;
    for (int i = 0; i < 3; i++){
        centerOfChild[i] = centerOfOctant[i]/1.0 + pow(0.5, depth + 1) * BOUNDARY * pow(-1, !(octant % 2)); 
        octant /= 2;
    }
    return centerOfChild;
}

void TreeNode::updateCenterOfMass(std::unique_ptr<CelestialBody>& body){
    array<double, 3> newCenterOfMass;
    array<double, 3> bodyPosition = body->getPosition();
    int bodyMass = body->getMass();
    for (int i = 0; i < 3; i++){
        newCenterOfMass[i] = (centerOfMass[i]*totalMass + bodyPosition[i]*bodyMass) / static_cast<double>(totalMass + bodyMass); 
    }
    centerOfMass = newCenterOfMass;

}

void TreeNode::insertBody(std::unique_ptr<CelestialBody>& body){
    if (external){
        internal = array<std::unique_ptr<TreeNode>, 8>();

        int octant = getOctant(external); 
        std::unique_ptr<TreeNode> newNode = std::make_unique<TreeNode>(std::move(body), depth + 1); TreeNode(std::move(external), depth + 1);
        newNode->centerOfOctant = calculateCenterOfOctant(octant);
        internal[octant] = std::move(newNode);

        octant = getOctant(body);
        if (internal[octant]) { 
            internal[octant]->insertBody(body); 
            }
        else {
            newNode = std::make_unique<TreeNode>(std::move(body), depth + 1);
            newNode->centerOfOctant = calculateCenterOfOctant(octant);
            internal[octant] = std::move(newNode);
        } 
        
        updateCenterOfMass(body);
        totalMass += body->getMass();

        external = nullptr;  
    }
    else {
        int octant = getOctant(body);
        std::unique_ptr<TreeNode> octantNode = std::move(internal[octant]);
        if (octantNode) octantNode->insertBody(body);
        else{
            std::unique_ptr<TreeNode> newNode = std::make_unique<TreeNode>(std::move(body), depth + 1);
            newNode->centerOfOctant = calculateCenterOfOctant(octant);
            internal[octant] = std::move(newNode);
        } 
        updateCenterOfMass(body);
        totalMass += body->getMass();
    }
}

void TreeNode::traverseTree(std::unique_ptr<TreeNode>& root){
    if (root->external) std::cout << root->external->getMass() << " " << root->depth << std::endl;
    else if (root->internal.size()) {
        for (int i = 0; i < root->internal.size(); i++){
            if (root->internal[i]) {
                //cout << centerOfMass[0] << " " << centerOfMass[1] << " " << centerOfMass[2] << " depth " << depth << endl;
                traverseTree(root->internal[i]);
            }
        }
    }
}

