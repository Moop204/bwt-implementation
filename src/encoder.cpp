#include <vector>
#include <string>
#include <iostream>
#include <algorithm>


bool DISPLAY = false;

void displayPermutations(const std::vector<std::string> perm)
{
    for (std::string s : perm)
    {
        std::cout << s << "\n";
    }
}

/*
void eraseSet(int val, std::unordered_set<int> &s)
{
    for (auto it = s.begin(); it != s.end(); ++it)
    {
        if (*it == val)
        {
            s.erase(it);
            return;
        }
    }
}
*/
std::string shiftString(std::string &s)
{
    std::string tmp = s.substr(1, s.size()) + s.substr(0, 1);
    if(DISPLAY)
    std::cout << tmp;
    return tmp;
}


int main(void)
{
    std::string message_code;
    //std::cout << "Enter string to be transformed with Burrows-Wheeler transform, with one symbol reserved to denote end of line ($): ";
    std::cin >> message_code;
    if (message_code == "")
    {
        message_code = "doggosgofasto";
    }
    message_code += '\n';

    std::vector<std::string> circular_shifts{};
    std::string permutations = message_code;

    //std::cout << permutations << std::endl;

    int size = permutations.size();
    for (int i = 0; i < size; ++i)
    {
        circular_shifts.push_back(permutations);
        permutations = shiftString(permutations);
    }

    if(DISPLAY) {
      std::cout << "Each permutation\n";
      displayPermutations(circular_shifts);
    }
    std::sort(circular_shifts.begin(), circular_shifts.end());
    if(DISPLAY) {
      std::cout << "Each sorted permutation\n";
      displayPermutations(circular_shifts);
    }

    std::string lastcol{""};

    for (std::string s : circular_shifts)
    {
        lastcol += s.back();
    }

    //std::cout << "Take the last column\n";
    std::cout << lastcol;

    // Compare compression here
    //
}
