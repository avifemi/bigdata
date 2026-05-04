#include <iostream>
using namespace std;

const int T = 2;

bool cmp(int a, int b)
{
    return a < b;
}

struct Node
{
    int keys[2*T - 1];
    Node* children[2*T];
    int n;
    bool leaf;
};

Node* createNode(bool leaf)
{
    Node* node = new Node;
    node->leaf = leaf;
    node->n = 0;
    for (int i = 0; i < 2*T; i++)
        node->children[i] = nullptr;
    return node;
}

void inorder(Node* root)
{
    if (!root)
        return;

    int i;
    for (i = 0; i < root->n; i++)
    {
        if (!root->leaf)
            inorder(root->children[i]);

        cout << root->keys[i] << " ";
    }

    if (!root->leaf)
        inorder(root->children[i]);
}

bool search(Node* root, int key)
{
    if (!root)
        return false;

    int i = 0;

    while (i < root->n && cmp(root->keys[i], key))
        i++;

    if (i < root->n && root->keys[i] == key)
    {
        cout << "\n" << key << " есть в дереве\n";
        return true;
    }

    if (root->leaf)
    {
        cout << "\n" << key << " нет в дереве\n";
        return false;
    }

    return search(root->children[i], key);
}

void split(Node* parent, int i)
{
    Node* full = parent->children[i];
    Node* newNode = createNode(full->leaf);

    newNode->n = T - 1;

    for (int j = 0; j < T - 1; j++)
        newNode->keys[j] = full->keys[j + T];

    if (!full->leaf)
    {
        for (int j = 0; j < T; j++)
            newNode->children[j] = full->children[j + T];
    }

    full->n = T - 1;

    for (int j = parent->n; j >= i + 1; j--)
        parent->children[j + 1] = parent->children[j];

    parent->children[i + 1] = newNode;

    for (int j = parent->n - 1; j >= i; j--)
        parent->keys[j + 1] = parent->keys[j];

    parent->keys[i] = full->keys[T - 1];
    parent->n++;
}

void insertNonFull(Node* node, int key)
{
    int i = node->n - 1;

    if (node->leaf)
    {
        while (i >= 0 && cmp(key, node->keys[i]))
        {
            node->keys[i + 1] = node->keys[i];
            i--;
        }

        node->keys[i + 1] = key;
        node->n++;
    }
    else
    {
        while (i >= 0 && cmp(key, node->keys[i]))
            i--;

        i++;

        if (node->children[i]->n == 2*T - 1)
        {
            split(node, i);

            if (cmp(node->keys[i], key))
                i++;
        }

        insertNonFull(node->children[i], key);
    }
}

Node* insert(Node* root, int key)
{
    if (root == nullptr)
    {
        root = createNode(true);
        root->keys[0] = key;
        root->n = 1;
        return root;
    }

    if (root->n == 2*T - 1)
    {
        Node* newRoot = createNode(false);
        newRoot->children[0] = root;

        split(newRoot, 0);

        int i = 0;
        if (cmp(newRoot->keys[0], key))
            i++;

        insertNonFull(newRoot->children[i], key);

        return newRoot;
    }
    else
    {
        insertNonFull(root, key);
        return root;
    }
}

int getPred(Node* node, int idx)
{
    Node* cur = node->children[idx];
    while (!cur->leaf)
        cur = cur->children[cur->n];
    return cur->keys[cur->n - 1];
}

int getSucc(Node* node, int idx)
{
    Node* cur = node->children[idx + 1];
    while (!cur->leaf)
        cur = cur->children[0];
    return cur->keys[0];
}

void merge(Node* node, int idx)
{
    Node* child = node->children[idx];
    Node* sibling = node->children[idx + 1];

    child->keys[T - 1] = node->keys[idx];

    for (int i = 0; i < sibling->n; i++)
        child->keys[i + T] = sibling->keys[i];

    if (!child->leaf)
    {
        for (int i = 0; i <= sibling->n; i++)
            child->children[i + T] = sibling->children[i];
    }

    for (int i = idx + 1; i < node->n; i++)
        node->keys[i - 1] = node->keys[i];

    for (int i = idx + 2; i <= node->n; i++)
        node->children[i - 1] = node->children[i];

    child->n += sibling->n + 1;
    node->n--;

    delete sibling;
}

void borrowFromPrev(Node* node, int idx)
{
    Node* child = node->children[idx];
    Node* sibling = node->children[idx - 1];

    for (int i = child->n - 1; i >= 0; i--)
        child->keys[i + 1] = child->keys[i];

    if (!child->leaf)
    {
        for (int i = child->n; i >= 0; i--)
            child->children[i + 1] = child->children[i];
    }

    child->keys[0] = node->keys[idx - 1];

    if (!child->leaf)
        child->children[0] = sibling->children[sibling->n];

    node->keys[idx - 1] = sibling->keys[sibling->n - 1];

    child->n++;
    sibling->n--;
}

void borrowFromNext(Node* node, int idx)
{
    Node* child = node->children[idx];
    Node* sibling = node->children[idx + 1];

    child->keys[child->n] = node->keys[idx];

    if (!child->leaf)
        child->children[child->n + 1] = sibling->children[0];

    node->keys[idx] = sibling->keys[0];

    for (int i = 1; i < sibling->n; i++)
        sibling->keys[i - 1] = sibling->keys[i];

    if (!sibling->leaf)
    {
        for (int i = 1; i <= sibling->n; i++)
            sibling->children[i - 1] = sibling->children[i];
    }

    child->n++;
    sibling->n--;
}

void fill(Node* node, int idx)
{
    if (idx != 0 && node->children[idx - 1]->n >= T)
        borrowFromPrev(node, idx);
    else if (idx != node->n && node->children[idx + 1]->n >= T)
        borrowFromNext(node, idx);
    else
    {
        if (idx != node->n)
            merge(node, idx);
        else
            merge(node, idx - 1);
    }
}

void removeKey(Node*& root, int key)
{
    if (!root)
        return;

    int idx = 0;

    while (idx < root->n && cmp(root->keys[idx], key))
        idx++;

    if (idx < root->n && root->keys[idx] == key)
    {
        if (root->leaf)
        {
            for (int i = idx + 1; i < root->n; i++)
                root->keys[i - 1] = root->keys[i];

            root->n--;
        }
        else
        {
            if (root->children[idx]->n >= T)
            {
                int pred = getPred(root, idx);
                root->keys[idx] = pred;
                removeKey(root->children[idx], pred);
            }
            else if (root->children[idx + 1]->n >= T)
            {
                int succ = getSucc(root, idx);
                root->keys[idx] = succ;
                removeKey(root->children[idx + 1], succ);
            }
            else
            {
                merge(root, idx);
                removeKey(root->children[idx], key);
            }
        }
    }
    else
    {
        if (root->leaf)
            return;

        bool flag = (idx == root->n);

        if (root->children[idx]->n < T)
            fill(root, idx);

        if (flag && idx > root->n)
            removeKey(root->children[idx - 1], key);
        else
            removeKey(root->children[idx], key);
    }

    if (root->n == 0)
    {
        Node* tmp = root;

        if (root->leaf)
            root = nullptr;
        else
            root = root->children[0];

        delete tmp;
    }
}

void deleteTree(Node* root)
{
    if (!root) return;

    if (!root->leaf)
    {
        for (int i = 0; i <= root->n; i++)
            deleteTree(root->children[i]);
    }

    delete root;
}

int main()
{
    Node* root = nullptr;
    root = insert(root, 10);
    root = insert(root, 20);
    root = insert(root, 5);
    root = insert(root, 6);
    root = insert(root, 12);
    inorder(root);
    search(root, 12);
    removeKey(root, 6);
    inorder(root);
    
    deleteTree(root);
    return 0;
}
