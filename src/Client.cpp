//    Battleship game assignment for MSU CSCI 366
//    Copyright (C) 2020    Mike P. Wittie
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "common.hpp"
#include "Client.hpp"

Client::~Client() {
    remove(const_cast<char *>(board_name.c_str()));
}


void Client::initialize(unsigned int player, unsigned int board_size){
    this->player = player;
    this->board_size = board_size;

    if(player < 1 || player > MAX_PLAYERS)
        throw ClientWrongPlayerNumberException();

    this->board_name = "player_ " + to_string(player) + ".action_board.json";

    vector< vector<int> > board(board_size, vector<int>(board_size));
    ofstream action_board_file(board_name);
    cereal::JSONOutputArchive archive(action_board_file);
    archive(CEREAL_NVP(board));

    initialized = true;
}

void Client::fire(unsigned int x, unsigned int y) {
    ofstream out_file("player_1.shot.json");
    cereal::JSONOutputArchive archive(out_file);
    archive(CEREAL_NVP(x),CEREAL_NVP(y));

}

bool Client::result_available() {
    string file1 = "player_1.result.json";
    string file2 = "player_2.result.json";
    ifstream player1(file1);
    ifstream player2(file2);

    if (player1 and player2){
        return true;
    } else{
        return false;
    }

}

int Client::get_result() {
    int end;
    ifstream result("player_1.result.json");
    cereal::JSONInputArchive arch(result);
    arch(end);

    switch(end){
        case HIT:
            std::remove("player_1.result.json");
            std::remove("player_2.result.json");
            return HIT;
        case MISS:
            return MISS;
        case OUT_OF_BOUNDS:
            return OUT_OF_BOUNDS;
    }
    if(end < -1 or end > 1){
        throw ClientException("Bad Result from file");
    }
    return end;
}

void Client::update_action_board(int result, unsigned int x, unsigned int y) {
    vector<vector<int> > vect(board_size, vector<int> (board_size,0));
    vect[y][x] = result;
    ofstream arr("player_1.action_board.json");
    cereal::JSONOutputArchive arch_write(arr);
    arch_write(cereal::make_nvp("board", vect));

}

string Client::render_action_board(){
}