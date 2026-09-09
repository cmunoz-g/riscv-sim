#include "load_file.h"

#include <filesystem>
#include <fstream>

namespace riscvsim
{

std::vector<uint8_t> load_file(const char* path)
{
    auto size{std::filesystem::file_size(path)};
    std::vector<uint8_t> buf(size);
    std::fstream file;
    file.open(path, std::ios::in | std::ios::binary);
    if (!file)
    {
        throw std::runtime_error(std::string(path) + ": could not open");
    }
    file.read(reinterpret_cast<char*>(buf.data()), size);
    file.close();
    return buf;
}

};  // namespace riscvsim
