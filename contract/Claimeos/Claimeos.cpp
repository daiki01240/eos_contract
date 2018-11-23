#include <stdio.h>
#include <string>
#include <vector>
#include <eosiolib/eosio.hpp>
#include <eosiolib/time.hpp>

using namespace eosio;

class Claimeos : public contract
{
public:
  // constructor
  Claimeos(account_name s) : contract(s), _claims(s, s) // ※4
  { 
  }

  //@abi action
  void addhash(std::string ipfshash, account_name publisher)
  {
    require_auth(publisher);

    uint64_t check = 0;

    for (auto &item : _claims)
    {
      if (item.ipfs == ipfshash)
      {
        check++;
      }
    }
    if (check == 0)
    {
      _claims.emplace(get_self(), [&](auto &c) {
        c.key = _claims.available_primary_key();
        c.ipfs = ipfshash;
        c.name = publisher;
        c.time_stamp = eosio::time_point_sec(now());
      });
    }
  }

  //@abi action
  void delhash(std::string ipfshash)
  {
    require_auth(_self);

    std::vector<uint64_t> keysForDeletion;
    for (auto &item : _claims)
    {
      if (item.ipfs == ipfshash)
      {
        keysForDeletion.push_back(item.key);
      }
    }

    for (uint64_t key : keysForDeletion)
    {
      auto itr = _claims.find(key);
      if (itr != _claims.end())
      {
        _claims.erase(itr);
      }
    }
}

private:
  // @abi table
  struct claim // ※1
  {
    uint64_t key;
    std::string ipfs;
    account_name name;
    eosio::time_point_sec time_stamp;

    uint64_t primary_key() const { return key; }
  };
  typedef eosio::multi_index<N(claim), claim> claims; // ※2
  claims _claims; //※3
};

EOSIO_ABI(Claimeos, (addhash)(delhash))