#include <thread>

namespace slamd {

template <typename T>
class ThreadBox {
   public:
    ThreadBox(
        T data
    )
        : data(data) {
        this->thread_id = std::this_thread::get_id();
    }

    T* get() {
        if (std::this_thread::get_id() != this->thread_id) {
            throw std::runtime_error(
                "Tried to access thread box outside of owning thread"
            );
        }

        return &this->data;
    }

   private:
    T data;
    std::thread::id thread_id;
};

}  // namespace slamd