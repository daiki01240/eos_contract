#include "cardgame.hpp"

// Determine which card to be dealt.
// Determine which strategy to be picked by the AI.
// The requirements for random numbers in a blockchain are to be deterministic but not predictable.

int cardgame::random(const int range){

    //find exist seed
    auto seed_iterator = _seed.begin();

    //init seed
    if(seed_iterator == _seed.end()){
        seed_iterator = _seed.emplace(_self, [&](auto& seed){});
    }

    //create seed value
    int prime = 63357;
    auto new_seed_value = (seed_iterator-> value + now()) % prime;

    //update seed
    _seed.modify(seed_iterator, _self, [&](auto&s){
         s.value = new_seed_value;
    });

    //
    int random_result = new_seed_value % range;
    return random_result;
}

void cardgame::draw_one_card(vector<uint8_t>& deck, vector<uint8_t>& hand){

    //get next card
    int deck_card_idx = random(deck.size());

    //手札の枚数分vectorをforで回す
    //EMPTYと一致した場合,first_empty_slotをupdateする
    int first_empty_slot = -1;
    for(int i = 0; i <= hand.size(); i++){
        auto id = hand[i];
        if(card_dict.at(id).type == EMPTY){
            first_empty_slot = i;
            break;
        }
    }
    eosio_assert(first_empty_slot !=  -1, "No empty slot in the player's hand");    

    //vectorのn番目を参照して、random関数で取得したcardをinsertする
    hand[first_empty_slot] = deck[deck_card_idx];

    //deckから対象のカードを削除
    deck.erase(deck.begin() + deck_card_idx);
};