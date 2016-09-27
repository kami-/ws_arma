#include "id_generator.h"

namespace wsa {
namespace id_generator {

namespace {
    std::atomic<uint32_t> id = 1000;
}

    uint32_t nextId() {
        return ++id;
    };

} // namespace id_generator
} // namespace wsa
