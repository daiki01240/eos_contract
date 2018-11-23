// youvote.cpp

#include "youvote.hpp"

// note we are explcit in our use of eosio library functions
// note we liberally use print to assist in debugging

// public methods exposed via the ABI

void youvote::version()
{
    eosio::print("YouVote version  0.22");
};

void youvote::addpoll(eosio::name s, std::string pollName)
{
    // require_auth(s);

    eosio::print("Add poll ", pollName);

    // update the table to include a new poll
    _polls.emplace(get_self(), [&](auto &p) {
        p.key = _polls.available_primary_key();
        p.pollId = _polls.available_primary_key();
        p.pollName = pollName;
        p.pollStatus = 0;
        p.option = "";
        p.count = 0;
    });
}

void youvote::rmpoll(eosio::name s, std::string pollName)
{
    //require_auth(s);

    eosio::print("Remove poll ", pollName);

    std::vector<uint64_t> keysForDeletion;
    // find items which are for the named poll
    for (auto &item : _polls)
    {
        if (item.pollName == pollName)
        {
            keysForDeletion.push_back(item.key);
        }
    }

    // now delete each item for that poll
    for (uint64_t key : keysForDeletion)
    {
        eosio::print("remove from _polls ", key);
        auto itr = _polls.find(key);
        if (itr != _polls.end())
        {
            _polls.erase(itr);
        }
    }

    // add remove votes ... don't need it the axtions are permanently stored on the block chain

    std::vector<uint64_t> keysForDeletionFromVotes;
    // find items which are for the named poll
    for (auto &item : _votes)
    {
        if (item.pollName == pollName)
        {
            keysForDeletionFromVotes.push_back(item.key);
        }
    }

    // now delete each item for that poll
    for (uint64_t key : keysForDeletionFromVotes)
    {
        eosio::print("remove from _votes ", key);
        auto itr = _votes.find(key);
        if (itr != _votes.end())
        {
            _votes.erase(itr);
        }
    }
}

void youvote::status(std::string pollName)
{
    eosio::print("Change poll status ", pollName);

    std::vector<uint64_t> keysForModify;
    // find items which are for the named poll
    for (auto &item : _polls)
    {
        if (item.pollName == pollName)
        {
            keysForModify.push_back(item.key);
        }
    }

    // now get each item and modify the status
    for (uint64_t key : keysForModify)
    {
        eosio::print("modify _polls status", key);
        auto itr = _polls.find(key);
        if (itr != _polls.end())
        {
            _polls.modify(itr, get_self(), [&](auto &p) {
                p.pollStatus = p.pollStatus + 1;
            });
        }
    }
}

void youvote::statusreset(std::string pollName)
{
    eosio::print("Reset poll status ", pollName);

    std::vector<uint64_t> keysForModify;
    // find all poll items
    for (auto &item : _polls)
    {
        if (item.pollName == pollName)
        {
            keysForModify.push_back(item.key);
        }
    }

    // update the status in each poll item
    for (uint64_t key : keysForModify)
    {
        eosio::print("modify _polls status", key);
        auto itr = _polls.find(key);
        if (itr != _polls.end())
        {
            _polls.modify(itr, get_self(), [&](auto &p) {
                p.pollStatus = 0;
            });
        }
    }
}

void youvote::addpollopt(std::string pollName, std::string option)
{
    eosio::print("Add poll option ", pollName, "option ", option);

    // find the pollId, from _polls, use this to update the _polls with a new option
    for (auto &item : _polls)
    {
        if (item.pollName == pollName)
        {
            // can only add if the poll is not started or finished
            if (item.pollStatus == 0)
            {
                _polls.emplace(get_self(), [&](auto &p) {
                    p.key = _polls.available_primary_key();
                    p.pollId = item.pollId;
                    p.pollName = item.pollName;
                    p.pollStatus = 0;
                    p.option = option;
                    p.count = 0; });
            }
            else
            {
                eosio::print("Can not add poll option ", pollName, "option ", option, " Poll has started or is finished.");
            }

            break; // so you only add it once
        }
    }
}

void youvote::rmpollopt(std::string pollName, std::string option)
{
    eosio::print("Remove poll option ", pollName, "option ", option);

    std::vector<uint64_t> keysForDeletion;
    // find and remove the named poll
    for (auto &item : _polls)
    {
        if (item.pollName == pollName)
        {
            keysForDeletion.push_back(item.key);
        }
    }

    for (uint64_t key : keysForDeletion)
    {
        eosio::print("remove from _polls ", key);
        auto itr = _polls.find(key);
        if (itr != _polls.end())
        {
            if (itr->option == option)
            {
                _polls.erase(itr);
            }
        }
    }
}

void youvote::vote(std::string pollName, std::string option, std::string accountName)
{
    eosio::print("vote for ", option, " in poll ", pollName, " by ", accountName);

    // is the poll open
    for (auto &item : _polls)
    {
        if (item.pollName == pollName)
        {
            if (item.pollStatus != 1)
            {
                eosio::print("Poll ", pollName, " is not open");
                return;
            }
            break; // only need to check status once
        }
    }

    // has account name already voted?
    for (auto &vote : _votes)
    {
        if (vote.pollName == pollName && vote.account == accountName)
        {
            eosio::print(accountName, " has already voted in poll ", pollName);
            //eosio_assert(true, "Already Voted");
            return;
        }
    }

    uint64_t pollId = 99999; // get the pollId for the _votes table

    // find the poll and the option and increment the count
    for (auto &item : _polls)
    {
        if (item.pollName == pollName && item.option == option)
        {
            pollId = item.pollId; // for recording vote in this poll
            _polls.modify(item, get_self(), [&](auto &p) {
                p.count = p.count + 1;
            });
        }
    }

    // record that accountName has voted
    _votes.emplace(get_self(), [&](auto &pv) {
        pv.key = _votes.available_primary_key();
        pv.pollId = pollId;
        pv.pollName = pollName;
        pv.account = accountName;
    });
}

EOSIO_DISPATCH(youvote, (version)(addpoll)(rmpoll)(status)(statusreset)(addpollopt)(rmpollopt)(vote))