#include <vector>
#include <cmath>
#include <iostream>
#include "CelestialBody.cpp"
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

        vector<double> calculateForce(CelestialBody body, TreeNode node){ 
            if (node.external) return body.CalcCompF(*node.external);

            double s = 2*BOUNDARY*pow(0.5, node.getDepth()-1);
            CelestialBody tempBody = CelestialBody(node.getTotalMass(), node.centerOfMass, vector<double>(3), vector<double>(3), vector<double>(3));
            double d = body.CalcR(tempBody);
            //cout << s/d << endl;
            if (s/d < theta){  //the body is sufficiently far away from the center of mass.
                return body.CalcCompF(tempBody);
            }
            vector<double> force = vector<double>(3);
            for (auto child: node.internal){
                if(child){ //check if child is null
                    force = force + calculateForce(body, *child);
                } 
            }
            return force;
        }
};