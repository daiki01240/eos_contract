#include <string>
#include <stdio.h>
#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosio.token/eosio.token.hpp>
#include <eosio.system/eosio.system.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/crypto.h>

using namespace eosio;

class tatami : public eosio::contract
{
    private:
      //@abi table schools
      struct schoolaa
      {
          uint64_t school_id;
          std::string school_name;
          std::string pub_key;

          auto primary_key() const { return school_id; }
      };

      //@abi table students
      struct studentaa
      {
          account_name student_name;
          vector<string> signature = {};
          vector<string> raw_type = {};
          uint64_t counter = 0;
          //   vector<cliam> claim;
          auto primary_key() const { return student_name; }
      };

    //   //@abi table informations
    //   struct infomation
    //   {
    //       std::stirng signature;
    //       std::string row_type;
    //   }

    //   typedef std::string claim;

      typedef eosio::multi_index<N(schools),schoolaa> schools_table;
      typedef eosio::multi_index<N(students), studentaa> students_table;

      schools_table _schools;
      students_table _students;

    public:
      tatami(account_name self) : contract(self), _schools(self, self), _students(self, self){}

      void registersh(account_name school_name, std::string pub_key);

      void registerst(account_name student_name);

      void addclaim(account_name student_name, std::string signature, std::string row_type);

      void verifyclaim(account_name student_name, uint64_t index);


};