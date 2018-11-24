#include "gameplay.cpp"

void cardgame::login(account_name username){
    
    //verify account
    require_auth(username);

    auto user_iterator = _users.find(username);
    if(user_iterator == _users.end()){
        user_iterator = _users.emplace(username, [&](auto &new_user) {
            new_user.name = username;
        });
    }
}

void cardgame::startgame(account_name username){
    
    require_auth(username);

    auto& user = _users.get(username, "User doesnt exist");
    _users.modify(user, username, [&](auto& modified_user){

        game game_data;

        for(uint8_t i = 0; i < 4; i++ ){
            draw_one_card(game_data.deck_player, game_data.hand_player);
            draw_one_card(game_data.deck_ai, game_data.hand_ai);
        }
        
        modified_user.game_data = game_data;
    });
}

void cardgame::playcard(account_name username, uint8_t player_card_idx){

    require_auth(username);

    eosio_assert(player_card_idx < 4, "playcard: Invalid hand index");

    auto& user = _users.get(username, "User doesnt exist");

    eosio_assert(user.game_data.status == ONGOING, "playcard: This game has ended Please start a new one");
    eosio_assert(user.game_data.selected_card_player == 0, "playcard: The player has played his card this turn!");

    _users.modify(user, username, [&](auto& modified_user){
        game& game_data = modified_user.game_data;

        game_data.selected_card_player = game_data.hand_player[player_card_idx];
        game_data.hand_player[player_card_idx] = 0;

        int ai_card_idx = ai_choose_card(game_data);
        game_data.selected_card_ai = game_data.hand_ai[ai_card_idx];
        game_data.hand_ai[ai_card_idx] = 0;
    });
}


EOSIO_ABI(cardgame, (login)(startgame)(playcard))