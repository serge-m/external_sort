#ifndef k_way_merge_h__
#define k_way_merge_h__
#include <queue>
#include <memory>
#include <vector>
#include "chunk_reader.h"
#include "file_operations.h"


class qelement /// Элемент кучи. содержит значение, по которому идет сравнение,  и кэш, из которого читается слещующее значение
{
public:
    wtype value_;
    chunk_reader * reader_;

    bool operator<(const qelement & right) const 
    {
        return value_ > right.value_;
    }
};

int run_k_way_merge(
    const std::string & path_out,
    size_t count_chunks,
    size_t num_elements_in_memory
    )
{
    std::vector<std::shared_ptr<chunk_reader>> cached_readers; 
    std::priority_queue<qelement> q_;

    std::fstream fout(path_out.c_str(), std::ios::binary | std::ios::out);

    /// создеем кэш для каждого временного файла. Освобождение памяти возложим на shared_ptr
    for (size_t i = 0; i < count_chunks; ++i) 
    {
        std::string path_out_current = get_tmp_file_name( path_out, i); /// имя временного файла
        std::shared_ptr<chunk_reader> cur_reader_ptr(new chunk_reader(path_out_current.c_str(), num_elements_in_memory)); 
        cached_readers.push_back(cur_reader_ptr);
    }

    /// заполняем кучу
    /// читаем по одному элементу из каждого кэша
    for (size_t i = 0; i < count_chunks; ++i) 
    {
        qelement qe;
        qe.reader_ = cached_readers[i].get(); 
        if( qe.reader_->get_next(qe.value_) )
            q_.push(qe);
    }

    while (!q_.empty())
    {
        /// здесь конечно, хорошо было бы не удалять-вставлять элемент из кучи, 
        /// а делать изменение его приоритета. Но пока оставим, как проще.
        qelement qe = q_.top(); 
        q_.pop();

        /// будем надеяться на хорошую буферизацию операционной системы - не будем делать дополнительный буффер для вывода
        fout.write(reinterpret_cast<const char*>(&qe.value_), sizeof(qe.value_)); 

        /// если еще есть элементы в файле, читаем их и добавляем в кучу
        if (qe.reader_->get_next(qe.value_))
            q_.push(qe);

    }
    return 0;
}


#endif // k_way_merge_h__
