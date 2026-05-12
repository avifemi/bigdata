#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <climits>
using namespace std;

class CountMinSketch 
{
private:
    int depth;
    int width;
    vector<vector<int>> table;
    vector<function<int(const string&, int)>> hashFunctions;

public:
    CountMinSketch(
        int depth,
        int width,
        vector<function<int(const string&, int)>> hashes
    ) {
        this->depth = depth;
        this->width = width;
        this->hashFunctions = hashes;
        table.assign(depth, vector<int>(width, 0));
    }

    void add(const string& key, int count = 1) 
    {
        for (int i = 0; i < depth; i++) 
        {
            int idx = hashFunctionsi;
            table[i][idx] += count;
        }
    }

    int estimate(const string& key) 
    {
        int minVal = INT_MAX;
        for (int i = 0; i < depth; i++) 
        {
            int idx = hashFunctionsi;
            minVal = min(minVal, table[i][idx]);
        }
        return minVal;
    }

    void print() 
    {
        cout << "\nCount-Min Sketch:\n";
        for (int i = 0; i < depth; i++) 
        {
            cout << "Row " << i << ": ";
            for (int j = 0; j < width; j++) 
            {
                cout << table[i][j] << " ";
            }
            cout << endl;
        }
    }
};

int hash1(const string& key, int width) 
{
    int h = 0;
    for (char c : key) h = (h * 31 + c) % width;
    return h;
}

int hash2(const string& key, int width) 
{
    int h = 0;
    for (char c : key) h = (h * 17 + c * 7) % width;
    return h;
}

int hash3(const string& key, int width) 
{
    int h = 5381;
    for (char c : key) h = ((h << 5) + h + c) % width;
    return h;
}

int main() 
{
    vector<function<int(const string&, int)>> hashes = 
    {
        hash1, hash2, hash3
    };
    CountMinSketch cms(3, 20, hashes);
    cms.add("apple");
    cms.add("apple");
    cms.add("banana", 3);
    cms.add("orange", 2);
    cout << "apple count ≈ " << cms.estimate("apple") << endl;
    cout << "banana count ≈ " << cms.estimate("banana") << endl;
    cout << "orange count ≈ " << cms.estimate("orange") << endl;
    cout << "grape count ≈ " << cms.estimate("grape") << endl;
    cms.print();

    return 0;
}
