#pragma once
#include <thread>

namespace slamd {

void assert_thread(std::thread::id thread_id);

}