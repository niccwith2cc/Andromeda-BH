#include<vector>
#include<iostream> //remove later?

using namespace std;

const double BOUNDARY = 1000000.0;

class TreeNode{
    private:
        int totalMass;
        vector<double> centerOfMass = vector<double>(3);
        int depth;


    public:
        CelestialBody* external;
        vector<TreeNode*> internal;
        vector<double> centerOfOctant = vector<double>(3);


        //get octant
        int getOctant(CelestialBody* body){
            vector<double> pos = body->getPosition();

            int index = 0;

            for (int i = 0; i < 3; i++){
                index += (pos[i] > centerOfOctant[i]) * pow(2, 2 - i); //I dare you to remember why you did 2-i
            }
            return index;
            
        }

        vector<double> calculateCenterOfOctant(int octant){
            vector<double> centerOfChild = vector<double>(3);
            for (int i = 0; i < 3; i++){
                centerOfChild[i] = centerOfOctant[i]/1.0 + pow(0.5, depth + 1) * BOUNDARY * pow(-1, !(octant % 2)); 
                octant /= 2;
            }
            return centerOfChild;
        }

        TreeNode(CelestialBody* ext, int d = 1){
            external = ext;
            depth = d;
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

        void insertBody(CelestialBody* body){
            if (external){
                internal = vector<TreeNode*>(8);

                int octant = getOctant(external); 
                TreeNode* newNode = new TreeNode(external, depth + 1);
                newNode->centerOfOctant = calculateCenterOfOctant(octant);
                internal[octant] = newNode;

                octant = getOctant(body);
                if (internal[octant]) { internal[octant]->insertBody(body); }
                else {
                    newNode = new TreeNode(body, depth + 1);
                    newNode->centerOfOctant = calculateCenterOfOctant(octant);
                    internal[octant] = newNode;
                } 

                external = NULL;  
            }
            else if (internal.size()){
                int octant = getOctant(body);
                TreeNode* octantNode = internal[octant];
                if (octantNode) octantNode->insertBody(body);
                else{
                    TreeNode* newNode = new TreeNode(body, depth + 1);
                    newNode->centerOfOctant = calculateCenterOfOctant(octant);
                    internal[octant] = newNode;
                } 
                updateCenterOfMass(body);
                totalMass += body->getMass();
            }
            else {
                external = body;
            }
        }


        void traverseTree(TreeNode* root){
            if (root->external) std::cout << root->external->getMass() << " " << root->depth << std::endl;
            else if (root->internal.size()) {
                for (int i = 0; i < root->internal.size(); i++){
                    if (root->internal[i]) {
                        traverseTree(root->internal[i]);
                    }
                }
            }
        }
};
