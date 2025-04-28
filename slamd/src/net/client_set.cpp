#include <slamd/net/client_set.hpp>

namespace slamd {
namespace _net {

void ClientSet::add(
    std::shared_ptr<Connection> conn
) {
    this->clean();

    std::scoped_lock l(this->client_mutex);
    this->clients.push_back(conn);
}

void ClientSet::clean() {
    std::scoped_lock l(this->client_mutex);

    for (auto it = this->clients.begin(); it != this->clients.end();) {
        if (!(*it)->is_alive()) {
            it = this->clients.erase(it);
        }
    }
}

void ClientSet::broadcast(
    std::vector<uint8_t>&& msg
) {
    auto msg_ptr = std::make_shared<std::vector<uint8_t>>(std::move(msg));

    std::scoped_lock l(this->client_mutex);

    for (auto it = this->clients.begin(); it != this->clients.end();) {
        if (!(*it)->is_alive()) {
            it = this->clients.erase(it);
        } else {
            (*it)->write(msg_ptr);
        }
    }
}

}  // namespace _net
}  // namespace slamd