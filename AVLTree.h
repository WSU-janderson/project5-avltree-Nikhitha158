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
    using KeyType   = std::string; // Keys are strings
    using ValueType = size_t;      // Values are size_t

protected:
    // Nested node class used internally by AVLTree.

    class AVLNode {
    public:
        KeyType   key;    // key associated with this node
        ValueType value;  // value associated with this key
        size_t    height; // height of this node (1 for a leaf)

        AVLNode* left;    // pointer to left child
        AVLNode* right;   // pointer to right child

        // Constructor initializes a new leaf node with given key/value
        AVLNode(const KeyType& k, ValueType v)
            : key(k), value(v), height(1), left(nullptr), right(nullptr) {}

        // Returns how many children this node has
        size_t numChildren() const {
            size_t count = 0;
            if (left)  ++count;
            if (right) ++count;
            return count;
        }

        // Returns true if this node has no children
        bool isLeaf() const {
            return !left && !right;
        }

        // Returns the stored height for this node
        size_t getHeight() const {
            return height;
        }

        /* Recomputes this node's height based on its children.
          Height of a node = 1 + max(height(left), height(right)).
          Height of a null child is considered 0.   */
        void updateHeight() {
            size_t leftHeight  = left  ? left->height  : 0;
            size_t rightHeight = right ? right->height : 0;
            height = 1 + std::max(leftHeight, rightHeight);
        }

        /* Returns the balance factor of this node:
         balance = height(left) - height(right). For an AVL tree, balance should always be -1, 0, or +1.
         value outside this range means the subtree is unbalanced.
         */
        int getBalance() const {
            size_t leftHeight  = left  ? left->height  : 0;
            size_t rightHeight = right ? right->height : 0;
            return static_cast<int>(leftHeight) - static_cast<int>(rightHeight);
        }
    };

    // Pointer to the root node of the AVL tree (nullptr if tree is empty)
    AVLNode* root;

    // Number of key-value pairs currently stored in the tree
    size_t   treeSize;
    /* Inserts (key, value) into the subtree rooted at 'node'. Returns true if a new node was inserted, false if key already exists.
     Rebalances the subtree and updates heights on the way back up.
     */
    bool insert(AVLNode*& node, const KeyType& key, ValueType value);

    /* Removes the node with the given key from the subtree rooted at 'node'.
     Returns true if the key was found and removed, false otherwise. Rebalances the subtree and updates heights on the way back up.
     */
    bool remove(AVLNode*& node, const KeyType& key);

    /* Recursive contains helper.
    Returns true if 'key' exists in the subtree rooted at 'node'.
     */
    bool contains(const AVLNode* node, const KeyType& key) const;

    //To find and return the node with the given key. Returns nullptr if the key is not found.
    AVLNode* getNode(AVLNode* node, const KeyType& key) const;

    // findRange() : pushes all VALUES whose keys  lie in [lowKey, highKey] into 'result'.
    void findRange(const AVLNode* node,
                   const KeyType& lowKey,
                   const KeyType& highKey,
                   vector<ValueType>& result) const;

    // getkeys(): pushes all keys into 'result'.

    void getKeys(const AVLNode* node, vector<KeyType>& result) const;

    // copy constructor and assignment: copies the subtree rooted at 'otherNode' and returns the root of the copied subtree.

    AVLNode* copyTree(const AVLNode* otherNode);

    // destructor and assignment : Deletes all nodes in the subtree rooted at 'node'.

    void clearTree(AVLNode* node);

    /* Prints the tree sideways into 'os' corresponding to 'depth'.
     * Right subtree is printed first so the tree is rotated.
     */
    void printTree(ostream& os,
                   const AVLNode* node,
                   size_t depth = 0) const;

    //Performs a left rotation around 'node'.

    void rotateLeft(AVLNode*& node);

    // Performs a right rotation around 'node'.


    void rotateRight(AVLNode*& node);

    // Checks the balance factor of 'node' and performs the appropriate single or double rotation to restore AVL balance.

    void balanceNode(AVLNode*& node);
    bool removeNode(AVLNode*& node);
    // Finds the node with the minimum key in the subtree rooted at 'node'.
    AVLNode* findMin(AVLNode* node) const;

public:
    //  constructor: creates an empty AVL tree
    AVLTree();

    // Copy constructor: creates a  copy of 'other'
    AVLTree(const AVLTree& other);

    // Copy assignment operator: clears current tree and  copies 'other'
    AVLTree& operator=(const AVLTree& other);

    // Destructor: releases all dynamically allocated nodes
    ~AVLTree();
    /* Inserts a new key-value pair into the tree.
      If key does not exist: creates a new node and returns true.
      If key already exists: does nothing and returns false.
     After insertion, the tree is rebalanced if necessary.
     */
    bool insert(const KeyType& key, ValueType value);

    //Removes the node with the given key from the tree.
    bool remove(const KeyType& key);

    /* Returns true if the given key exists in the tree, false otherwise.
     * Time complexity: O(log n) for a balanced AVL tree.
     */
    bool contains(const KeyType& key) const;

    /* Returns the value associated with 'key' if it exists,
      If the key is not in the tree, returns std::nullopt.
     */
    std::optional<ValueType> get(const KeyType& key) const;

    /* Returns a reference to the value associated with 'key'.
     If the key exists: returns a reference to its value.
     If the key does not exist: inserts it with default value 0, then returns a reference to the new value.
     */
    ValueType& operator[](const KeyType& key);

    /*Returns a vector of VALUES whose keys are in the inclusive range [lowKey, highKey].
     If no keys are in that range, the returned vector is empty. */
    vector<ValueType> findRange(const KeyType& lowKey,
                                const KeyType& highKey) const;

    /* Returns a vector containing all keys currently stored in the tree.
      Keys are added using an in-order traversal, so they will be sorted.
     */
    vector<KeyType> keys() const;

    /* Returns the number of key-value pairs stored in the tree.
      Time complexity: O(1).*/
    size_t size() const;

    /*Returns the height of the tree (height of the root node).
     * Height of an empty tree is 0.
     * Time complexity: O(1) because each node stores its height.
     */
    size_t getHeight() const;
    friend ostream& operator<<(ostream& os, const AVLTree& tree);
};

#endif // AVLTREE_H
