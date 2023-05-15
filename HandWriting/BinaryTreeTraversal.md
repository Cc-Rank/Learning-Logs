# 二叉树的各种遍历

## [前序遍历](https://leetcode.cn/problems/binary-tree-preorder-traversal/)

### 前序递归

```cpp
class Solution {
public:
    vector<int> preorderTraversal(TreeNode* root) {
        vector<int> result;
        function<void(TreeNode*)> preorder = [&](TreeNode* root) {
            if (!root)
                return;
            result.emplace_back(root->val);
            preorder(root->left);
            preorder(root->right);
        };
        preorder(root);
        return result;
    }
};
```

### 前序迭代

```cpp
class Solution {
public:
    vector<int> preorderTraversal(TreeNode* root) {
        vector<int> result;
        stack<TreeNode*> stk;
        TreeNode* node = root;
        while (node || !stk.empty()) {
            while (node) {
                result.emplace_back(node->val);
                stk.emplace(node);
                node = node->left;
            }
            node = stk.top();
            stk.pop();
            node = node->right;
        }
        return result;
    }
};
```

### [前序Morris遍历](https://leetcode.cn/problems/binary-tree-preorder-traversal/solution/er-cha-shu-de-qian-xu-bian-li-by-leetcode-solution/)

```cpp
class Solution {
public:
    vector<int> preorderTraversal(TreeNode* root) {
        vector<int> result;
        TreeNode* node = root, *predecessor = nullptr;
        while (node)
        {
            predecessor = node->left;
            if (predecessor)
            {
                while (predecessor->right && predecessor->right != node)
                    predecessor = predecessor->right;
                if (!predecessor->right)
                {
                    result.emplace_back(node->val);
                    predecessor->right = node;
                    node = node->left;
                }
                else
                {
                    predecessor->right = nullptr;
                    node = node->right;
                }
            }
            else
            {
                result.emplace_back(node->val);
                node = node->right;
            }
        }
        return result;
    }
};
```

## [中序遍历](https://leetcode.cn/problems/binary-tree-inorder-traversal/)

### 中序递归

```cpp
class Solution {
public:
    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> result;
        function<void(TreeNode*)> inOrder = [&](TreeNode* node) {
            if (!node)
                return;
            inOrder(node->left);
            result.emplace_back(node->val);
            inOrder(node->right);
        };
        inOrder(root);
        return result;
    }
};
```

### 中序迭代

```cpp
class Solution {
public:
    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> result;
        stack<TreeNode*> stk;
        TreeNode* node = root;
        while (node || !stk.empty()) {
            while (node) {
                stk.push(node);
                node = node->left;
            }
            node = stk.top();
            stk.pop();
            result.emplace_back(node->val);
            node = node->right;
        }
        return result;
    }
};
```

### 中序 Morris 遍历

```cpp
class Solution {
public:
    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> result;
        TreeNode *node = root, *predecessor = nullptr;

        while (node) {
            if (node->left) {
                predecessor = node->left;
                while (predecessor->right && predecessor->right != node)
                    predecessor = predecessor->right;
                if (!predecessor->right) {
                    predecessor->right = node;
                    node = node->left;
                }
                else {
                    result.emplace_back(node->val);
                    predecessor->right = nullptr;
                    node = node->right;
                }
            }
            else {
                result.emplace_back(node->val);
                node = node->right;
            }
        }
        return result;
    }
};
```

## [后序遍历](https://leetcode.cn/problems/binary-tree-postorder-traversal/)

> 后序遍历还有一种方式是从左往右前序遍历二叉树，并将遍历结果倒序输出，即为后序遍历结果。

### 后序递归

```cpp
class Solution {
public:
    vector<int> postorderTraversal(TreeNode* root) {
        vector<int> result;
        function<void(TreeNode*)> postOrder = [&](TreeNode* node) {
            if (!node)
                return;
            postOrder(node->left);
            postOrder(node->right);
            result.emplace_back(node->val);
        };
        postOrder(root);
        return result;
    }
};
```

### 后序迭代

```cpp
class Solution {
public:
    vector<int> postorderTraversal(TreeNode* root) {
        vector<int> result;
        stack<TreeNode*> stk;
        TreeNode* node = root, * prev = nullptr;
        while (node || !stk.empty()) {
            while (node) {
                stk.emplace(node);
                node = node->left;
            }
            node = stk.top();
            if (!node->right || node->right == prev) {
                stk.pop();
                result.emplace_back(node->val);
                prev = node;
                node = nullptr;
            }
            else
                node = node->right;
        }
        return result;
    }
};
```

## [层序遍历](https://leetcode.cn/problems/binary-tree-level-order-traversal/)

```cpp
class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> result;
        if (!root)
            return result;
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            int cnt = q.size();
            result.push_back(vector<int>());
            for (int i = 0; i < cnt; ++i) {
                TreeNode* node = q.front();
                q.pop();
                result.back().emplace_back(node->val);
                if (node->left)
                    q.push(node->left);
                if (node->right)
                    q.push(node->right);
            }
        }
        return result;
    }
};
```
