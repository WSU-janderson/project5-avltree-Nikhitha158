/* Name : Nikhitha Palakurla
Project-5 : Map ADT: AVL Tree
This project implements a AVL tree that serves as a map data type. A map allows you to store and retrieve key-value pairs.
AVL tree remains balanced by implementing single and double and triple rotations when inserting and removing data.
 */


#include "AVLTree.h"

// Default constructor: start with an empty tree.
AVLTree::AVLTree() : root(nullptr), treeSize(0) {}

// Copy constructor: deep-copy the other tree.
AVLTree::AVLTree(const AVLTree& other) : root(nullptr), treeSize(0) {
    root = copyTree(other.root);      // copy the whole structure
    treeSize = other.treeSize;        // copy the size
}

// Assignment operator: clear current tree, then deep-copy other.
AVLTree& AVLTree::operator=(const AVLTree& other) {
    if (this != &other) {
        clearTree(root);              // delete current tree
        root = copyTree(other.root);  // copies other's tree
        treeSize = other.treeSize;
    }
    return *this;
}

// Destructor: free all nodes.
AVLTree::~AVLTree() {
    clearTree(root);
}

//  insert: inserts (key, value) starting from root.
bool AVLTree::insert(const KeyType& key, ValueType value) {
    bool inserted = insert(root, key, value);
    if (inserted) {
        ++treeSize;                            // count new nodes
    }
    return inserted;
}

bool AVLTree::insert(AVLNode*& node, const KeyType& key, ValueType value) {
    // if an empty place is found then it creates a new node.
    if (!node) {
        node = new AVLNode(key, value);
        return true;
    }

    // Decides whether to go to the left or right subtree:
    if (key < node->key) {
        // Insert into the left subtree.
        if (!insert(node->left, key, value)) {
            return false;
        }
    } else if (key > node->key) {
        // Insert into the right subtree.
        if (!insert(node->right, key, value)) {
            return false;
        }
    } else {
        // key == node->key
        return false;
    }

    //  update height and rebalance.
    node->updateHeight();
    balanceNode(node);
    return true;
}


//  remove: remove given key from the tree, starting from root.
bool AVLTree::remove(const KeyType& key) {
    bool removed = remove(root, key);
    if (removed) {
        --treeSize;                   // decrease count if something is removed
    }
    return removed;
}

bool AVLTree::remove(AVLNode*& node, const KeyType& key) {
    if (!node) {
        return false;                 // key not found
    }

    bool removed;

    if (key < node->key) {
        // Search and remove in left subtree.
        removed = remove(node->left, key);
    } else if (key > node->key) {
        // Search and remove in right subtree.
        removed = remove(node->right, key);
    } else {
        // node->key == key → this is the node to delete.
        removed = removeNode(node);
    }

    // If after removal the node still exists, update its height and rebalance.
    if (node) {
        node->updateHeight();
        balanceNode(node);
    }
    return removed;
}

bool AVLTree::removeNode(AVLNode*& node) {
    if (!node) {
        return false;
    }

    AVLNode* old = node;
    size_t children = node->numChildren();

    if (children == 0) {
        // Case 1: leaf node → just delete it.
        node = nullptr;
        delete old;
    }
    else if (children == 1) {
        // Case 2: one child → replace node with its single child.
        node = (node->left ? node->left : node->right);
        delete old;
    }
    else {
        // Case 3: two children.
        // Find inorder successor (smallest in right subtree).
        AVLNode* succ = findMin(node->right);

        // Copy successor's key/value into current node.
        node->key = succ->key;
        node->value = succ->value;

        // Now delete the successor node from the right subtree.
        return remove(node->right, succ->key);
    }
    return true;
}

// Finds node with smallest key in a subtree (left-most node).
AVLTree::AVLNode* AVLTree::findMin(AVLNode* node) const {
    if (!node) {
        return nullptr;
    }
    while (node->left) {
        node = node->left;
    }
    return node;
}


// contains: tells if 'key' is in the tree.
bool AVLTree::contains(const KeyType& key) const {
    return contains(root, key);
}

//  contains: checks subtree rooted at 'node'.
bool AVLTree::contains(const AVLNode* node, const KeyType& key) const {
    if (!node) {
        return false;
    }
    if (key < node->key) {
        return contains(node->left, key);
    }
    if (key > node->key) {
        return contains(node->right, key);
    }
    return true;
}

// Returns the value for 'key' . If key not found, returns std::nullopt.
std::optional<AVLTree::ValueType>
AVLTree::get(const KeyType& key) const {
    AVLNode* found = getNode(root, key);
    return found ? std::optional<ValueType>(found->value) : std::nullopt;
}

// finds and returns the node pointer for 'key' in subtree.
AVLTree::AVLNode*
AVLTree::getNode(AVLNode* node, const KeyType& key) const {
    if (!node) {
        return nullptr;
    }
    if (key < node->key) {
        return getNode(node->left, key);
    }
    if (key > node->key) {
        return getNode(node->right, key);
    }
    return node;
}


// returns reference to value for 'key'.
AVLTree::ValueType&
AVLTree::operator[](const KeyType& key) {
    AVLNode* node = getNode(root, key);
    //if (!node) {
        // Key not found → insert with default value 0.
       // insert(key, 0);
     //   node = getNode(root, key);
 //   }
    return node->value;
}

// Returns vector of VALUES whose keys lie between [lowKey, highKey] (inclusive).
std::vector<AVLTree::ValueType>
AVLTree::findRange(const KeyType& lowKey,
                   const KeyType& highKey) const {
    std::vector<ValueType> out;
    findRange(root, lowKey, highKey, out);
    return out;
}

void AVLTree::findRange(const AVLNode* node,
                        const KeyType& lowKey,
                        const KeyType& highKey,
                        std::vector<ValueType>& result) const {
    if (!node) {
        return;
    }

    // If node's key is greater than lowKey,  left subtree.
    if (node->key > lowKey) {
        findRange(node->left, lowKey, highKey, result);
    }

    // If node's key lies within [lowKey, highKey], include its value.
    if (node->key >= lowKey && node->key <= highKey) {
        result.push_back(node->value);
    }

    // If node's key is less than highKey,  right subtree.
    if (node->key < highKey) {
        findRange(node->right, lowKey, highKey, result);
    }
}

// Returns all keys currently stored, in sorted order
std::vector<AVLTree::KeyType>
AVLTree::keys() const {
    std::vector<KeyType> result;
    getKeys(root, result);
    return result;
}

void AVLTree::getKeys(const AVLNode* node,
                      std::vector<KeyType>& result) const {
    if (!node) {
        return;
    }
    getKeys(node->left, result);
    result.push_back(node->key);
    getKeys(node->right, result);
}

// Returns how many nodes are in the tree.
size_t AVLTree::size() const {
    return treeSize;
}

// Returns height of the tree (height of root, or 0 if empty).
size_t AVLTree::getHeight() const {
    return root ? root->height : 0;
}
// Left rotation around 'node'
void AVLTree::rotateLeft(AVLNode*& node) {
    AVLNode* newRoot = node->right;   // right child becomes new root of this subtree

    node->right = newRoot->left;      // move newRoot's left subtree over
    newRoot->left = node;             // current node becomes left child

    node->updateHeight();             // update heights after  change
    newRoot->updateHeight();

    node = newRoot;                   // update the reference to point to new root
}

// Right rotation around 'node'.
void AVLTree::rotateRight(AVLNode*& node) {
    AVLNode* newRoot = node->left;    // left child becomes new root

    node->left = newRoot->right;      // move newRoot's right subtree over
    newRoot->right = node;            // current node becomes right child

    node->updateHeight();
    newRoot->updateHeight();

    node = newRoot;
}

// Checks node's balance factor and performs necessary  rotation.
void AVLTree::balanceNode(AVLNode*& node) {
    if (!node) {
        return;
    }

    int balance = node->getBalance(); // positive → left-heavy, negative → right-heavy

    // Left-heavy subtree
    if (balance > 1) {
        // If left child is right-heavy, do a left rotation on the child first.
        if (node->left && node->left->getBalance() < 0) {
            rotateLeft(node->left);
        }
        // Then right-rotate the node.
        rotateRight(node);
    }
    // Right-heavy subtree
    else if (balance < -1) {
        // If right child is left-heavy, do a right rotation on the child first .
        if (node->right && node->right->getBalance() > 0) {
            rotateRight(node->right);
        }
        // Then left-rotate the node.
        rotateLeft(node);
    }
}

// copies the subtree rooted at 'other' and returns new root.
AVLTree::AVLNode*
AVLTree::copyTree(const AVLNode* other) {
    if (!other) {
        return nullptr;
    }

    AVLNode* n = new AVLNode(other->key, other->value);
    n->height = other->height;          // copy stored height
    n->left  = copyTree(other->left);   // copy left subtree
    n->right = copyTree(other->right);  // copy right subtree
    return n;
}

//  deletes all nodes in subtree rooted at 'node'.
void AVLTree::clearTree(AVLNode* node) {
    if (!node) {
        return;
    }
    clearTree(node->left);
    clearTree(node->right);
    delete node;
}

std::ostream& operator<<(std::ostream& os, const AVLTree& tree) {
    tree.printTree(os, tree.root);
    return os;
}

void AVLTree::printTree(std::ostream& os,
                        const AVLNode* node,
                        size_t depth) const {
    if (!node) {
        return;
    }

    // Print right subtree
    printTree(os, node->right, depth + 1);

    // Indent based on depth level to show tree structure.
    for (size_t i = 0; i < depth; ++i) {
        os << "    ";  //
    }

    // Print the current node: key, value, height, and balance factor.
    os << node->key << ":" << node->value
       << " (h:" << node->height
       << ", b:" << node->getBalance()
       << ")\n";

    // Print left subtree.
    printTree(os, node->left, depth + 1);
}
