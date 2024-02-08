#include<iostream>
#include<cmath>
#include"TreeNode.h"

using std::make_unique;

TreeNode::TreeNode(){}

TreeNode::TreeNode(unique_ptr<CelestialBody> ext, int d){
    external = std::move(ext);
    //std::cout << d << std::endl;
    depth = d;
    centerOfMass = external->getPosition();
    totalMass = external->getMass();
}

double TreeNode::BOUNDARY;

void TreeNode::setBoundary(double boundary){
    BOUNDARY = boundary;
}

int TreeNode::getDepth(){
    return depth;
}

int TreeNode::getTotalMass(){
    return totalMass;
}

//get octant
int TreeNode::getOctant(const unique_ptr<CelestialBody>& body){
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

void TreeNode::updateCenterOfMass(const unique_ptr<CelestialBody>& body){
    array<double, 3> newCenterOfMass;
    array<double, 3> bodyPosition = body->getPosition();
    int bodyMass = body->getMass();
    for (int i = 0; i < 3; i++){
        newCenterOfMass[i] = (centerOfMass[i]*totalMass + bodyPosition[i]*bodyMass) / static_cast<double>(totalMass + bodyMass); 
    }
    centerOfMass = newCenterOfMass;

}

void TreeNode::insertBody(unique_ptr<CelestialBody> body){

    updateCenterOfMass(body);
    totalMass += body->getMass();

    if (external){
        
        internal = array<unique_ptr<TreeNode>, 8>();

        int octant = getOctant(external); 
        //std::cout << octant << std::endl;
        unique_ptr<TreeNode> newNode(new TreeNode(std::move(external), depth + 1));
        newNode->centerOfOctant = calculateCenterOfOctant(octant);
        internal[octant] = std::move(newNode);
    }
    int octant = getOctant(body);
    if (internal[octant]) internal[octant]->insertBody(std::move(body));
    else{
        unique_ptr<TreeNode> newNode(new TreeNode(std::move(body), depth + 1));
        newNode->centerOfOctant = calculateCenterOfOctant(octant);
        internal[octant] = std::move(newNode);
    } 
}


void TreeNode::traverseTree(unique_ptr<TreeNode>& root){
    if (!root) return;
    if (root->external) std::cout << root->depth << std::endl;
    else {
        for (int i = 0; i < root->internal.size(); i++){
            traverseTree(root->internal[i]);
        }
    }
}
