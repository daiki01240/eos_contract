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
      struct school1
      {
          uint64_t school_id;
          account_name school_name;
          eosio::public_key pub_key;

          auto primary_key() const { return school_id; }
      };

      //@abi table students
      struct student1
      {
          account_name student_name;
          vector<signature> signature = {};
          vector<string> raw_type = {};
          vector<string> photo = {};
          vector<eosio::time_point_sec> time_stamp = {};
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

      typedef eosio::multi_index<N(schools),school1> schools_table;
      typedef eosio::multi_index<N(students), student1> students_table;

      schools_table _schools;
      students_table _students;

    public:
      tatami(account_name self) : contract(self), _schools(self, self), _students(self, self){}

      void registersh(account_name school_name, const eosio::public_key &pk);

      void registerst(account_name student_name);

      void addclaim(account_name student_name, const signature &sig, std::string raw_type);

      void verifyclaim(account_name student_name, uint64_t index, const eosio::public_key &pk);
};