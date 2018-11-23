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

    _users.modify(user, username, [&](auto& modified_user){

        game game_data;

        for(uint8_t i = 0; i < 4; i++ ){
            draw_one_card(game_data.deck_player, game_data.hand_player);
            draw_one_card(game_data.deck_ai, game_data.hand_ai);
        }
        
        modified_user.game_data = game_data;
    });
}

EOSIO_ABI(cardgame, (login))