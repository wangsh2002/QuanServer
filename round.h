#ifndef ROUND_H
#define ROUND_H
#include<iostream>
#include"ch.h"
#include<sstream>
#include<string>
#include<vector>
#include<stdlib.h>
#include<unordered_map>
//操作
class Operation
{
public:
    Operation(std::string _initiator, std::string _target, int _priority):
    initiator(_initiator), target(_target), priority(_priority){};
    std::string initiator;//发起者的name
    std::string target;//目标的name
    int priority;//优先级
    //执行操作
    virtual std::string operation(){return "error";};
};
//动作操作
class Move : public Operation
{
public:
    Move(std::string _initiator, std::string _target, int _priority, double _offsetx, double _offsety):
        Operation(_initiator, _target, _priority), offsetx(_offsetx), offsety(_offsety){};
    //执行移动操作
    std::string operation();
private:
    //两个偏移量
    double offsetx;
    double offsety;
};

class Round
{
private:
    //建立玩家
    static std::unordered_map<int, ch> players;
    std::vector<Operation*> poer;//操作数组
    std::vector<std::string> stringSplit(const std::string& str, char delim);//分割字符

public:
    static int delete_player(int fd);
    static int add_player(int fd, ch player);
    //处理指令
    void handle_msg(std::string msg);
    static std::unordered_map<int, ch> get_players();
    //处理回合操作
    std::string handle_round();
    
};


#endif
