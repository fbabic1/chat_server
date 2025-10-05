#include "lib/utils/message.h"

namespace utils {

std::ostream& operator<<(std::ostream& os, const Message& msg) {
    os << msg.toString();
    return os;
}

} // namespace utils