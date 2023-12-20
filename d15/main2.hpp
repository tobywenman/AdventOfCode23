#include <list>
#include <array>
#include <string>
#include <tuple>

class hashMap
{
    public:
        void add(std::string str, uint8_t val);
        void sub(std::string str);
        unsigned sum();
    private:
        uint8_t hash(std::string str);
        std::array<std::list<std::tuple<std::string, uint8_t>>, 256> data;
};