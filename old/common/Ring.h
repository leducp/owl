#ifndef OWL_RING_H
#define OWL_RING_H

#include <stdint.h>

namespace owl
{
    template<typename T, uint32_t N>
    class Ring
    {
        static_assert((N > 0) and ((N & (N - 1)) == 0), "N shall be a power of two");

    public:
        Ring()
            : head_{0}
            , tail_{0}
        {
            reset(); 
        }

        ~Ring() = default;

        uint32_t capacity() const   { return N;                     }
        uint32_t size() const       { return head_ - tail_;         }
        uint32_t available() const  { return capacity() - size();   }
        bool isFull() const         { return size() == capacity();  } 
        bool isEmpty() const        { return size() == 0;           }

        void reset()
        {
            head_ = 0;
            tail_ = 0;
        }

        bool push(T const& item)
        {
            if (isFull())
            {
                return false;
            }

            uint32_t index = head_ & (N - 1);
            data_[index] = item;
            ++head_;

            return true;
        }

        bool pop(T& item)
        {
            if (isEmpty())
            {
                return false;
            }

            uint32_t index = tail_ & (N - 1);
            item = data_[index];
            ++tail_;

            return true;
        }


    private:
        uint32_t head_;
        uint32_t tail_;
        T data_[N];
    };
}

#endif
