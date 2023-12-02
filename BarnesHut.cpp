#include <vector>
#include <cmath>
#include "TreeNode.cpp"
#include "CelestialBody.cpp"

class BarnesHut{
    private:
        static TreeNode root;
        double theta = 0.5;

    public:
        BarnesHut(CelestialBody *body){
            root = TreeNode(body);
        }

        void insert(CelestialBody *body){
            root.insertBody(body);
        }

        vector<double> calculateForce(CelestialBody body, TreeNode node = root){ 
            if (root.external) return body.CalcCompF(*root.external);
            double s = 2*BOUNDARY*pow(0.5, node.getDepth()-1);
            CelestialBody tempBody = CelestialBody(node.getTotalMass(), node.centerOfMass, vector<double>(3), vector<double>(3), vector<double>(3));
            double d = body.CalcR(tempBody);
            if (s/d < theta){
                
            }

            for (auto node: root.internal){

            }

        }
};