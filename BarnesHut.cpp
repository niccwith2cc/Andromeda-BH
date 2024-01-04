#include <vector>
#include <cmath>
#include <iostream>
#include "CelestialBody.h"
#include "TreeNode.cpp"

using std::cout;
using std::endl;

class BarnesHut{
    private:
        double theta;

    public:

        TreeNode root;

        BarnesHut(CelestialBody* body, double th = 0.5){
            root = TreeNode(body);
            theta = th;
        }

        void insert(CelestialBody* body){
            root.insertBody(body);
        }

        array<double,3> calculateForce(CelestialBody body, TreeNode node){ 
            if (node.external) return body.CalcCompF(*node.external);

            double s = 2*BOUNDARY*pow(0.5, node.getDepth()-1);
            CelestialBody tempBody = CelestialBody(node.getTotalMass(), node.centerOfMass, {0.0,0.0,0.0}, {0.0,0.0,0.0}, {0.0,0.0,0.0}); 
            double d = body.CalcR(tempBody);
            //cout << s/d << endl;
            if (s/d < theta){  //the body is sufficiently far away from the center of mass.
                return body.CalcCompF(tempBody);
            }
            array<double,3> force = {0.0,0.0,0.0};
            for (auto child: node.internal){
                if(child){ //check if child is null
                    force = force + calculateForce(body, *child);
                } 
            }
            return force;
        }
};