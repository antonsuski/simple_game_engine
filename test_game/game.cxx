#include <iostream>
#include <string_view>

#include "engine.hxx"

int main(int /*argc*/, char* /*argv*/[])
{
    std::string_view lol{ "lol" };
    std::cout << lol << std::endl;
    return 0;
}
