#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>  
#include <random>
using namespace std;

typedef int wtype;

void print_usage()
{
    cout
        << "Usage: " << endl
        << "    " << "external_sort gen <length> <file name>  -- generate binary file with n random integers" << endl
        << "    " << "external_sort show <file name>  -- read and outputs binary file with random integers" << endl
        << "    " << "external_sort sort <input file name> <output file name> -- sort integers from input " << endl
        ;
}

void show_binary_file(string path)
{
    fstream f(path.c_str(), ios::binary | ios::in);
    const size_t chunk_size = 10;
    std::vector<wtype> a(chunk_size);

    while (f)
    {
        f.read(reinterpret_cast<char*>(&a[0]), sizeof(wtype)* a.size());
        size_t num_read = f.gcount() / sizeof(wtype);
        
        std::ostream_iterator<wtype> out_it(std::cout, "\n");
        std::copy(a.begin(), a.begin() + num_read, out_it);

    }

    /*std::copy(
        std::istreambuf_iterator<int>(f),
        std::istreambuf_iterator<int>(),
        std::ostreambuf_iterator<int>(cout));
        f.close();*/
}

void generate_random_file(unsigned int n, string path)
{
    fstream f(path.c_str(), ios::binary | ios::out);
    std::default_random_engine e{n}; // seed n
    std::uniform_int_distribution<int> d{ 0, std::numeric_limits<int>::max() };
    for (unsigned int i = 0; i < n; ++i)
    {
        wtype number = d(e);
        cout << number << endl;
        f.write( reinterpret_cast<const char*>(&number), sizeof(number));
    }
    f.close();
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


    cout << "mode = " << mode << " file = " << arg2;
    cout << " file2 = " << arg3;
    cout << endl;

    if (mode == "gen")
    {
        int n = 0;
        try
        {
            n = std::stoi(arg2);
        }
        catch (const invalid_argument & )
        {
            cout << "Unable to convert parameter '" << arg2 << "' to a number";
            return 1;
        }

        if (arg3 == "")
        {
            cout << "Output file name was not specified" << endl;
            print_usage();
            return 1;
        }
        generate_random_file(n, arg3);
    }
    else if (mode == "show")
    {
        if (arg2 == "")
        {
            cout << "Output file name was not specified" << endl;
            print_usage();
            return 1;
        }

        show_binary_file(arg2);
    }
    else
    {
        cout << "Unsupported mode '" << mode << "'" << endl;
        print_usage();
        return 1;
    }

    return 0;
}