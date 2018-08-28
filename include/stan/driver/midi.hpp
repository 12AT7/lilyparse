#include <type_safe/strong_typedef.hpp>

namespace midi {
struct pitch : ts::strong_typedef<pitch, std::uint8_t>
{
    using ts::strong_typedef<pitch, std::uint8_t>::strong_typedef;
};
} // namespace midi
