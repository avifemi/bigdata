#include <iostream>
#include <functional>
using namespace std;

template<typename K, typename V>
class BTreeNode
{
public:
    K* keys;
    V* values;
    BTreeNode<K, V>** children;
    int t;
    int n;
    bool leaf;

    BTreeNode(int f, bool leaf1)
    {
        t = f;
        leaf = leaf1;
        keys = new K[2 * t - 1];
        values = new V[2 * t - 1];
        children = new BTreeNode<K, V>*[2 * t];
        n = 0;

        for (int i = 0; i < 2 * t; i++)
            children[i] = nullptr;
    }

    ~BTreeNode()
    {
        delete[] keys;
        delete[] values;
        delete[] children;
    }
};

template<typename K, typename V, typename Compare = less<K>>
class BTree
{
private:
    BTreeNode<K, V>* root;
    int t;
    Compare comp;

private:
    bool equal(const K& a, const K& b)
    {
        return comp(a, b) == false && comp(b, a) == false;
    }

    void inorder(BTreeNode<K, V>* node)
    {
        if (node == nullptr)
            return;

        for (int i = 0; i < node->n; i++)
        {
            if (node->leaf == false)
                inorder(node->children[i]);

            cout << node->keys[i] << ":" << node->values[i] << " ";
        }

        if (node->leaf == false)
            inorder(node->children[node->n]);
    }

    V* search(BTreeNode<K, V>* node, const K& key)
    {
        if (node == nullptr)
            return nullptr;

        int i = 0;

        while (i < node->n && comp(node->keys[i], key))
        {
            i++;
        }

        if (i < node->n && equal(node->keys[i], key))
        {
            return &node->values[i];
        }

        if (node->leaf == true)
            return nullptr;

        return search(node->children[i], key);
    }

public:
    BTree(int f)
    {
        root = nullptr;
        t = f;
    }

    V* search(const K& key)
    {
        return search(root, key);
    }

    void inorder()
    {
        inorder(root);
        cout << endl;
    }
};

int main()
{
    
}
