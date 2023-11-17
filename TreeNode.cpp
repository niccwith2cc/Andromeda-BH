#include<vector>
#include<iostream> //remove later?

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

            return index;
            
        }

        TreeNode(CelestialBody ext){
            external = &ext;
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

        void insertBody(TreeNode node){
            if (external){
                internal = vector<TreeNode*>(8);
                TreeNode new_node = TreeNode(*external);
                internal[getOctant(external)] = &new_node;
                internal[getOctant(node.external)] = &node;
                external = NULL;
            }
            else if (internal.size()){
                int octant = getOctant(node.external);
                if (internal[octant]){
                    internal[octant]->insertBody(*internal[octant]);
                    internal[octant]->insertBody(node);
                }

                updateCenterOfMass(*node.external);
                totalMass += node.external->getMass();
            }
            else{
                external = node.external;
            }
        }

        void traverseTree(TreeNode *root){
            std::cout << "Node: ";
            if (root->external) std::cout << root->external->getMass();
            if (root->internal.size()) {
                std:: cout << "Internal Node" << std::endl;
                for (auto node: root->internal){
                    traverseTree(node);
                }
            }
        }
};
