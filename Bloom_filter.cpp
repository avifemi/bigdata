#include <iostream>
#include <vector>
#include <functional>
#include <string>

class BloomFilter 
{
private:
    size_t bitSize;
    std::vector<bool> bits;
    std::vector<std::function<size_t(const std::string&)>> hashFunctions;
    size_t toIndex(size_t hash) const 
    {
        return hash % bitSize;
    }

public:
    BloomFilter(size_t bitSize,
                const std::vector<std::function<size_t(const std::string&)>>& funcs)
        : bitSize(bitSize), bits(bitSize, false), hashFunctions(funcs) {}

    void add(const std::string& value) 
    {
        for (const auto& hashFunc : hashFunctions) 
        {
            size_t index = toIndex(hashFunc(value));
            bits[index] = true;
        }
    }

    bool possiblyContains(const std::string& value) const 
    {
        for (const auto& hashFunc : hashFunctions) 
        {
            size_t index = toIndex(hashFunc(value));
            if (!bits[index]) 
            {
                return false;
            }
        }
        return true;
    }

    void debugPrint() const 
    {
        for (size_t i = 0; i < bitSize; i++) 
        {
            std::cout << bits[i];
        }
        std::cout << "\n";
    }
};

size_t hash1(const std::string& s) 
{
    size_t h = 0;
    for (char c : s) 
    {
        h = h * 31 + c;
    }
    return h;
}

size_t hash2(const std::string& s) 
{
    size_t h = 0;
    for (char c : s) 
    {
        h = h * 131 + c;
    }
    return h;
}

size_t hash3(const std::string& s) 
{
    return std::hash<std::string>{}(s);
}

int main() 
{
    std::vector<std::function<size_t(const std::string&)>> funcs = 
    {
        hash1, hash2, hash3
    };
    BloomFilter bf(1000, funcs);
    bf.add("apple");
    bf.add("banana");
    bf.add("orange");
    std::cout << std::boolalpha;
    std::cout << "apple  -> " << bf.possiblyContains("apple") << "\n";
    std::cout << "banana -> " << bf.possiblyContains("banana") << "\n";
    std::cout << "grape  -> " << bf.possiblyContains("grape") << "\n";

    return 0;
}
