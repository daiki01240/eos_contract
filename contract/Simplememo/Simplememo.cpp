 #include <stdio.h>
#include <string>
#include <vector>
#include <eosiolib/eosio.hpp>

using namespace eosio;

class Simplememo : public contract
{
public:
  // constructor
  Simplememo(account_name s) : contract(s), _memos(s, s), _memos2(s, s), _points(s, s) // ※4
  {
  }

  // @abi action
  void addmemo(account_name username, std::string content)
  {
    // Ensure this action is authorized by the player
    require_auth(username);

    // update the table to include a new memo
    _memos.emplace(get_self(), [&](auto &p) {
      p.key = _memos.available_primary_key();
      p.name = username;
      p.content = content;
    });
  };

  // @abi action
  void deletememo(uint64_t index)
  {
    auto memov1 = _memos.find(index);
    if (memov1 != _memos.end())
    {
      _memos.erase(memov1);
    }
  };

  // @abi action
  void pluspoint(uint64_t index)
  {
    auto memov1 = _memos.find(index);
    if (memov1 != _memos.end())
    {
      _memos.modify(memov1,_self,[&](auto& a){
        a.count++;
      });
    }
  };

  // @abi action
  void minuspoint(uint64_t index)
  {
    auto memov1 = _memos.find(index);
    if (memov1 != _memos.end())
    {
      _memos.modify(memov1, _self, [&](auto &a) {
        a.count--;
      });
    }
  };

  // @abi action
  void merge()
  {
    require_auth(_self);
    for (auto &item : _memos)
    {
      _memos2.emplace(get_self(), [&](auto &c) {
        c.key = item.key;
        c.name = item.name;
        c.content = item.content;
      });
    }
  };

private:
  // @abi table
  struct memov1
  {
    uint64_t key;
    account_name name;
    std::string content;
    uint16_t count = 0;

    uint64_t primary_key() const { return key; }
  };

  // @abi table
  struct memov2
  {
    uint64_t key;
    account_name name;
    std::string content;
    uint16_t count = 0;

    uint64_t primary_key() const { return key; }
  };

  // @abi table
  struct point
  {
    uint64_t key;
    uint64_t memo_id;
    account_name name;

    uint64_t primary_key() const { return key; }
  };

  typedef eosio::multi_index<N(memov1), memov1> memos; 
  typedef eosio::multi_index<N(memov2), memov2> memos2;
  typedef eosio::multi_index<N(point), point> points;

  memos _memos; 
  memos2 _memos2; 
  points _points;
};

EOSIO_ABI(Simplememo, (addmemo)(deletememo)(pluspoint)(minuspoint)(merge)) 
