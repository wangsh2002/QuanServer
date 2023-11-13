#include"ch.h"


void ch::init(std::string _name, double _x, double _y)
{
    name = _name;
    pos = {_x, _y};
    ps = 1000;//体力
    th = 100;//韧性
    pw = 100;//力量
    bl = 300;//平衡
    hp = 500;//生命
}

bool ch::is_hit(std::pair<double, double> attack_pos)
{
    return false;
}

bool ch::under_attack(double attack)
{
    return false;
}

std::string ch::pos_shift(double Offsetx, double Offsety)
{
    pos.first += Offsetx;
    pos.second += Offsety;
    return std::string(name + "走到了" + "(" + std::to_string(pos.first) + "," + std::to_string(pos.second) + ")");
}

std::string ch::get_name()
{
    return name;
}
