#include "filemanager.h"
#include "product.h"

namespace ECommerce {

// Explicit instantiation for Product – tells the compiler to generate code
// for FileManager<Product> right here, so other .cpp files don't need to see
// the implementation (they only see the declaration in the header).
template class FileManager<Product>;

// If you later add other types (e.g., User), instantiate them here:
// template class FileManager<User>;

// Optional: non‑template helper functions that are common to all file operations
namespace FileUtils {

    bool fileExists(const std::string& filename) {
        std::ifstream f(filename.c_str());
        return f.good();
    }

    size_t getFileSize(const std::string& filename) {
        std::ifstream f(filename, std::ios::binary | std::ios::ate);
        if (!f) return 0;
        return f.tellg();
    }

} // namespace FileUtils

} // namespace ECommerce