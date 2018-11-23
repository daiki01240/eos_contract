#include <stdio.h>
#include <string>
#include <eosiolib/eosio.hpp>

using namespace eosio;

class Simplememo : public contract
{
public:
  // constructor
  Simplememo(account_name s) : contract(s), _memos(s, s) // ※4
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
  void readmemo(account_name username)
  {
    std::string content = _memos.get(username).content;
    print(content);
  };

  // @abi action
  void deletememo(uint64_t index)
  {
    auto memo = _memos.find(index);
    if (memo != _memos.end())
    {
      _memos.erase(memo);
    }
  };

  // @abi action
  void pluspoint(uint64_t index)
  {
    auto memo = _memos.find(index);
    if (memo != _memos.end())
    {
      _memos.modify(memo,_self,[&](auto& a){
        a.count++;
      });
    }
  };

  // @abi action
  void minuspoint(uint64_t index)
  {
    auto memo = _memos.find(index);
    if (memo != _memos.end())
    {
      _memos.modify(memo, _self, [&](auto &a) {
        a.count--;
      });
    }
  };

private:
  // @abi table
  struct memo // ※1
  {
    uint64_t key;
    account_name name;
    std::string content;
    uint16_t count = 0;

    uint64_t primary_key() const { return key; }
  };
  typedef eosio::multi_index<N(memo), memo> memos; // ※2

  memos _memos; //※3
};

EOSIO_ABI(Simplememo, (addmemo)(readmemo)(deletememo)(pluspoint)(minuspoint))

