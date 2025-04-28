#pragma once

#include <list>
#include <mutex>
#include <slamd/net/connection.hpp>

namespace slamd {
namespace _net {

class ClientSet {
   public:
    void add(std::shared_ptr<Connection> conn);
    void broadcast(std::vector<uint8_t>&& msg);

   private:
    void clean();
    std::mutex client_mutex;
    std::list<std::shared_ptr<Connection>> clients;
};

}  // namespace _net
}  // namespace slamd