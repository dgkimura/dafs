#pragma once

namespace dafs
{
    template <typename T>
    std::string serialize(const T& obj);


    template <typename T>
    T deserialize(std::string obj);
}
