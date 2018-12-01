#include "tatami.hpp"

void tatami::registersh(account_name school_name, std::string pub_key)
{
    require_auth(school_name);
    //_users.get(school, "User is already exist");

    _schools.emplace(get_self(),[&](auto &s)
    {
        s.school_id = _schools.available_primary_key();
        s.school_name = school_name;
        s.pub_key = pub_key;
    });
}

void tatami::registerst(account_name student_name)
{
    require_auth(student_name);
    //_users.get(school, "User is already exist");

    _students.emplace(get_self(), [&](auto &s) {
        s.student_name = student_name;
    });
}

void tatami::addclaim(account_name student_name, std::string signature, std::string raw_type)
{
    //_self
    require_auth(student_name);

        auto student = _students.find(student_name);
        if (student != _students.end())
        {
            _students.modify(student, _self, [&](auto &s) {
                s.signature.push_back(signature);
                s.raw_type.push_back(raw_type);
                s.counter++;
            });
        }
    // });
}

void tatami::verifyclaim(account_name student_name, uint64_t index)
{
    require_auth(student_name);
    //_users.get(school, "User is already exist");
    auto student = _students.find(student_name);
    auto sig = student->signature[index];
    auto data = student->raw_type[index];

    action(permission_level{get_self(), N(active)},
           N(stateofchain), N(ecrecover),
           std::make_tuple(sig, data))
        .send();
}

EOSIO_ABI(tatami, (registersh)(registerst)(addclaim)(verifyclaim))