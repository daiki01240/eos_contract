#include "tatami.hpp"

///@abi action
void tatami::registersh(account_name school_name, const eosio::public_key &pk)
{
    //require_auth(school_name);
    //_users.get(school, "User is already exist");

    _schools.emplace(get_self(),[&](auto &s)
    {
        s.school_id = _schools.available_primary_key();
        s.school_name = school_name;
        s.pub_key = pk;
    });
}

///@abi action
void tatami::registerst(account_name student_name)
{
    //require_auth(student_name);
    //_users.get(school, "User is already exist");

    _students.emplace(get_self(), [&](auto &s) {
        s.student_name = student_name;
    });
}

///@abi action
void tatami::addclaim(account_name student_name, const signature &sig, std::string raw_type)
{
    //_self
    //require_auth(student_name);

        auto student_pid = _students.find(student_name);
        if (student_pid != _students.end())
        {
            _students.modify(student_pid, _self, [&](auto &s) {
                s.signature.push_back(sig);
                s.raw_type.push_back(raw_type);
                s.counter++;
            });
        }
    // });
}

///@abi action
void tatami::verifyclaim(account_name student_name, uint64_t index, const eosio::public_key &pk)
{
    //require_auth(student_name);
    //_users.get(school, "User is already exist");
    auto student = _students.find(student_name);
    auto sig = student->signature[index];
    auto data = student->raw_type[index];

    action(permission_level{get_self(), N(eosio.code)},
           N(stateofchain), N(ecverify),
           std::make_tuple("ok", "SIG_K1_K4Smz9hhB3tT5rRa2yqCYejbEyyDHf4ZC1yKboaFqYEWEaNbAuPFTZXzTj6K9mzK1xvDZAM9u8L5UcjEjanPkEy6qnWJhX", "EOS7FUkRDj64C92GpyjbHiva59jG3VhEs4GhS3VwfQ1HYFgVfDAzW"))
        .send();
}

EOSIO_ABI(tatami, (registersh)(registerst)(addclaim)(verifyclaim))