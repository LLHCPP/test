#include "ThreadPool.h"
#include <iostream>
#include<future>
#include<stack>

int myfun(int l) {
    return l + 3;
}

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;

    TreeNode() : val(0), left(nullptr), right(nullptr) {}

    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}

    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class Solution {
public:
    vector<int> inorderTraversal(TreeNode *root) {
        vector<int> ans;
        if (root == nullptr) {
            return ans;
        }
        stack<TreeNode *> MySt;
        while (root || !MySt.empty()) {
            while (root) {
                MySt.push(root);
                root = root->left;
            }
            ans.push_back(MySt.top()->val);
            root = MySt.top()->right;
            MySt.pop();
        }
        return ans;
    }
};

int main() {
//    std::cout << "Hello, World!" << std::endl;
//    return 0;
    ThreadPool pool(4);
    future<int> k = pool.commit(myfun, 2);
    cout << k.get();
}
