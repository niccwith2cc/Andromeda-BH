#include <vector>
#include <cmath>
#include "TreeNode.cpp"
#include "CelestialBody.cpp"

class BarnesHut{
    private:
        static TreeNode root;

    public:
        BarnesHut(CelestialBody *body){
            root = TreeNode(body);
        }

        void insert(CelestialBody *body){
            root.insertBody(body);
        }

        vector<double> calculateForce(CelestialBody body, TreeNode node = root){ 
            if (root.external) return body.CalcCompF(*root.external);
            for (auto node: root.internal){
                
            }

        }
}