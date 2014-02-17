#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>  

#include <algorithm>
#include "types.h"
#include "k_way_merge.h"
#include "file_operations.h"
using namespace std;


const double percent_in_ram = 0.1;                  /// ���� ������ �� �����, ������� ����� ��������� � ������ 
const double percent_in_ram_for_merge = percent_in_ram; 


const int error_return_value = 1;

void print_usage()
{
    cout
        << "Usage: " << endl
        << "    " << "external_sort gen <length> <file name>  -- generate binary file with n random integers" << endl
        << "    " << "external_sort show <file name>  -- read and outputs binary file with random integers" << endl
        << "    " << "external_sort sort <input file name> <output file name> -- sort integers from input " << endl
        ;
}


/// ��������� ����� �����, ������� ����� ����������� � ������
int sort_chunks(fstream & fin,
    const string path_out,
    const size_t n,
    const size_t chunk_size_internal_sort,
    size_t & count_chunks)
{
    std::vector<wtype> a(chunk_size_internal_sort);
    count_chunks = 0;
    for (size_t i = 0; i < n; i += chunk_size_internal_sort, ++count_chunks)
    {
        fin.read(reinterpret_cast<char*>(&a[0]), sizeof(wtype)* a.size());
        size_t num_read = static_cast<size_t>(fin.gcount()) / sizeof(wtype);
        std::sort(a.begin(), a.begin() + num_read);

        string path_out_current = get_tmp_file_name(path_out, count_chunks); /// ��� ���������� �����
        fstream fout(path_out_current.c_str(), ios::binary | ios::out);
        fout.write(reinterpret_cast<const char *>(&a[0]), num_read * sizeof(wtype));
    }
    return 0;
}


/// ���������� ������������� �����
int merge_chunks(const string & path_out, const size_t count_chunks, const size_t chunk_size_internal_sort)
{
    try
    {
        run_k_way_merge(path_out, count_chunks, chunk_size_internal_sort);
    }
    catch (...) /// ��� ������ ���� ������������ ��������� �� ������
    {
        cerr << "Sorting failed" << endl;
        return error_return_value;
    }
    return 0;
}




int external_sort(const string path_in, const string path_out)
{
    size_t n = 0;

    fstream fin(path_in.c_str(), ios::binary | ios::in);
    if (!fin)
    {
        cerr << "Unable to open input file '" << path_in << "'" << endl;
        return error_return_value;
    }
    // get length of file:
    fin.seekg(0, fin.end);
    std::streamoff length = fin.tellg();
    fin.seekg(0, fin.beg);
    
    if (length <= 0)
    {
        cerr << "Unable to read input file '" << path_in << "'" << endl;
        return error_return_value;
    }

    size_t ulength = static_cast<size_t>(length);

    n = ulength / sizeof(wtype);

    if (ulength != n * sizeof(wtype)) // �� ������
    {
        cerr << "Size of file is not multiple of integer size" << endl;
        return error_return_value;
    }

    size_t chunk_size_internal_sort = static_cast<size_t>(n * percent_in_ram); /// ������ �����, ������� �� ����� ����������� �� ���������� ������

    size_t count_chunks = 0;
    
    sort_chunks(fin, path_out, n, chunk_size_internal_sort, count_chunks );

    merge_chunks(path_out, count_chunks, static_cast<size_t>(chunk_size_internal_sort * percent_in_ram_for_merge)); /// ������ ����� �� ������� ����� ��������� ��-�������

    delete_temporary_files(path_out, count_chunks);


    return 0;
}

int main( int argc, char *argv[] )
{
    string mode;
    string arg2;
    string arg3;
    string str_n;

    if (argc > 1) mode = string(argv[1]);
    if (argc > 2) arg2 = string(argv[2]);
    if (argc > 3) arg3 = string(argv[3]);


    /*cout << "mode = " << mode << " file = " << arg2;
    cout << " file2 = " << arg3;
    cout << endl;*/

    if (mode == "gen")
    {
        int n = 0;
        try
        {
            n = std::stoi(arg2);
        }
        catch (const invalid_argument & )
        {
            cerr << "Unable to convert parameter '" << arg2 << "' to a number";
            return error_return_value;
        }

        if (arg3 == "")
        {
            cerr << "Output file name was not specified" << endl;
            print_usage();
            return error_return_value;
        }
        generate_random_file(n, arg3);
    }
    else if (mode == "show")
    {
        if (arg2 == "")
        {
            cerr << "Output file name was not specified" << endl;
            print_usage();
            return error_return_value;
        }

        show_binary_file(arg2);
    }
    else if (mode == "sort")
    {
        if (arg2 == "" || arg3 == "")
        {
            cerr << "Input/output file names were not specified" << endl;
            print_usage();
            return error_return_value;
        }
        return external_sort(arg2, arg3);
    }
    else
    {
        cerr << "Unsupported mode '" << mode << "'" << endl;
        print_usage();
        return error_return_value;
    }

    return 0;
}