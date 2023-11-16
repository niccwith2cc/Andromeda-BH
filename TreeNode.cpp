#include<vector>
#include "CelestialBody.cpp"

using namespace std;

class TreeNode{
    private:
        int totalMass;
        vector<double> centerOfMass = vector<double>(3);
        vector<TreeNode*> internal;
        CelestialBody* external;
        int depth;

    public:

        //get octant
        int getOctant(CelestialBody* body){
            vector<double> pos = body->getPosition();

            //replace center
            vector<double> center = vector<double>(3);

            int index = 0;

            for (int i = 0; i < 3; i++){
                index += (pos[i] > center[i]) * pow(2,i);
            }
            
        }

        TreeNode(CelestialBody* ext){
            external = ext;
        }

        void updateCenterOfMass(CelestialBody body){
            vector<double> newCenterOfMass(3);
            vector<double> bodyPosition = body.getPosition();
            int bodyMass = body.getMass();
            for (int i = 0; i < 3; i++){
                newCenterOfMass[i] = (centerOfMass[i]*totalMass + bodyPosition[i]*bodyMass) / (totalMass + bodyMass); 
            }
            centerOfMass = newCenterOfMass;
        }

        void insertBody(TreeNode* node){
            if (external){
                internal = vector<TreeNode*>(8);
                internal[getOctant(external)] = *TreeNode(external);
                internal[getOctant(node->external)] = node;
                external = NULL;
            }
            else if (internal.size()){
                int octant = getOctant(node->external);
                if (internal[octant]){
                    internal[octant]->insertBody(internal[octant]);
                    internal[octant]->insertBody(node);
                }

                updateCenterOfMass(*node->external);
                totalMass += *node->external.getMass();
            }
            else{
                external = *node->external;
            }
        }
}
