#ifndef chunk_reader_h__
#define chunk_reader_h__

#include <fstream>
#include "types.h"


class chunk_reader
{
private:
    std::ifstream fin_;
    std::vector<wtype> vec_;
    size_t len_;
    size_t idx_;
    size_t memory_size_;

public:
    chunk_reader(const char * path, size_t memory_size )
        : fin_( path, std::ios::binary )
        , vec_( 0 )
        , memory_size_(memory_size)
    {
        if (fin_.fail())
        {
            throw std::exception();
        }

        //vec_.reserve(memory_size);
        vec_.resize(memory_size);
        len_ = 0;
        idx_ = 0;
        //it_ = vec_.begin();
    }

    bool get_next(wtype &result)
    {
        if (idx_ >= len_)
        {
            vec_.resize(memory_size_);
            fin_.read(reinterpret_cast<char*>(&vec_[0]), sizeof(wtype)* vec_.size());
            size_t num_read = static_cast<size_t>(fin_.gcount()) / sizeof(wtype); // нужна проверка на кратность
            if (num_read == 0)
            {
                return false;
            }
            idx_ = 0;
            len_ = num_read;
        }
        result = vec_[idx_++];
        return true;
    }





};


#endif // chunk_reader_h__
