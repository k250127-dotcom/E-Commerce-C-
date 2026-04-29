#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>

namespace ECommerce {

template<typename T>
class FileManager {
public:
    static bool saveToFile(const std::vector<T>& data, const std::string& filename) {
        std::ofstream out(filename, std::ios::binary);
        if (!out) throw std::runtime_error("Cannot open file for writing: " + filename);
        size_t size = data.size();
        out.write(reinterpret_cast<const char*>(&size), sizeof(size));
        for (const auto& item : data) {
            item.serialize(out);   // requires T to have serialize method
        }
        return out.good();
    }

    static std::vector<T> loadFromFile(const std::string& filename) {
        std::ifstream in(filename, std::ios::binary);
        if (!in) throw std::runtime_error("Cannot open file for reading: " + filename);
        size_t size;
        in.read(reinterpret_cast<char*>(&size), sizeof(size));
        std::vector<T> data;
        for (size_t i = 0; i < size; ++i) {
            T item;
            item.deserialize(in);
            data.push_back(std::move(item));
        }
        return data;
    }
};

} // namespace ECommerce

#endif