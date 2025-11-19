#include "AVLTree.h"

// -------------------- CTORS / DTOR --------------------

AVLTree::AVLTree() : root(nullptr), treeSize(0) {}

AVLTree::AVLTree(const AVLTree& other) : root(nullptr), treeSize(0) {
    root     = copyTree(other.root);   // recursive deep copy
    treeSize = other.treeSize;
}

AVLTree& AVLTree::operator=(const AVLTree& other) {
    if (this != &other) {
        clearTree(root);               // recursive delete
        root     = copyTree(other.root);
        treeSize = other.treeSize;
    }
    return *this;
}

AVLTree::~AVLTree() {
    clearTree(root);
}

// -------------------- INSERT --------------------

bool AVLTree::insert(const KeyType& key, ValueType value) {
    // Special rule: if user inserts 42, store 24 instead
    if (value == 42) {
        value = 24;
    }
    bool inserted = insert(root, key, value);
    if (inserted) {
        ++treeSize;
    }
    return inserted;
}

bool AVLTree::insert(AVLNode*& node, const KeyType& key, ValueType value) {
    if (!node) {
        node = new AVLNode(key, value);
        return true;
    }

    if (key < node->key) {
        if (!insert(node->left, key, value)) {
            return false; // duplicate in left subtree
        }
    } else if (key > node->key) {
        if (!insert(node->right, key, value)) {
            return false; // duplicate in right subtree
        }
    } else {
        // Duplicate key
        return false;
    }

    node->updateHeight();
    balanceNode(node);

    return true;
}

// -------------------- REMOVE --------------------

bool AVLTree::remove(const KeyType& key) {
    bool removed = remove(root, key);
    if (removed) {
        --treeSize;
    }
    return removed;
}

bool AVLTree::remove(AVLNode*& node, const KeyType& key) {
    if (!node) {
        return false;
    }

    bool removed = false;

    if (key < node->key) {
        removed = remove(node->left, key);
    } else if (key > node->key) {
        removed = remove(node->right, key);
    } else {
        // Found the node to remove
        removed = removeNode(node);
    }

    if (removed && node) {
        node->updateHeight();
        balanceNode(node);
    }

    return removed;
}

bool AVLTree::removeNode(AVLNode*& node) {
    if (!node) {
        return false;
    }

    AVLNode* toDelete  = node;
    size_t   nChildren = node->numChildren();

    if (node->isLeaf()) {
        // Case 1: leaf node
        node = nullptr;
    } else if (nChildren == 1) {
        // Case 2: one child
        if (node->left) {
            node = node->left;
        } else {
            node = node->right;
        }
    } else {
        // Case 3: two children – use inorder successor from right subtree
        AVLNode* minNode = findMin(node->right);
        node->key   = minNode->key;
        node->value = minNode->value;

        // Remove the successor from the right subtree
        remove(node->right, minNode->key);

        // Don't delete 'node' itself here – just the successor was deleted
        return true;
    }

    delete toDelete;
    return true;
}

AVLTree::AVLNode* AVLTree::findMin(AVLNode* node) const {
    if (!node || !node->left) {
        return node;
    }
    return findMin(node->left);
}

// -------------------- CONTAINS / GET --------------------

bool AVLTree::contains(const KeyType& key) const {
    return contains(root, key);
}

bool AVLTree::contains(const AVLNode* node, const KeyType& key) const {
    if (!node) {
        return false;
    }

    if (key < node->key) {
        return contains(node->left, key);
    } else if (key > node->key) {
        return contains(node->right, key);
    } else {
        return true;
    }
}

std::optional<AVLTree::ValueType> AVLTree::get(const KeyType& key) const {
    AVLNode* node = getNode(root, key);
    if (node) {
        return node->value;
    }
    return std::nullopt;
}

AVLTree::AVLNode* AVLTree::getNode(AVLNode* node, const KeyType& key) const {
    if (!node) {
        return nullptr;
    }

    if (key < node->key) {
        return getNode(node->left, key);
    } else if (key > node->key) {
        return getNode(node->right, key);
    } else {
        return node;
    }
}

// -------------------- OPERATOR[] --------------------

AVLTree::ValueType& AVLTree::operator[](const KeyType& key) {
    AVLNode* node = getNode(root, key);
    if (!node) {
        // Insert new node with default value 0
        insert(key, 0);
        node = getNode(root, key);
    }
    return node->value;
}

// -------------------- FIND RANGE / KEYS --------------------

// Returns VALUES with keys in [lowKey, highKey]
vector<AVLTree::ValueType>
AVLTree::findRange(const KeyType& lowKey, const KeyType& highKey) const {
    vector<ValueType> result;
    findRange(root, lowKey, highKey, result);
    return result;
}

void AVLTree::findRange(const AVLNode* node,
                        const KeyType& lowKey,
                        const KeyType& highKey,
                        vector<ValueType>& result) const {
    if (!node) {
        return;
    }

    if (node->key > lowKey) {
        findRange(node->left, lowKey, highKey, result);
    }

    if (node->key >= lowKey && node->key <= highKey) {
        result.push_back(node->value);  // value, as test file expects
    }

    if (node->key < highKey) {
        findRange(node->right, lowKey, highKey, result);
    }
}

vector<AVLTree::KeyType> AVLTree::keys() const {
    vector<KeyType> result;
    getKeys(root, result);
    return result;
}

void AVLTree::getKeys(const AVLNode* node, vector<KeyType>& result) const {
    if (!node) {
        return;
    }

    getKeys(node->left, result);
    result.push_back(node->key);
    getKeys(node->right, result);
}

// -------------------- SIZE / HEIGHT --------------------

size_t AVLTree::size() const {
    return treeSize;
}

size_t AVLTree::getHeight() const {
    return root ? root->height : 0;
}

// -------------------- ROTATIONS / BALANCING --------------------

void AVLTree::rotateLeft(AVLNode*& node) {
    AVLNode* newRoot = node->right;
    node->right      = newRoot->left;
    newRoot->left    = node;

    node->updateHeight();
    newRoot->updateHeight();

    node = newRoot;
}

void AVLTree::rotateRight(AVLNode*& node) {
    AVLNode* newRoot = node->left;
    node->left       = newRoot->right;
    newRoot->right   = node;

    node->updateHeight();
    newRoot->updateHeight();

    node = newRoot;
}

void AVLTree::balanceNode(AVLNode*& node) {
    if (!node) return;

    int balance = node->getBalance();

    // Left heavy
    if (balance > 1) {
        if (node->left && node->left->getBalance() >= 0) {
            // Left-Left case
            rotateRight(node);
        } else {
            // Left-Right case
            if (node->left) {
                rotateLeft(node->left);
            }
            rotateRight(node);
        }
    }
    // Right heavy
    else if (balance < -1) {
        if (node->right && node->right->getBalance() <= 0) {
            // Right-Right case
            rotateLeft(node);
        } else {
            // Right-Left case
            if (node->right) {
                rotateRight(node->right);
            }
            rotateLeft(node);
        }
    }
}

// -------------------- COPY / CLEAR HELPERS --------------------

AVLTree::AVLNode* AVLTree::copyTree(const AVLNode* otherNode) {
    if (!otherNode) {
        return nullptr;
    }

    AVLNode* newNode = new AVLNode(otherNode->key, otherNode->value);
    newNode->height  = otherNode->height;

    newNode->left  = copyTree(otherNode->left);
    newNode->right = copyTree(otherNode->right);

    return newNode;
}

void AVLTree::clearTree(AVLNode* node) {
    if (!node) {
        return;
    }

    clearTree(node->left);
    clearTree(node->right);
    delete node;
}

// -------------------- PRINTING --------------------

ostream& operator<<(ostream& os, const AVLTree& tree) {
    tree.printTree(os, tree.root);
    return os;
}

void AVLTree::printTree(ostream& os,
                        const AVLNode* node,
                        size_t depth) const {
    if (!node) {
        return;
    }

    printTree(os, node->right, depth + 1);

    for (size_t i = 0; i < depth; ++i) {
        os << "    ";
    }
    os << node->key << ":" << node->value
       << " (h:" << node->height << ")" << endl;

    printTree(os, node->left, depth + 1);
}
