#ifndef AVLTREE_H
#define AVLTREE_H

#include <string>
#include <vector>
#include <optional>
#include <iostream>
#include <algorithm>

class AVLTree {
public:
    using KeyType   = std::string; // Keys stored in the tree
    using ValueType = size_t;      // Values stored in the tree

private:
    // This class represents a single node in the AVL tree.
    class AVLNode {
    public:
        KeyType   key;    // Key for this node
        ValueType value;  // Value associated with the key
        size_t height;    // Height of this node in the tree

        AVLNode* left;    // Pointer to left child
        AVLNode* right;   // Pointer to right child

        // Constructor: new node starts(height = 1)
        AVLNode(const KeyType& k, ValueType v)
            : key(k), value(v), height(1), left(nullptr), right(nullptr) {}

        // Returns number of children this node has
        size_t numChildren() const {
            size_t c = 0;
            if (left)  ++c;
            if (right) ++c;
            return c;
        }

        // Returns true if the node has no children
        bool isLeaf() const {
            return !left && !right;
        }

        // Recalculates this node's height from its children.
        // Height = 1 + max(height(left), height(right))
        void updateHeight() {
            size_t leftH  = left  ? left->height  : 0;
            size_t rightH = right ? right->height : 0;
            height = 1 + std::max(leftH, rightH);
        }

        // Returns the balance factor = height(left) - height(right)
        // For an AVL tree this should stay in {-1, 0, +1}
        int getBalance() const {
            int leftH  = left  ? static_cast<int>(left->height)  : 0;
            int rightH = right ? static_cast<int>(right->height) : 0;
            return leftH - rightH;
        }
    };

    // Pointer to the root node of the AVL tree (nullptr if empty)
    AVLNode* root;

    // Number of key-value pairs stored in the tree
    size_t treeSize;
    // insert : inserts (key, value) into subtree rooted at 'node'.
    // Returns true if a new node is inserted, false if the key already exists.
    bool insert(AVLNode*& node, const KeyType& key, ValueType value);

    //  remove : removes 'key' from subtree rooted at 'node'.
    // Returns true if a node was removed, false if key not found.
    bool remove(AVLNode*& node, const KeyType& key);

    // Handles the node deletion logic.
    bool removeNode(AVLNode*& node);

    //  contains : returns true if key exists in subtree rooted at 'node'.
    bool contains(const AVLNode* node, const KeyType& key) const;
    // Returns nullptr if not found.
    AVLNode* getNode(AVLNode* node, const KeyType& key) const;

    //  adds all VALUES whose keys are in [lowKey, highKey] to 'result'.
    void findRange(const AVLNode* node,
                   const KeyType& lowKey,
                   const KeyType& highKey,
                   std::vector<ValueType>& result) const;

    // getkeys performs in-order traversal and pushes all keys into 'result'.
    void getKeys(const AVLNode* node,
                 std::vector<KeyType>& result) const;

    // Creates a  deep copy of the subtree rooted at 'other' and returns the new root.
    AVLNode* copyTree(const AVLNode* other);

    //  deletes all nodes in subtree rooted at 'node'.
    void clearTree(AVLNode* node);

    // Finds the node with the smallest key in subtree rooted at 'node'.
    AVLNode* findMin(AVLNode* node) const;

    // Rotations used to restore AVL balance:
    void rotateLeft(AVLNode*& node);
    void rotateRight(AVLNode*& node);

    // Checks balance factor of 'node' and performs the necessary rotation.
    void balanceNode(AVLNode*& node);

    //  print to draw the tree sideways into an output stream.
    void printTree(std::ostream& os,
                   const AVLNode* node,
                   size_t depth = 0) const;

public:
    //  constructor: creates an empty AVL tree.
    AVLTree();

    // Copy constructor: creates a  copy of 'other'.
    AVLTree(const AVLTree& other);

    // Assignment operator: clears current tree, then copies 'other'.
    AVLTree& operator=(const AVLTree& other);

    // Destructor: frees all dynamically allocated nodes.
    ~AVLTree();

    // Inserts (key, value) into the tree.
    // Returns true if a new node is inserted, false if key already exists.
    bool insert(const KeyType& key, ValueType value);

    // Removes the given key from the tree.
    // Returns true if a node was removed, false if key not found.
    bool remove(const KeyType& key);

    // Returns true if the key exists in the tree; false otherwise.
    bool contains(const KeyType& key) const;

    // Returns the value associated with 'key' inside std::optional.
    // If not found, returns std::nullopt.
    std::optional<ValueType> get(const KeyType& key) const;

    //  operator: returns reference to value for 'key'.
    //
    ValueType& operator[](const KeyType& key);

    // Returns a vector of all VALUES whose keys lie between [lowKey, highKey].
    std::vector<ValueType> findRange(const KeyType& lowKey,
                                     const KeyType& highKey) const;

    // Returns a vector of all keys currently stored in the tree, in sorted order.
    std::vector<KeyType> keys() const;

    // Returns how many key-value pairs are in the tree (O(1)).
    size_t size() const;

    // Returns the height of the tree (height of the root).
    // Empty tree has height 0.
    size_t getHeight() const;

    // printing using: std::cout << tree;
    friend std::ostream& operator<<(std::ostream& os,
                                    const AVLTree& tree);
};

#endif
