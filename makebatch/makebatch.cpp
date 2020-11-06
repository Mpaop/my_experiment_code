#include <iostream>
#include <fstream>
#include <string>

int main()
{
    std::ofstream file;
    std::string commitMessage;

    file.open("commit.bat");

    std::cout << "Please write a summary of what you did in this commit:";
    std::getline(std::cin, commitMessage);

    file << "git add . \n";
    file << "git commit -m \"" << commitMessage << "\" \n";

    file.close();

    system("commit.bat");

    return 0;
}