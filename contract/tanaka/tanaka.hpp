#include <string>
#include <stdio.h>
#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosio.token/eosio.token.hpp>
#include <eosio.system/eosio.system.hpp>

using namespace std;
using namespace eosio;
class tanaka : public eosio::contract
{
  private:
    //@abi table datas
    struct data
    {
        uint64_t key;
        string data1;
        uint64_t data2;

        auto primary_key() const { return key; }
    };
    typedef eosio::multi_index<N(datas), data> datas_table;

    datas_table _datas;

  public:
    tanaka(account_name self) : contract(self), _datas(self, self) {}

    void test(string data1, uint64_t data2);
};