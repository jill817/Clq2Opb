#include <iostream>
#include "Pars.h"
// 解析命令行参数的函数
bool parseArguments(int argc, char *argv[], FilePaths &filePaths)
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input_filename> <output_filename>" << std::endl;
        return false;
    }
    filePaths = FilePaths(argv[1], argv[2]);
    return true;
}
