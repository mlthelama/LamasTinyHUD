#pragma once

namespace util {
    class string_util {
    public:
        template <typename T>
        static std::string int_to_hex(T a_i) {
            std::stringstream stream;
            stream << "0x" << std::setfill('0') << std::setw(sizeof(T) * 2) << std::hex << a_i;

            return stream.str();
        }
    };
}
