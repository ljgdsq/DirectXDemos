#pragma once
#include <map>
#include <string>
using namespace std;
void test()
{
    std::map<int, void*> _m;
    int a = 10;
    std::string str = "str";
    _m.insert(make_pair(1,&a));
}