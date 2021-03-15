#include "HCTree.hpp"

void HCTree::clearHelper(HCNode* &node) {
    // base case for recursion
    if (node == NULL) {
        return;
    }
    clearHelper(node->c1); // clear left subtree
    clearHelper(node->c0); // clear right subtree
    delete node; // delete node 
    node = NULL; // make sure the node is set to null
}

HCTree::~HCTree() {
    clearHelper(root);
    root = 0;
}

void HCTree::build(const vector<int>& freqs) { 
    // priority queue to store nodes
    std::priority_queue<HCNode*, std::vector<HCNode*>, HCNodePtrComp> pq;
    // Loop thru freqs to create all nodes to add to initial pq
    for (long unsigned int i = 0; i < freqs.size(); i++) {
        if (freqs[i] > 0) {
            // Create new node for byte
            HCNode* node = new HCNode(freqs[i], i, nullptr, nullptr, nullptr);
            // Add new node to pq
            pq.push(node);
            // Add node to leaves vector
            leaves[i] = node;
        }
    }

    // While the pq has more than one node
    while (pq.size() > 1) {
        // Get the top node and then remove it from the pq
        HCNode* curr = pq.top();
        pq.pop();
        // Get the second node and then remove it
        HCNode* next = pq.top();
        pq.pop();
        
        int newCount = curr->count + next->count; // Combined count for new node
        // Default newNode
        HCNode* newNode = new HCNode(newCount, curr->symbol, curr, next, nullptr);
        
        // THE LESSER COUNT/SYMBOL IS ALWAYS C0 AND THE SYMBOL
        // if counts are not the same, separate by count
        if (curr->count != next->count) {
            // if curr > next, next is c0 and the symbol
            if (curr->count > next->count) {
                newNode->symbol = next->symbol;
                newNode->c0 = next;
                newNode->c1 = curr;
            }
        }
        // if counts are the same, separate by symbol 
        else if (curr->count == next->count) {
            // if curr symbol is > next, next is c0 and symbol
            if (curr->symbol > next->symbol) {
                newNode->symbol = next->symbol;
                newNode->c0 = next;
                newNode->c1 = curr;
            }
        }
        
        // Set new parent for curr and next
        curr->p = newNode;
        next->p = newNode;

        // Add the new node back into the pq
        pq.push(newNode);
    }
    // Root is last node in pq
    root = pq.top();
}



void traverseTree(HCNode* node, BitOutputStream& out) {
    if (node == 0) {
        return;
    }

    if (node->p == 0) {
        return;
    }

    traverseTree(node->p, out);

    // go up the tree from the leaves
    // if the leaf is a left child, write 0
    if (node == node->p->c0) {
        out.writeBit(0);
    }
    // if the leaf is a right child, write 1
    else if (node == node->p->c1) {
        out.writeBit(1);
    }
}

void HCTree::encode(byte symbol, BitOutputStream& out) const {
    HCNode* n;
    // Find correct symbol from leaves vector
    for (long unsigned int i = 0; i < leaves.size(); i++) {
        if (leaves[i] != 0) {
            if (leaves[i]->symbol == symbol) {
            n = leaves[i];
            break;
            }
        }
        
    }
    // recursive traversal
    traverseTree(n, out);
}

int HCTree::decode(BitInputStream& in) const {
    int bit;
    // start at root
    HCNode* n = root;

    // Traverse tree until you get to a leaf
    while (n->c0 != nullptr && n->c1 != nullptr) {
        bit = in.readBit(); // get current bit
        // traverse right for 1
        if (bit == 1) {
            // if it exists, go to c1
            if (n->c1 != nullptr) {
                n = n->c1;
            }
            // if it is a leaf child, that is the symbol
            else {
                return n->symbol;
            }
        }
        // traverse left for 0
        else if (bit == 0) {
            // if it exists, go to c0
            if (n->c0 != nullptr) {
                n = n->c0;
            }
            // if it is a leaf child, that is the symbol
            else {
                return n->symbol;
            }
        }
    }
    // Decode using tree
    return n->symbol; // shouldnt really go here
}
