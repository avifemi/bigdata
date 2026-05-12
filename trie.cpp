#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;

class Trie 
{
private:
    struct Node 
    {
        bool isEnd;
        int value;
        unordered_map<char, Node*> next;
        Node() : isEnd(false), value(0) {}
    };
    Node* root;
    string alphabet;

public:
    Trie(const string& alphabet) 
    {
        this->alphabet = alphabet;
        root = new Node();
    }
    void insert(const string& key, int value) 
    {
        Node* cur = root;
        for (char c : key) 
        {
            if (cur->next.find(c) == cur->next.end()) 
            {
                cur->next[c] = new Node();
            }
            cur = cur->next[c];
        }
        cur->isEnd = true;
        cur->value = value;
    }
    bool search(const string& key, int &result) 
    {
        Node* cur = root;
        for (char c : key) 
        {
            if (cur->next.find(c) == cur->next.end()) 
            {
                return false;
            }
            cur = cur->next[c];
        }
        if (cur->isEnd) 
        {
            result = cur->value;
            return true;
        }
        return false;
    }
    
private:
    bool removeHelper(Node* node, const string& key, int depth) 
    {
        if (!node) return false;
        if (depth == key.size()) 
        {
            if (!node->isEnd) return false;
            node->isEnd = false;
            return node->next.empty();
        }

        char c = key[depth];
        if (node->next.find(c) == node->next.end())
            return false;

        bool shouldDeleteChild = removeHelper(node->next[c], key, depth + 1);
        if (shouldDeleteChild) 
        {
            delete node->next[c];
            node->next.erase(c);

            return !node->isEnd && node->next.empty();
        }
        return false;
    }

public:
    void remove(const string& key) 
    {
        removeHelper(root, key, 0);
    }
};

int main() 
{
    Trie trie("abcdefghijklmnopqrstuvwxyz");
    trie.insert("hello", 1);
    trie.insert("world", 2);
    int result;
    if (trie.search("hello", result)) 
    {
        cout << "hello = " << result << endl;
    }
    trie.remove("hello");
    if (!trie.search("hello", result)) 
    {
        cout << "hello deleted" << endl;
    }
    
    return 0;
}
