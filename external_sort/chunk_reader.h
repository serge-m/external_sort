#ifndef chunk_reader_h__
#define chunk_reader_h__

#include <fstream>
#include <algorithm> 
#include "types.h"


class chunk_reader
{
private:
    std::ifstream fin_;         /// хендлер входного файла
    std::vector<wtype> vec_;    /// вектор-кэш. размер вектора будет соответсвовать максимальному размеру кэша. фактический размер будем хранить в len_
    size_t len_;                /// количество элементов в кэше. 
    size_t idx_;                /// последнего непрочитанного элемента кэша
    size_t max_cache_size_;        /// максимальный размер (количество элементов в кэше)

public:
    chunk_reader(const char * path, size_t memory_size)
        : fin_(path, std::ios::binary)
        , vec_(0)
        , max_cache_size_(std::max<size_t>( memory_size, 1u ))
    {
        if (fin_.fail())
        {
            throw std::exception();
        }

        vec_.resize(memory_size);
        len_ = 0;
        idx_ = 0;
    }

    /// тут нужно отдельно подумать, как делать копирование, и нужно ли. Поэтому просто закроем эти функции
    chunk_reader(const chunk_reader&) = delete;
    chunk_reader& operator=(const chunk_reader&) = delete;


    /// получаем следующий элемент. Читаем из временного файла в память пачками
    /// если все хорошо, то будем возвращать true
    /// если файл, кончился, возвратим false
    bool get_next(wtype &result)
    {
        if (idx_ >= len_)
        {
            vec_.resize(max_cache_size_);
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
