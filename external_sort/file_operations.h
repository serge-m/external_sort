#ifndef file_operations_h__
#define file_operations_h__
#include <string>

std::string get_tmp_file_name(const std::string & path, size_t index)
{
    return path + std::to_string(index);
}

void delete_temporary_files(const std::string &path_out, size_t count_chunks)
{
    for (size_t i = 0; i < count_chunks; ++i)
    {
        std::string path_cur = get_tmp_file_name(path_out, i);
        int ret_code = std::remove(path_cur.c_str());
        if (ret_code != 0)
        {
            std::cerr << "Error during file '" << path_cur << "' deletion: " << ret_code << '\n';
        }
    }

}

#endif // file_operations_h__
