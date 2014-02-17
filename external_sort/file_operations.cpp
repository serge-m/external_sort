#include "file_operations.h"
#include <iostream>
#include <random>
#include <fstream>
#include "types.h"
#include <iterator>
using namespace std;

const int error_return_value = 1;


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


/// выводим на экран содержимое бинарного файла
int show_binary_file(const string &path)
{
    fstream f(path.c_str(), ios::binary | ios::in);
    if (f.fail())
    {
        cerr << "Unable to open file '" << path << "' for reading" << endl;
        return error_return_value;
    }
    const size_t chunk_size = 512;
    std::vector<wtype> a(chunk_size);

    while (f)
    {
        f.read(reinterpret_cast<char*>(&a[0]), sizeof(wtype)* a.size());
        size_t num_read = static_cast<size_t>(f.gcount()) / sizeof(wtype);

        std::ostream_iterator<wtype> out_it(std::cout, "\n");
        std::copy(a.begin(), a.begin() + num_read, out_it);

    }

    return 0;
}

/// генерируем бинарный файл из целых
void generate_random_file(unsigned int n, const string &path)
{
    fstream f(path.c_str(), ios::binary | ios::out);
    std::default_random_engine e{ n }; // seed n
    std::uniform_int_distribution<int> d{ 0, std::numeric_limits<int>::max() };
    for (unsigned int i = 0; i < n; ++i)
    {
        wtype number = d(e);
        //cout << number << endl;
        f.write(reinterpret_cast<const char*>(&number), sizeof(number));
    }
    f.close();
}
