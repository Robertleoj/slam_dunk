#include <thread>

namespace sdunk {

class Renderer {
   public:
    Renderer();
    ~Renderer();

   private:
    void render_job();
    bool should_stop;

    std::thread render_thread;
};

}  // namespace sdunk