#include"round.h"

std::vector<std::string> Round::stringSplit(const std::string &str, char delim)
{
    std::stringstream ss(str);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delim)) {
        if (!item.empty()) {
            elems.push_back(item);
        }
    }
    return elems;
}

int Round::delete_player(int fd)
{
    players.erase(fd);
    return players.size();
}

int Round::add_player(int fd, ch player)
{
    players.insert({fd, player});
    return players.size();
}

void Round::handle_msg(std::string msg)
{
    std::vector<std::string> msg_split = stringSplit(msg, ' ');
    if(msg_split[0] == "move")
    {
        if(msg_split[1] == "1")
        {
            poer.push_back(new Move(msg_split[3], msg_split[3], 3, 0.0, 1.0 * std::stoi(msg_split[2])));
        }
        else if(msg_split[1] == "2")
        {
            poer.push_back(new Move(msg_split[3], msg_split[3], 3, 1.0 * std::stoi(msg_split[2]), 0.0));
        }
        else if(msg_split[1] == "3")
        {
            poer.push_back(new Move(msg_split[3], msg_split[3], 3, 0.0, -1.0 * std::stoi(msg_split[2])));
        }
        else if(msg_split[1] == "4")
        {
            poer.push_back(new Move(msg_split[3], msg_split[3], 3, -1.0 * std::stoi(msg_split[2]), 0.0));
        }
    }
}

std::unordered_map<int, ch> Round::get_players()
{
    return players;
}

std::string Round::handle_round()
{
    std::string ret = "";
    for(int i = 0; i < poer.size(); i++)
    {
        ret += poer[i]->operation() + "\n";
    }
    poer.clear();
    return ret;
}

std::string Move::operation()
{
    std::string ret = "";
    auto players = Round::get_players();
    for(auto player : players)
    {
        if(player.second.get_name() == target)
        {
            ret += player.second.pos_shift(offsetx, offsety);
            break;
        }
    }
    return ret;
}
