#include <vector>
#include <cmath>
#include "TreeNode.cpp"

class BarnesHut{
    private:
        TreeNode* root;

    public:
        BarnesHut(TreeNode node){
            root = node;
        }

        void insert(TreeNode node){
            root.insertNode(node);
        }
}