#include<vector>
#include<iostream> //remove later?

using namespace std;

class TreeNode{
    private:
        int totalMass;
        vector<double> centerOfMass = vector<double>(3);
        int depth;

    public:
        CelestialBody* external;
        vector<TreeNode*> internal;


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

        TreeNode(CelestialBody* ext){
            external = ext;
        }

        void updateCenterOfMass(CelestialBody* body){
            vector<double> newCenterOfMass(3);
            vector<double> bodyPosition = body->getPosition();
            int bodyMass = body->getMass();
            for (int i = 0; i < 3; i++){
                newCenterOfMass[i] = (centerOfMass[i]*totalMass + bodyPosition[i]*bodyMass) / (totalMass + bodyMass); 
            }
            centerOfMass = newCenterOfMass;
        }

        void insertNode(TreeNode node){
            if (external){
                internal = vector<TreeNode*>(8);
                TreeNode new_node = TreeNode(external);
                internal[0] = &new_node;
                internal[1] = &node;
                external = NULL;
            }
            else if (internal.size()){
                int octant = 0; //getOctant(node.external);
                if (internal[octant]){
                    internal[octant]->insertNode(*internal[octant]);
                    if (internal[octant]) internal[octant]->insertNode(node);
                }

                updateCenterOfMass(node.external);
                totalMass += node.external->getMass();
            }
            else{
                external = node.external;
            }
        }

        void traverseTree(TreeNode *root){
            if (root->external) std::cout << root->external->getMass() << std::endl;
            else if (root->internal.size()) {
                for (int i = 0; i < root->internal.size(); i++){
                    if (root->internal[i]) {
                        std::cout << i << " ";
                        traverseTree(root->internal[i]);
                    }
                }
            }
        }
};
