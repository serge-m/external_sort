#ifndef file_operations_h__
#define file_operations_h__
#include <string>

std::string get_tmp_file_name(const std::string & path, size_t index)
{
    return path + std::to_string(index);
}


#endif // file_operations_h__
