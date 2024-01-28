#include<iostream>
#include<cmath>
#include"TreeNode.h"

constexpr double BOUNDARY = 1000000.0;


TreeNode::TreeNode(){}

TreeNode::TreeNode(CelestialBody* ext, int d){
    external = ext;
    depth = d;
    centerOfMass = ext->getPosition();
    totalMass = ext->getMass();
}

int TreeNode::getDepth(){
    return depth;
}

int TreeNode::getTotalMass(){
    return totalMass;
}

//get octant
int TreeNode::getOctant(CelestialBody* body){
    array<double, 3> pos = body->getPosition();

    int index = 0;

    for (int i = 0; i < 3; i++){
        index += (pos[i] > centerOfOctant[i]) * pow(2, 2 - i); // used to map the body to its corresponding octant based on its position
    }
    return index;
    
}

array<double, 3> TreeNode::calculateCenterOfOctant(int octant){
    array<double, 3> centerOfChild;
    for (int i = 0; i < 3; i++){
        centerOfChild[i] = centerOfOctant[i]/1.0 + pow(0.5, depth) * BOUNDARY * pow(-1, !(octant % 2)); 
        octant /= 2;
    }
    return centerOfChild;
}

void TreeNode::updateCenterOfMass(CelestialBody* body){
    array<double, 3> newCenterOfMass;
    array<double, 3> bodyPosition = body->getPosition();
    int bodyMass = body->getMass();
    for (int i = 0; i < 3; i++){
        newCenterOfMass[i] = (centerOfMass[i]*totalMass + bodyPosition[i]*bodyMass) / static_cast<double>(totalMass + bodyMass); 
    }
    centerOfMass = newCenterOfMass;

}

void TreeNode::insertBody(CelestialBody* body){
    if (external){
        //std::cout << external->getPosition()[0] << " " << external->getPosition()[1] << " " << external->getPosition()[2] << " " << body->getPosition()[0] << " " << body->getPosition()[1] << " " << body->getPosition()[2] <<std::endl; 
        //std::cout << depth <<std::endl;
        
        internal = array<TreeNode*, 8>();

        int octant = getOctant(external); 
        //std::cout << octant << std::endl;
        TreeNode* newNode = new TreeNode(external, depth + 1);
        newNode->centerOfOctant = calculateCenterOfOctant(octant);
        internal[octant] = newNode;

        octant = getOctant(body);
        if (internal[octant]) { 
            internal[octant]->insertBody(body); 
            }
        else {
            newNode = new TreeNode(body, depth + 1);
            newNode->centerOfOctant = calculateCenterOfOctant(octant);
            internal[octant] = newNode;
        } 
        
        updateCenterOfMass(body);
        totalMass += body->getMass();

        external = nullptr;  
    }
    else {
        int octant = getOctant(body);
        TreeNode* octantNode = internal[octant];
        if (octantNode) octantNode->insertBody(body);
        else{
            TreeNode* newNode = new TreeNode(body, depth + 1);
            newNode->centerOfOctant = calculateCenterOfOctant(octant);
            internal[octant] = newNode;
        } 
        updateCenterOfMass(body);
        totalMass += body->getMass();
    }
}

void TreeNode::traverseTree(TreeNode* root){
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
