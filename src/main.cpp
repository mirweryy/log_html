#include "../include/parselib.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "The number of arguments is less than 2" << std::endl;
        return 0;
    }
    else
    {
        char path[100];
        strncpy(path, argv[1], sizeof(path) - 1);
        path[sizeof(path) - 1] = '\0';
        parserlog a(path);
    }
    return 0;
}