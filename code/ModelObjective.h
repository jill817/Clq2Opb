#pragma once
#include "header.h"
#include "ModelCon.h"
#include "ModelVar.h"

class ModelObjective
{
public:
    std::string name;              // 目标函数名称
    bool isMinimize;               // 是否为最小化目标函数（true 表示最小化，false 表示最大化）
    std::vector<Value> coeffSet;   // 目标函数中变量的系数
    std::vector<size_t> varIdxSet; // 目标函数中变量的索引
    Value constantTerm;            // 目标函数中的常数项
    ModelObjective(const std::string &_name = "objective", bool _isMinimize = true);
    void addTerm(size_t varIdx, Value coeff);
    void setConstantTerm(Value constant);
    std::string getExpression(const ModelVarUtil &varUtil) const;
    void print(const ModelVarUtil &varUtil) const;
};