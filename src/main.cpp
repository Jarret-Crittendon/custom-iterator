#include <iostream>
#include "Jarray.hpp"
#include <algorithm>

int main()
{
    Jarray<std::string> sray;
    sray.push_back("Kamiya Nao");
    sray.push_back("Tachibana Arisu");
    sray.push_back("Minase Iori");
    sray.push_back("Futaba An");
    sray.push_back("Hisakawa Hayate");
    sray.push_back("Igarashi Kyouko");
    sray.push_back("Futami Mami");

    std::cout << sray.size() << ", " << sray.capacity() << "\n\n";

    std::sort(sray.begin(), sray.end());
    for (const auto& s : sray) {
        std::cout << s << '\n';
    }
}