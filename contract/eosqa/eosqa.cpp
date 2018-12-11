#include "eosqa.hpp"

void eosqa::addquestion(string title, string body, account_name owner)
{
    require_auth(owner);

    _questions.emplace(get_self(), [&](auto &q) 
    {
        q.question_key = _questions.available_primary_key();
        q.title = title;
        q.body = body;
        q.owner = owner;
        q.time_stamp = eosio::time_point_sec(now());
    });
};

void eosqa::addanswer(uint64_t question_key, string answer, account_name from)
{
    require_auth(from);

    _answers.emplace(get_self(), [&](auto &a) {
        a.answer_key = _answers.available_primary_key();
        a.question_key = question_key;
        a.answer = answer;
        a.username = from;
        a.time_stamp = eosio::time_point_sec(now());
    });
};

void eosqa::voteanswer(account_name from,uint64_t answer_key)
{
    //1.送り主か判定
    require_auth(from);

    //2.idがあるのか判定
    //eosio_assert();
    auto answers_id = _answers.find(answer_key);
    auto question_key = answers_id->question_key;
    auto question_id = _questions.find(question_key);
    //eosio_assert(answers_id == null, "Invalid your ID!");

    //3.+ か - の判定
    //vectorに入ってるかの判定が必要
    //入っていれば +,入ってなければ -
    //vectorの検索条件はintっぽい、どこの配列に入っているか。

    if (answers_id != _answers.end())
    {
        _answers.modify(answers_id, _self, [&](auto &a) {
            a.vote.push_back(from);
        });
    }

    if (question_id != _questions.end())
    {
        _questions.modify(question_id, _self, [&](auto &q) {
            q.allvote++;
        });
    }

    //players.modify（iterator、account、[＆] （auto＆player） { player.inventory.erase（i）; } ）;
    //update vote & allvote
};

void eosqa::payment(uint64_t view, uint64_t index)
{
    require_auth(_self);

    auto question_index = _questions.find(index);
    auto question_benefit = view * 0.1;
    auto owner_benefit = question_benefit / 2;
    auto one_answer_benefit = owner_benefit / question_index->allvote;

    if (question_index != _questions.end())
    {
        action(permission_level{get_self(), N(active)},
               N(eosio.token), N(transfer),
               std::make_tuple(get_self(), question_index->owner, owner_benefit, "send your reword"))
            .send();

        _questions.modify(question_index, _self, [&](auto &q) {
            q.price = q.price + owner_benefit;
        });
    }

    for (auto &item : _answers)
    {
        if (item.question_key == question_index->question_key)
        {
            auto quantity = one_answer_benefit * item.vote.size();
            action(permission_level{get_self(), N(active)},
                   N(eosio.token), N(transfer),
                   std::make_tuple(get_self(), item.username, quantity, "send your reword"))
                .send();
        }
        }
};

EOSIO_ABI(eosqa, (addquestion)(addanswer)(voteanswer)(payment));