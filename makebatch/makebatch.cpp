#include <iostream>
#include <fstream>
#include <string>

int main()
{
    std::ofstream file;
    std::string commitMessage;
    
#ifdef _WIN32
    file.open("commit.bat");
#elif __APPLE__
    file.open("commit.command");
#else
    file.open("commit");
#endif

    std::cout << "Please write a summary of what you did in this commit:";
    std::getline(std::cin, commitMessage);

    file << "git add . \n";
    file << "git commit -m \"" << commitMessage << "\" \n";

    file.close();

#ifdef _WIN32
    system("commit.bat");
#elif __APPLE__
        system("chmod 755 commit.command");
        system("./commit.command");
#else
    system("chmod 755 commit");
    system("./commit");
#endif

    return 0;
}