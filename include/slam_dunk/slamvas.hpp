#include <slam_dunk/tree.hpp>
#include <thread>

namespace sdunk {

class Slamvas {
   public:
    Slamvas();
    ~Slamvas();

   private:
    void render_job();
    bool should_stop;

    std::thread render_thread;
    Tree tree;
};

}  // namespace sdunk