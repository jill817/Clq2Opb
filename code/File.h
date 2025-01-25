#pragma once
#include <string>

class FilePaths
{
public:
    FilePaths(const std::string &inputFilename = "", const std::string &outputFilename = "")
        : inputFilename_(inputFilename), outputFilename_(outputFilename) {}

    std::string getInputFilename() const { return inputFilename_; }
    std::string getOutputFilename() const { return outputFilename_; }

private:
    std::string inputFilename_;
    std::string outputFilename_;
};