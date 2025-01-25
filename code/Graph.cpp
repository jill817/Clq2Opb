#include "Graph.h"
#include <fstream>
#include <sstream>
#include <iostream>

// 读取图文件的函数
Graph readGraphFromFile(const std::string &filename)
{
    std::ifstream file(filename, std::ios::binary); // 以二进制模式打开文件
    if (!file.is_open())
    {
        std::cerr << "无法打开文件，请检查文件路径和权限" << std::endl;
        return Graph(); // 返回一个空的图对象
    }

    int numVertices = 0, numEdges = 0;
    std::string line;
    std::getline(file, line);
    if (line.find("p edge") == 0)
    { // 检查是否以 "p edge" 开头
        std::istringstream iss(line);
        std::string type, edge;
        iss >> type >> edge >> numVertices >> numEdges;
        if (numVertices <= 0 || numEdges <= 0)
        {
            std::cerr << "文件格式错误：顶点数或边数无效" << std::endl;
            return Graph(); // 返回一个空的图对象
        }
    }

    // 初始化图对象
    Graph graph(numVertices, numEdges);

    while (std::getline(file, line))
    {
        if (line.find("e") == 0)
        { // 检查是否以 "e" 开头
            std::istringstream iss(line);
            std::string type;
            int u, v;
            iss >> type >> u >> v;
            if (u > 0 && u <= numVertices && v > 0 && v <= numVertices)
            {
                graph.setEdge(u - 1, v - 1, 1); // 顶点编号从 1 开始，数组索引从 0 开始
            }
        }
    }

    file.close();
    return graph;
}
