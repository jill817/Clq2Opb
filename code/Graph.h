#include <vector>
#include <sstream>
#include <string>

class Graph
{
public:
    int numVertices;                 // 顶点数
    int numEdges;                    // 边数
    std::vector<int> adjacencyArray; // 一维数组存储邻接矩阵的下三角部分

    // 构造函数
    Graph(int vertices = 0, int edges = 0)
        : numVertices(vertices), numEdges(edges), adjacencyArray(vertices * (vertices + 1) / 2, 0) {}

    // 获取边的值（无向图，u 和 v 的顺序不重要）
    int getEdge(int u, int v) const
    {
        if (u < v)
            std::swap(u, v); // 确保 u >= v，访问下三角部分
        return adjacencyArray[u * (u + 1) / 2 + v];
    }

    // 设置边的值（无向图，u 和 v 的顺序不重要）
    void setEdge(int u, int v, int value)
    {
        if (u < v)
            std::swap(u, v); // 确保 u >= v，访问下三角部分
        adjacencyArray[u * (u + 1) / 2 + v] = value;
    }
};
Graph readGraphFromFile(const std::string &filename);