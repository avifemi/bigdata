#include <iostream>
#include <vector>
#include <list>
#include <string>
using namespace std;

class HashTable 
{
private:
    vector<list<pair<string, int>>> table;
    int size;
    int (*hashFunc)(const string&, int);
public:
    HashTable(int size, int (*func)(const string&, int)) 
    {
        this->size = size;
        this->hashFunc = func;
        table.resize(size);
    }
    void insert(const string& key, int value) 
    {
        int idx = hashFunc(key, size);
        for (auto &p : table[idx]) 
        {
            if (p.first == key) 
            {
                p.second = value;
                return;
            }
        }
        table[idx].push_back({key, value});
    }
    bool search(const string& key, int &result) 
    {
        int idx = hashFunc(key, size);
        for (auto &p : table[idx]) 
        {
            if (p.first == key) 
            {
                result = p.second;
                return true;
            }
        }
        return false;
    }
    
    void remove(const string& key) 
    {
        int idx = hashFunc(key, size);
        for (auto it = table[idx].begin(); it != table[idx].end(); ++it) 
        {
            if (it->first == key) 
            {
                table[idx].erase(it);
                return;
            }
        }
    }
    
    void print() 
    {
        cout << "\nHash Table state:\n";
        for (int i = 0; i < size; i++) 
        {
            cout << i << ": ";
            for (auto &p : table[i]) 
            {
                cout << "(" << p.first << ", " << p.second << ") ";
            }
            cout << endl;
        }
    }
};

int simpleHash(const string& key, int size) 
{
    int hash = 0;
    for (char c : key)
    {
        hash = (hash * 31 + c) % size;
    }
    return hash;
}

int main() 
{
    HashTable ht(10, simpleHash);
    ht.insert("apple", 10);
    ht.insert("banana", 20);
    ht.insert("orange", 30);
    ht.insert("grape", 40);
    ht.insert("melon", 50);
    int result;

    if (ht.search("banana", result)) 
    {
        cout << "banana = " << result << endl;
    } 
    else 
    {
        cout << "banana not found" << endl;
    }
    ht.remove("banana");
    if (!ht.search("banana", result)) 
    {
        cout << "banana successfully removed" << endl;
    }
    ht.insert("apple", 999);
    ht.print();

    return 0;
}
