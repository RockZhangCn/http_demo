#include "common.h"
string Trim(string str)//去除str两端的空格
{
    char target = ' ';
    auto left = str.find_first_not_of(target);
    auto right = str.find_last_not_of(target);
    return str.substr(left, right - left + 1);
}

std::vector<int> FindAllPosition(string str ,string flag,int num)//查询str内所有flag的位置
{

    std::vector<int> Result;
    auto Pos = str.find(flag);
    while (Pos != -1 && Result.size() != num)
    {
        Result.push_back(Pos);
        Pos = str.find(flag, *(--Result.end()) + 1);
    }
    return Result;
}

std::vector<string> Split(string str,string flag,int num,int skipEmpty)//将字符串的按flag拆分
{

    std::vector<string> dataSet;
    auto PushData = [&dataSet, skipEmpty](string line)
    {
        if (line.length() != 0 || !skipEmpty)
        {
            dataSet.push_back(line);
        }
    };
    auto Pos = FindAllPosition(str,flag, num != 0 ? num : -1);
    if (!Pos.size())
    {
        return dataSet;
    }
    for (int i = 0; i < Pos.size(); i++)
    {
        if (dataSet.size() == num && num != 0)
        {	//满足数量直接截到结束
            PushData(str.substr(Pos[dataSet.size()] + flag.size()));
            break;
        }
        if (i == 0 && Pos[0] != 0)
        {	//第一个数的位置不是0的话补上
            PushData(str.substr(0, Pos[0]));
        }
        if (i != Pos.size() - 1)
        {
            int Left = Pos[i] + flag.length();
            int Right = Pos[i + 1] - Left;
            PushData(str.substr(Left, Right));
        }
        else
        {	//最后一个标志到结束
            PushData(str.substr(*(--Pos.end()) + flag.size()));
        }
    }
    return dataSet;
}
