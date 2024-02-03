#include <array>
#include <cmath>
#include <iostream>
#include "CelestialBody.h"
#include "BarnesHut.h"

using std::endl;
using std::array;
using std::unique_ptr;

BarnesHut::BarnesHut(){}

BarnesHut::BarnesHut(unique_ptr<CelestialBody> body, double boundary, double th){
    root = std::make_unique<TreeNode>(TreeNode(std::move(body)));
    root->setBoundary(boundary);
    BOUNDARY = boundary;
    theta = th;
}

double BarnesHut::BOUNDARY;

void BarnesHut::insert(unique_ptr<CelestialBody> body){
    root->insertBody(std::move(body));
}

array<double, 3> BarnesHut::calculateForce(CelestialBody body, const unique_ptr<TreeNode>& node){ 
    if (node->external) return body.CalcCompF(*(node->external));

    double s = 2*BOUNDARY*pow(0.5, node->getDepth()-1);
    CelestialBody tempBody = CelestialBody(node->getTotalMass(), node->centerOfMass, array<double,3>(), array<double,3>(), array<double,3>());
    double d = body.CalcR(tempBody);
    
    if (s/d < theta){  //the body is sufficiently far away from the center of mass.
        return body.CalcCompF(tempBody);
    }
    array<double, 3> force = {0.0,0.0,0.0};
    for (auto &child: node->internal){
        if(child){ //check if child is null
            force = force + calculateForce(body, child);
        } 
    }
    return force;
}
