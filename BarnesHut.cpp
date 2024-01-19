#include <array>
#include <cmath>
#include <iostream>
#include "CelestialBody.h"
#include "BarnesHut.h"

const double BOUNDARY = 1000000.0;


using std::endl;
using std::array;


BarnesHut::BarnesHut(CelestialBody* body, double th){
    root = TreeNode(body);
    theta = th;
}

void BarnesHut::insert(CelestialBody* body){
    root.insertBody(body);
}

array<double, 3> BarnesHut::calculateForce(CelestialBody body, TreeNode node){ 
    if (node.external) return body.CalcCompF(*node.external);

    double s = 2*BOUNDARY*pow(0.5, node.getDepth()-1);
    CelestialBody tempBody = CelestialBody(node.getTotalMass(), node.centerOfMass, array<double,3>(), array<double,3>(), array<double,3>());
    double d = body.CalcR(tempBody);
    if (s/d < theta){  //the body is sufficiently far away from the center of mass.
        return body.CalcCompF(tempBody);
    }
    array<double, 3> force = {0.0,0.0,0.0};
    for (auto child: node.internal){
        if(child){ //check if child is null
            force = force + calculateForce(body, *child);
        } 
    }
    return force;
}
