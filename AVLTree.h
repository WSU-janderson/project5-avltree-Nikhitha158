/**
 * AVLTree.h
 */

#ifndef AVLTREE_H
#define AVLTREE_H

#include <string>
#include <vector>
#include <optional>
#include <iostream>
#include <algorithm>

using namespace std;

class AVLTree {
public:
    using KeyType = std::string;
    using ValueType = size_t;

protected:
    class AVLNode {
    public:
        KeyType key;
        ValueType value;
        size_t height;

        AVLNode* left;
        AVLNode* right;

        AVLNode(const KeyType& k, ValueType v)
            : key(k), value(v), height(1), left(nullptr), right(nullptr) {}

        // 0, 1 or 2
        size_t numChildren() const {
            size_t count = 0;
            if (left) count++;
            if (right) count++;
            return count;
        }

        // true or false
        bool isLeaf() const {
            return !left && !right;
        }

        // number of hops to deepest leaf node
        size_t getHeight() const {
            return height;
        }

        // Update height based on children's heights
        void updateHeight() {
            size_t leftHeight = left ? left->height : 0;
            size_t rightHeight = right ? right->height : 0;
            height = 1 + std::max(leftHeight, rightHeight);
        }

        // Get balance factor
        int getBalance() const {
            size_t leftHeight = left ? left->height : 0;
            size_t rightHeight = right ? right->height : 0;
            return static_cast<int>(leftHeight) - static_cast<int>(rightHeight);
        }
    };

    AVLNode* root;
    size_t treeSize;

    // Recursive helper methods
    bool insert(AVLNode*& node, const KeyType& key, ValueType value);
    bool remove(AVLNode*& node, const KeyType& key);
    bool contains(const AVLNode* node, const KeyType& key) const; // Fixed: made this the helper
    AVLNode* getNode(AVLNode* node, const KeyType& key) const;
    void findRange(const AVLNode* node, const KeyType& lowKey, const KeyType& highKey,
                   vector<KeyType>& result) const;
    void getKeys(const AVLNode* node, vector<KeyType>& result) const;
    AVLNode* copyTree(const AVLNode* otherNode);
    void clearTree(AVLNode* node);
    void printTree(ostream& os, const AVLNode* node, size_t depth = 0) const;

    // Rotation methods
    void rotateLeft(AVLNode*& node);
    void rotateRight(AVLNode*& node);
    void balanceNode(AVLNode*& node);

    // Remove helper
    bool removeNode(AVLNode*& node);
    AVLNode* findMin(AVLNode* node);

public:
    AVLTree();
    AVLTree(const AVLTree& other);
    AVLTree& operator=(const AVLTree& other);
    ~AVLTree();

    // Public interface
    bool insert(const KeyType& key, ValueType value);
    bool remove(const KeyType& key);
    bool contains(const KeyType& key) const;
    std::optional<ValueType> get(const KeyType& key) const;
    ValueType& operator[](const KeyType& key);
    vector<KeyType> findRange(const KeyType& lowKey, const KeyType& highKey) const;
    vector<KeyType> keys() const;
    size_t size() const;
    size_t getHeight() const;

    // Friend function for output
    friend ostream& operator<<(ostream& os, const AVLTree& tree);
};

#endif // AVLTREE_H