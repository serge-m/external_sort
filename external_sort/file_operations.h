#ifndef file_operations_h__
#define file_operations_h__
#include <string>

std::string get_tmp_file_name(const std::string & path, size_t index);

void delete_temporary_files(const std::string &path_out, size_t count_chunks);
int show_binary_file(const std::string &path);
void generate_random_file(unsigned int n, const std::string &path);
#endif // file_operations_h__
