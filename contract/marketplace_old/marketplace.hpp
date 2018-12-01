#include <string>
#include <stdio.h>
#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosio.token/eosio.token.hpp>
#include <eosio.system/eosio.system.hpp>

using namespace std;
using namespace eosio;
class marketplace : public eosio::contract 
{
    private:

      //@abi table users
      struct user
      {
          account_name username;
          uint64_t amount;

          auto primary_key() const {return username;}
      };

      //@abi table owners
      struct owner
      {
          uint64_t content_id;
          std::string content;
          account_name ownername;

          auto primary_key() const {return content_id;}
      };

      //@abi table stores
      struct store
      {
          uint64_t store_id;
          uint64_t content_id;
          std::string content;
          uint32_t value;

          auto primary_key() const {return store_id;}
          //auto primary_key() const { return content_id; }
      };

      struct st_transfer
      {
          name from;
          name to;
          eosio::asset quantity;
          std::string memo;
      };

      struct regaccount_args
      {
          account_name sender;
          checksum256 hash;
          eosio::public_key owner_key;
          eosio::public_key active_key;
      };

      typedef eosio::multi_index<N(users),user> users_table;
      typedef eosio::multi_index<N(owners),owner> owners_table;
      typedef eosio::multi_index<N(stores),store> stores_table;

      users_table _users;
      owners_table _owners;
      stores_table _stores;

    public:
    marketplace(account_name self): contract(self), _users(self,self), _owners(self,self), _stores(self,self){}

    void transfer(uint64_t sender, uint64_t receiver);

    void publish(account_name username, std::string content);

    void sell(uint64_t content_id, account_name owner, uint64_t value);

    void purchase(uint64_t store_id, account_name username);

    void cancell(uint64_t store_id, account_name username);

    void exchange(account_name to, asset quantity, std::string memo);
};