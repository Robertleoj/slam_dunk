#include <slamd/net/broadcaster.hpp>

namespace slamd {
namespace _net {

Broadcaster::Broadcaster() {}

void Broadcaster::add(
    std::shared_ptr<ClientSet> client_set
) {
    std::scoped_lock l(this->client_sets_mutex);

    this->client_sets.add(client_set);
}

void Broadcaster::broadcast(
    std::vector<uint8_t>&& msg
) {
    std::scoped_lock l(this->client_sets_mutex);

    auto msg_ptr = std::make_shared<std::vector<uint8_t>>(std::move(msg));

    for (auto& client_set : this->client_sets.alive_objects()) {
        client_set->broadcast(msg_ptr);
    }
}

void Broadcaster::remove(
    std::shared_ptr<ClientSet> client_set
) {
    std::scoped_lock l(this->client_sets_mutex);
    this->client_sets.remove(client_set);
}

}  // namespace _net

}  // namespace slamd
