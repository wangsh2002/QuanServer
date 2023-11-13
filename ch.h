#ifndef CH_H
#define CH_H
#include<iostream>
#include<map>
#include<string>
#include<vector>
#include<sstream>
#include<string>

class ch
{
private:
    std::string name;//玩家姓名
    double ps;//体力
    double th;//韧性
    double pw;//力量
    double bl;//平衡
    double hp;//生命
    std::pair<double, double> pos;//位置
    
public:
    //初始化角色
    void init(std::string _name,double _x, double _y);
    //判断是否能击中
    bool is_hit(std::pair<double, double> attack_pos);
    //受到攻击
    bool under_attack(double attack);
    //位移
    std::string pos_shift(double Offsetx, double Offsety);
    //获取姓名
    std::string get_name();
};

#endif