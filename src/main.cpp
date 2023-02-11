// Copyright (c) 2022 Berk Kırtay

#include <iostream>
#include <sstream>
#include "Frame.h"
std::vector<std::string> parseArgs(int argc, char** argv);

int main(int argc, char** argv)
{
    auto parameters = parseArgs(argc, argv);
    Frame f;
    f.initializeComponents();
    f.load(parameters[1].c_str());
    f.mainLoop();
    return 0;
}

std::vector<std::string> parseArgs(int argc, char** argv)
{
    std::stringstream builder;
    builder << "Usage:\n"
        << "./{executable} {--ROM} {ROM_PATH} ";

    std::string usage = builder.str();

    if (argc != 3)
    {
        std::cout
            << "FAILURE: Incorrect number of arguments!\n"
            << argc << " given, 3 required.\n"
            << usage
            << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string arg1 = std::string(argv[1]);
    if (arg1 != "--ROM")
    {
        std::cout
            << "FAILURE: Given arguments are incorrect!\n"
            << usage
            << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string arg2 = std::string(argv[2]);
    return { arg1, arg2 };
}