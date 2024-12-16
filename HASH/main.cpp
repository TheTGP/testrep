#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <functional>
#include <random>
#include <algorithm>
#include <chrono>

template <typename K, typename V>

class HashTable {
private:
    struct HashNode {
        K key;
        V value;
        HashNode(K k, V v) : key(k), value(v) {}
    };

    std::vector<std::list<HashNode>> table;
    int capacity;
    int size;

    int hashFunction(const K& key) const {
        std::hash<K> hashFn;
        return hashFn(key) % capacity;
    }

public:
    HashTable(int cap = 101) : capacity(cap), size(0) {
        table.resize(capacity);
    }

    void insert(const K& key, const V& value) {
        int index = hashFunction(key);
        for (auto& node : table[index]) {
            if (node.key == key) {
                node.value = value; // Обновление значения, если ключ существует
                return;
            }
        }
        table[index].emplace_back(key, value);
        size++;
    }

    bool remove(const K& key) {
        int index = hashFunction(key);
        auto& chain = table[index];
        for (auto it = chain.begin(); it != chain.end(); ++it) {
            if (it->key == key) {
                chain.erase(it);
                size--;
                return true;
            }
        }
        return false;
    }

    bool search(const K& key, V& value) const {
        int index = hashFunction(key);
        for (const auto& node : table[index]) {
            if (node.key == key) {
                value = node.value;
                return true;
            }
        }
        return false;
    }

    int getSize() const {
        return size;
    }

    int getCapacity() const {
        return capacity;
    }

    int getCollisionCount() const {
        int collisions = 0;
        for (const auto& chain : table) {
            if (chain.size() > 1) {
                collisions += (chain.size() - 1);
            }
        }
        return collisions;
    }
};



class CustomHash {
public:
    static int hashFunction1(const std::string& key, int capacity) {
        //int hash = 0;
        const int p = 31;//основание
        const int m = 1e9+9;//модуль, чтобы не допустить переполнение

        long long hash;
        //for (char ch : key) {
        //    hash += static_cast<int>(ch);
        //}
        long long pPow = 1;//p^0
        for (char ch : key){
            hash = (hash + (ch - 'a' + 1)*pPow)%m;
            pPow = (pPow * p)%m;
        }
        return hash % capacity;
    }

    static int hashFunction2(const std::string& key, int capacity) {

        unsigned long hash = 5381; // Начальное значение
        for (char ch : key) {
            hash = ((hash << 5) + hash) + ch; // hash * 33 + ch
        }
        return hash % capacity;

        /*
        int hash = 0;
        int p = 31; // Простое число
        for (char ch : key) {
            hash = hash * p + static_cast<int>(ch);
        }
        return hash % capacity;*/
    }

    static int hashFunction3(const std::string& key, int capacity) {
        int hash = 0;
        for (char ch : key) {
            hash = (hash << 5) ^ (hash >> 27) ^ static_cast<int>(ch);
        }
        return hash % capacity;
    }
};




void testHashFunctions(int numElements) {
    HashTable<std::string, int> table1(101);
    HashTable<std::string, int> table2(101);
    HashTable<std::string, int> table3(101);
    HashTable<std::string, int> table4(101);

    std::vector<std::string> randomData;
    std::vector<std::string> realData = {"Аня", "Маша", "Наташа", "Сергей", "Андрей", "Тимофей", "Марианна", "Мария", "Анна", "Юлия"};
    std::vector<std::string> orderedData;

    // Генерация случайных данных
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0, 25);
    for (int i = 0; i < numElements; ++i) {
        std::string randomStr = "";
        for (int j = 0; j < 5; ++j) {
            char ch = 'a' + distribution(generator);
            randomStr += ch;
        }
        randomData.push_back(randomStr);
    }

    // Упорядоченные данные
    for (int i = 0; i < numElements; ++i) {
        orderedData.push_back("key" + std::to_string(i));
    }

    // Тестирование случайных данных
    for (const auto& key : randomData) {
        //table1.insert(key, 1);
        //table2.insert(key, 1);
        table3.insert(key, 1);
    }
    std::cout << "Случайные данные:" << std::endl;
    std::cout << "Коллизии хеш-функции 1: " << table1.getCollisionCount() << std::endl;
    std::cout << "Коллизии хеш-функции 2: " << table2.getCollisionCount() << std::endl;
    std::cout << "Коллизии хеш-функции 3: " << table3.getCollisionCount() << std::endl;

    // Тестирование реальных данных
    for (const auto& key : realData) {
        table1.insert(key, 1);
        table2.insert(key, 1);
        table3.insert(key, 1);
    }
    std::cout << "Реальные данные:" << std::endl;
    std::cout << "Коллизии хеш-функции 1: " << table1.getCollisionCount() << std::endl;
    std::cout << "Коллизии хеш-функции 2: " << table2.getCollisionCount() << std::endl;
    std::cout << "Коллизии хеш-функции 3: " << table3.getCollisionCount() << std::endl;

    // Тестирование упорядоченных данных
    for (const auto& key : orderedData) {
        table1.insert(key, 1);
        table2.insert(key, 1);
        table3.insert(key, 1);
    }
    std::cout << "Упорядоченные данные:" << std::endl;
    std::cout << "Коллизии хеш-функции 1: " << table1.getCollisionCount() << std::endl;
    std::cout << "Коллизии хеш-функции 2: " << table2.getCollisionCount() << std::endl;
    std::cout << "Коллизии хеш-функции 3: " << table3.getCollisionCount() << std::endl;
};

int main() {
    setlocale(LC_ALL, "rus");

    const int numElements = 1000; // Количество элементов для тестирования
    testHashFunctions(numElements);
    return 0;
}


int polynomialHash(const std::string& key) {
    const int p = 31; // Основание
    const int m = 1e9 + 9; // Модуль для предотвращения переполнения
    long long hashValue = 0;
    long long pPow = 1; // p^0

    for (char ch : key) {
        hashValue = (hashValue + (ch - 'a' + 1) * pPow) % m;
        pPow = (pPow * p) % m; // Увеличиваем степень p
    }

    return hashValue;
}

int bitwiseHash(const std::string& key) {
    unsigned long hash = 5381; // Начальное значение
    for (char ch : key) {
        hash = ((hash << 5) + hash) + ch; // hash * 33 + ch
    }
    return hash;
}

int primeHash(const std::string& key) {
    const int prime = 31; // Простое число
    long long hashValue = 0;

    for (size_t i = 0; i < key.size(); ++i) {
        hashValue += (key[i] * prime) % 1000000007; // Модуль для предотвращения переполнения
    }

    return hashValue;
}

