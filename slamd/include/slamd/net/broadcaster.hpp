#pragma once
#include <memory>
#include <mutex>
#include <slamd/net/client_set.hpp>
#include <slamd/net/weak_object_counter.hpp>

namespace slamd {
namespace _net {

class Broadcaster {
   public:
    Broadcaster();

    void add(std::shared_ptr<ClientSet> client_set);
    void remove(std::shared_ptr<ClientSet> client_set);

    void broadcast(std::vector<uint8_t>&& msg);

   private:
    std::mutex client_sets_mutex;
    WeakObjectCounter<ClientSet> client_sets;
};

}  // namespace _net
}  // namespace slamd