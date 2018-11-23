// hey
// this is the header file youvote.hpp

#pragma once

#include <eosiolib/eosio.hpp>

//using namespace eosio; -- not using this so you can explicitly see which eosio functions are used.

class[[eosio::contract]] youvote : public eosio::contract
{

  public:
    //using contract::contract;

    youvote(eosio::name receiver, eosio::name code, eosio::datastream<const char *> ds) : eosio::contract(receiver, code, ds), _polls(receiver, code.value), _votes(receiver, code.value){}

                                                                                                                                                                 // [[eosio::action]] will tell eosio-cpp that the function is to be exposed as an action for user of the smart contract.
                                                                                                                                                                 [[eosio::action]] void
                                                                                                                                                                 version();
    [[eosio::action]] void addpoll(eosio::name s, std::string pollName);
    [[eosio::action]] void rmpoll(eosio::name s, std::string pollName);
    [[eosio::action]] void status(std::string pollName);
    [[eosio::action]] void statusreset(std::string pollName);
    [[eosio::action]] void addpollopt(std::string pollName, std::string option);
    [[eosio::action]] void rmpollopt(std::string pollName, std::string option);
    [[eosio::action]] void vote(std::string pollName, std::string option, std::string accountName);

    //private: -- not private so the cleos get table call can see the table data.

    // create the multi index tables to store the data
    struct [[eosio::table]] poll
    {
        uint64_t key;           // primary key
        uint64_t pollId;        // second key, non-unique, this table will have dup rows for each poll because of option
        std::string pollName;   // name of poll
        uint8_t pollStatus = 0; // staus where 0 = closed, 1 = open, 2 = finished
        std::string option;     // the item you can vote for
        uint32_t count = 0;     // the number of votes for each itme -- this to be pulled out to separte table.

        uint64_t primary_key() const { return key; }
        uint64_t by_pollId() const { return pollId; }
    };
    typedef eosio::multi_index<"poll"_n, poll, eosio::indexed_by<"pollid"_n, eosio::const_mem_fun<poll, uint64_t, &poll::by_pollId>>> pollstable;

    struct [[eosio::table]] pollvotes
    {
        uint64_t key;
        uint64_t pollId;
        std::string pollName; // name of poll
        std::string account;  //this account has voted, use this to make sure noone votes > 1

        uint64_t primary_key() const { return key; }
        uint64_t by_pollId() const { return pollId; }
    };
    typedef eosio::multi_index<"pollvotes"_n, pollvotes, eosio::indexed_by<"pollid"_n, eosio::const_mem_fun<pollvotes, uint64_t, &pollvotes::by_pollId>>> votes;

    //// local instances of the multi indexes
    pollstable _polls;
    votes _votes;
};
