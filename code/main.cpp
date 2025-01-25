#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "ModelVar.h"
#include "ModelCon.h"
#include "Graph.h"
#include "ModelObjective.h"
#include "Pars.h"
#include "File.h"

// #define DEBUG

void createVariables(ModelVarUtil &varUtil, const Graph &graph)
{
    // 创建 xi 变量
    for (int i = 0; i < graph.numVertices; ++i)
    {
        string varName = "x" + to_string(i);
        size_t varIdx = varUtil.MakeVar(varName, true); // true 表示布尔变量
        varUtil.GetVar(varIdx).SetLowerBound(0);        // 初始化为 0
        varUtil.GetVar(varIdx).SetUpperBound(1);        // 布尔变量，上限为 1
    }

    // 创建 eij 变量
    // for (int i = 0; i < graph.numVertices; ++i)
    // {
    //     for (int j = i + 1; j < graph.numVertices; ++j)
    //     {
    //         string varName = "e" + to_string(i) + "_" + to_string(j);
    //         size_t varIdx = varUtil.MakeVar(varName, true); // true 表示布尔变量
    //         if (graph.getEdge(i, j) == 1)
    //         {                                            // 如果顶点 i 和 j 之间有边
    //             varUtil.GetVar(varIdx).SetLowerBound(1); // 有边，初始化为 1
    //             varUtil.GetVar(varIdx).SetUpperBound(1); // 布尔变量，上限为 1
    //         }
    //         else
    //         {
    //             varUtil.GetVar(varIdx).SetLowerBound(0); // 无边，初始化为 0
    //             varUtil.GetVar(varIdx).SetUpperBound(0); // 布尔变量，上限为 0
    //         }
    //     }
    // }

    // 创建 yij 变量
    for (int i = 0; i < graph.numVertices; ++i)
    {
        for (int j = i + 1; j < graph.numVertices; ++j)
        {
            string varName = "y" + to_string(i) + "_" + to_string(j);
            size_t varIdx = varUtil.MakeVar(varName, true); // true 表示布尔变量
            varUtil.GetVar(varIdx).SetLowerBound(0);        // 初始化为 0
            varUtil.GetVar(varIdx).SetUpperBound(1);        // 布尔变量，上限为 1
        }
    }
}
// 输出所有变量及其当前值的函数
void printAllVariables(const ModelVarUtil &varUtil)
{
    std::cout << "Variables and their current values:" << std::endl;
    for (const auto &var : varUtil.varSet)
    {
        std::cout << "Variable: " << var.name
                  << ", Lower Bound: " << var.lowerBound
                  << ", Upper Bound: " << var.upperBound << std::endl;
    }
}

// 构建约束的函数
void buildConstraints(ModelConUtil &conUtil, ModelVarUtil &varUtil, Graph graph)
{
    // 遍历所有变量
    for (const auto &var : varUtil.varSet)
    {
        // 如果变量是 yij
        if (var.name.find("y") == 0)
        { // 变量名以 "y" 开头
            // 提取 i 和 j
            size_t underscorePos = var.name.find('_');
            int i = stoi(var.name.substr(1, underscorePos - 1)); // 提取 i
            int j = stoi(var.name.substr(underscorePos + 1));    // 提取 j

            // 获取 xi 和 xj 的变量名
            string xiName = "x" + to_string(i);
            string xjName = "x" + to_string(j);

            // 约束 1: yij >= xi
            size_t conIdx1 = conUtil.MakeCon(var.name + "_ge_" + xiName);
            conUtil.GetCon(conIdx1).coeffSet = {1, -1}; // yij - xi >= 0
            conUtil.GetCon(conIdx1).varIdxSet = {
                var.idx,                   // yij 的索引
                varUtil.GetVar(xiName).idx // xi 的索引
            };
            conUtil.GetCon(conIdx1).RHS = 0;
            conUtil.GetCon(conIdx1).isLarge = true; // 表示 >=

            // 约束 2: yij >= xj
            size_t conIdx2 = conUtil.MakeCon(var.name + "_ge_" + xjName);
            conUtil.GetCon(conIdx2).coeffSet = {1, -1}; // yij - xj >= 0
            conUtil.GetCon(conIdx2).varIdxSet = {
                var.idx,                   // yij 的索引
                varUtil.GetVar(xjName).idx // xj 的索引
            };
            conUtil.GetCon(conIdx2).RHS = 0;
            conUtil.GetCon(conIdx2).isLarge = true; // 表示 >=

            // 约束 3: -yij + xi + xj >= 0
            size_t conIdx3 = conUtil.MakeCon(var.name + "_le_" + xiName + "_" + xjName);
            conUtil.GetCon(conIdx3).coeffSet = {-1, 1, 1}; // -yij + xi + xj >= 0
            conUtil.GetCon(conIdx3).varIdxSet = {
                var.idx,                    // yij 的索引
                varUtil.GetVar(xiName).idx, // xi 的索引
                varUtil.GetVar(xjName).idx  // xj 的索引
            };
            conUtil.GetCon(conIdx3).RHS = 0;
            conUtil.GetCon(conIdx3).isLarge = true; // 表示 >=

            // 约束 4: yij >= eij (eij 是已知的 0 或 1)
            // string eijName = "e" + to_string(i) + "_" + to_string(j);
            // Value eijValue = varUtil.GetVar(eijName).lowerBound; // 获取 eij 的值
            // size_t conIdx4 = conUtil.MakeCon(var.name + "_ge_" + eijName);
            size_t conIdx4 = conUtil.MakeCon(var.name + "_ge_" + "eij");
            conUtil.GetCon(conIdx4).coeffSet = {1}; // yij >= eijValue
            conUtil.GetCon(conIdx4).varIdxSet = {
                var.idx // yij 的索引
            };
            // conUtil.GetCon(conIdx4).RHS = eijValue; // 直接将 eij 的值作为 RHS
            conUtil.GetCon(conIdx4).RHS = graph.getEdge(i, j); // 直接将 eij 的值作为 RHS

            conUtil.GetCon(conIdx4).isLarge = true; // 表示 >=
        }
    }
}

// 删除所有 eij 变量的函数
void removeEijVariables(ModelVarUtil &varUtil)
{
    // 遍历 varSet，找到所有以 "e" 开头的变量
    auto it = varUtil.varSet.begin();
    while (it != varUtil.varSet.end())
    {
        if (it->name.find("e") == 0)
        { // 变量名以 "e" 开头
            // 从 name2idx 中删除该变量
            varUtil.name2idx.erase(it->name);

            // 从 varSet 中删除该变量
            it = varUtil.varSet.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // 更新 varIdx2ObjIdx（如果需要）
    varUtil.varIdx2ObjIdx.clear();
    for (size_t i = 0; i < varUtil.varSet.size(); ++i)
    {
        varUtil.varIdx2ObjIdx.push_back(i);
    }
}

// 直接输出所有约束的函数
void printAllConstraints(const ModelConUtil &conUtil, const ModelVarUtil &varUtil)
{
    std::cout << "All constraints:" << std::endl;
    for (const auto &con : conUtil.conSet)
    {
        // 构建约束的左侧表达式
        std::string leftExpr;
        for (size_t k = 0; k < con.varIdxSet.size(); ++k)
        {
            size_t varIdx = con.varIdxSet[k];
            Value coeff = con.coeffSet[k];
            std::string varName = varUtil.varSet[varIdx].name;

            // 添加系数和变量名
            if (k > 0 && coeff > 0)
            {
                leftExpr += " + ";
            }
            else if (coeff < 0)
            {
                leftExpr += " - ";
                coeff = -coeff; // 取绝对值
            }

            if (coeff != 1)
            {
                leftExpr += std::to_string(coeff) + " * ";
            }
            leftExpr += varName;
        }

        // 输出约束
        std::cout << leftExpr << " "
                  << (con.isEqual ? "=" : (con.isLarge ? ">=" : "<=")) << " "
                  << con.RHS << std::endl;
    }
}

// 设置目标函数的函数
void setupObjective(ModelObjective &objective, ModelVarUtil &varUtil)
{
    // 设置目标函数的名称为 "min_sum_xi"，优化方向为最小化
    objective = ModelObjective("min_sum_xi", true);

    // 遍历所有变量，找到 xi 并添加到目标函数中
    for (const auto &var : varUtil.varSet)
    {
        if (var.name.find("x") == 0)
        {                                  // 变量名以 "x" 开头
            objective.addTerm(var.idx, 1); // 系数为 1
        }
    }

    // 设置常数项为 0
    objective.setConstantTerm(0);
}

// 将变量、约束和目标函数输出为 OPB 文件
void writeOPBFile(const std::string &filename, const ModelVarUtil &varUtil, const ModelConUtil &conUtil, const ModelObjective &objective)
{
    std::ofstream outFile(filename);
    if (!outFile.is_open())
    {
        std::cerr << "无法打开文件: " << filename << std::endl;
        return;
    }

    // 输出文件头
    outFile << "* #variable= " << varUtil.varSet.size() << " #constraint= " << conUtil.conSet.size() << std::endl;
    outFile << "* #binary= " << varUtil.binaryNum << " #integer= " << varUtil.integerNum << std::endl;
    outFile << "* NOTE: using following variable name mapping" << std::endl;

    // 输出变量名映射
    for (size_t i = 0; i < varUtil.varSet.size(); ++i)
    {
        outFile << "* x" << (i + 1) << ".." << (i + 1) << " : " << varUtil.varSet[i].name << std::endl;
    }

    // 输出目标函数的缩放比例和偏移量
    outFile << "* Obj. scale : 1" << std::endl;
    outFile << "* Obj. offset : 0" << std::endl;
    outFile << "* NOTE: new objective = (old objective - offset) * scale" << std::endl;

    // 输出目标函数
    outFile << (objective.isMinimize ? "min: " : "max: ");
    for (size_t i = 0; i < objective.varIdxSet.size(); ++i)
    {
        outFile << (objective.coeffSet[i] >= 0 ? "+" : "") << objective.coeffSet[i] << " x" << (objective.varIdxSet[i] + 1) << " ";
    }
    outFile << ";" << std::endl;

    // 输出约束
    for (const auto &con : conUtil.conSet)
    {
        for (size_t i = 0; i < con.varIdxSet.size(); ++i)
        {
            outFile << (con.coeffSet[i] >= 0 ? "+" : "") << con.coeffSet[i] << " x" << (con.varIdxSet[i] + 1) << " ";
        }
        outFile << (con.isLarge ? ">=" : "<=") << " " << con.RHS << " ;" << std::endl;
    }

    outFile.close();
    std::cout << "OPB 文件已成功写入: " << filename << std::endl;
}

int main(int argc, char *argv[])
{
    FilePaths filePaths;

    // 解析命令行参数
    if (!parseArguments(argc, argv, filePaths))
    {
        return 1; // 如果参数不正确，返回错误码
    }
    Graph graph = readGraphFromFile(filePaths.getInputFilename());
    // 输出图的顶点数和边数
    std::cout << "顶点数: " << graph.numVertices << std::endl;
    std::cout << "边数: " << graph.numEdges << std::endl;

    // 创建 ModelVarUtil 对象
    ModelVarUtil varUtil;
    ModelConUtil conUtil;
    ModelObjective objective;
    // 调用函数创建变量
    createVariables(varUtil, graph);
#ifdef DEBUG
    printAllVariables(varUtil);
#endif

    buildConstraints(conUtil, varUtil, graph);
#ifdef DEBUG
    printAllConstraints(conUtil, varUtil);
#endif
    // removeEijVariables(varUtil);

    setupObjective(objective, varUtil);
#ifdef DEBUG
    objective.print(varUtil);
#endif

    writeOPBFile(filePaths.getOutputFilename(), varUtil, conUtil, objective);
    return 0;
}