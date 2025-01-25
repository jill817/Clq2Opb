#include "ModelObjective.h"
// 构造函数
ModelObjective::ModelObjective(const std::string &_name, bool _isMinimize)
    : name(_name), isMinimize(_isMinimize), constantTerm(0) {}

// 添加变量项
void ModelObjective::addTerm(size_t varIdx, Value coeff)
{
    varIdxSet.push_back(varIdx);
    coeffSet.push_back(coeff);
}

// 设置常数项
void ModelObjective::setConstantTerm(Value constant)
{
    constantTerm = constant;
}

// 获取目标函数的数学表达式
std::string ModelObjective::getExpression(const ModelVarUtil &varUtil) const
{
    std::string expr;
    for (size_t i = 0; i < varIdxSet.size(); ++i)
    {
        if (i > 0 && coeffSet[i] > 0)
        {
            expr += " + ";
        }
        else if (coeffSet[i] < 0)
        {
            expr += " - ";
        }
        expr += std::to_string(coeffSet[i]) + " * " + varUtil.varSet[varIdxSet[i]].name;
    }
    if (constantTerm != 0)
    {
        expr += " + " + std::to_string(constantTerm);
    }
    return expr;
}

// 输出目标函数
void ModelObjective::print(const ModelVarUtil &varUtil) const
{
    std::cout << (isMinimize ? "Minimize: " : "Maximize: ") << getExpression(varUtil) << std::endl;
}