#include "tanaka.hpp"

void tanaka::test(string data1, uint64_t data2)
{
    _datas.emplace(get_self(), [&](auto &s)
    {
        s.key = _datas.available_primary_key();
        s.data1 = data1;
        s.data2 = data2;
    });
};

EOSIO_ABI(tanaka, (test))
