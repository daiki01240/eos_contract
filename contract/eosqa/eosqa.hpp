#include <string>
#include <stdio.h>
#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosio.token/eosio.token.hpp>
#include <eosio.system/eosio.system.hpp>

using namespace std;
using namespace eosio;

class eosqa : public eosio::contract
{
    private:
    //@abi table questions
    struct question
    {
        uint64_t question_key;
        string title;
        string body;
        account_name owner;
        uint64_t price = 0;
        uint64_t view = 0;
        uint64_t allvote = 0;
        uint64_t flg = 0;
        eosio::time_point_sec time_stamp;

        uint64_t primary_key() const { return question_key; }
    };

    //@abi table answers
    struct answer
    {
        uint64_t answer_key;
        uint64_t question_key;
        string answer;
        account_name username;
        vector<account_name> vote = {};
        eosio::time_point_sec time_stamp;
     
        uint64_t primary_key() const { return answer_key; }
    };

    typedef eosio::multi_index<N(questions), question> question_table;
    typedef eosio::multi_index<N(answers), answer> answer_table;

    question_table _questions;
    answer_table _answers;

    int mathlogic(uint64_t pv);

  public:
    eosqa(account_name self) : contract(self), _questions(self, self), _answers(self, self) {}

    void addquestion(string title, string body, account_name owner);

    void addanswer(uint64_t question_key, string answer, account_name from);

    //void voteAnswer(uint64_t index, account_name from);
    void voteanswer(account_name from, uint64_t id);

    void payment(uint64_t view, uint64_t index);
};