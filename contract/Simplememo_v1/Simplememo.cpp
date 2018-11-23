#include <stdio.h>
#include <string>
#include <vector>
#include <eosiolib/eosio.hpp>

using namespace eosio;

class Simplememo1 : public contract
{
public:
  // constructor
  Simplememo1(account_name s) : contract(s), _memos(s, s), _memos2(s, s) // ※4
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
    print("001");
    require_auth(_self);
    print("002");
    for (auto &item : _memos)
    {
      print("003");
      _memos.emplace(get_self(), [&](auto &p) {
        print("004");
        _memos2.emplace(get_self(), [&](auto &c) {
          print("005");
          c.key = p.key;
          c.name = p.name;
          c.content = p.content;
        });
      });
    }
    print("007");
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
  struct memov3 
  {
    uint64_t key;
    account_name name;
    std::string content;
    uint16_t count = 0;

    uint64_t primary_key() const { return key; }
  };
  typedef eosio::multi_index<N(memov1), memov1> memos; 
  typedef eosio::multi_index<N(memov3), memov3> memos2; 

  memos _memos; 
  memos2 _memos2; 
};

EOSIO_ABI(Simplememo1, (addmemo)(deletememo)(pluspoint)(minuspoint)(merge))