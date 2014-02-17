#ifndef k_way_merge_h__
#define k_way_merge_h__
#include <queue>
#include <memory>
#include <vector>
#include "chunk_reader.h"
#include "file_operations.h"


class qelement
{
public:
    wtype value_;
    chunk_reader * reader_;

    bool operator<(const qelement & right) const 
    {
        return value_ > right.value_;
    }
};

class k_way_merge
{
private:
    std::priority_queue<qelement> q_;

public:
    int run(
        const std::string & path_out,
        size_t count_chunks,
        size_t num_elements_in_memory
        )
    {
        std::vector<std::shared_ptr<chunk_reader>> cr;

        std::fstream fout(path_out.c_str(), std::ios::binary | std::ios::out);

        for (size_t i = 0; i < count_chunks; ++i)
        {
            std::string path_out_current = get_tmp_file_name( path_out, i); /// им€ временного файла
            std::shared_ptr<chunk_reader> cur_reader_ptr(new chunk_reader(path_out_current.c_str(), num_elements_in_memory));
            cr.push_back(cur_reader_ptr);

        }

        for (size_t i = 0; i < count_chunks; ++i) /// заполн€ем кучу
        {
            qelement qe;
            qe.reader_ = cr[i].get();
            if( qe.reader_->get_next(qe.value_) )
                q_.push(qe);
        }

        while (!q_.empty())
        {
            /// здесь конечно, хорошо было бы не удал€ть-вставл€ть элемент из кучи, 
            /// а делать изменение его приоритета. Ќо пока оставим, как проще.
            qelement qe = q_.top(); 
            q_.pop();
            fout.write(reinterpret_cast<const char*>(&qe.value_), sizeof(qe.value_)); /// будем наде€тьс€ на хорошую буферизацию операционной системы - не будем делать дополнительный буффер дл€ вывода
            if (qe.reader_->get_next(qe.value_))
                q_.push(qe);

        }
        return 0;
    }


};

#endif // k_way_merge_h__
