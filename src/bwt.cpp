#include "bwt.h"
#include <set>
#include <iostream>
#include <algorithm>

/** 
 * \brief Creates an occurrence table
 * Counts number of times a character has appeared at each point. 
 * 
 * \param encoded The string to generate occurrence table from.
 * 
 * \return An occurrence table
 **/
std::vector<std::pair<char, int>> buildOccurrenceTable(const std::string &encoded)
{
    std::vector<std::pair<char, int>> res{};
    std::map<char, int> m;
    for (char c : encoded)
    {
        if (m.find(c) == m.end())
        {
            m[c] = 0;
        }
        std::pair<char, int> t(c, m[c]);
        res.push_back(t);
        ++m[c];
    }
    return res;
}

/**
 * \brief Creates a cumulative table
 * 
 * \param encoded The string to generate cumutlative table from.
 * 
 * \return A cumulative table
 * 
**/
std::map<char, int> buildCTable(const std::string &encoded)
{
    std::map<char, int> cmap{};
    for (char c : encoded)
    {
        if (cmap.find(c) == cmap.end())
        {
            cmap[c] = 1;
        }
        else
        {
            ++cmap[c];
        }
    }
    std::set<char> orderedChar(encoded.begin(), encoded.end());
    std::map<char, int> res{};
    int pre = 0;
    for (char c : orderedChar)
    {
        res[c] = pre;
        pre += cmap[c];
    }
    return res;
}

/** 
 * \brief Generates cumulative tables and occurance tables
 **/
void buildTables(std::string &encoded, std::map<char, int> &cTable,
                 std::vector<std::pair<char, int>> &oTable)
{
    std::map<char, int> m;
    std::set<char> L{encoded.begin(), encoded.end()};
    for (auto c : L)
    {
        m[c] = 0;
    }
    for (char c : encoded)
    {
        std::pair<char, int> t(c, m[c]);
        oTable.push_back(t);
        ++m[c];
    }

    //std::sort(encoded.begin(), encoded.end());
    std::set<char> orderedChar(encoded.begin(), encoded.end());
    int pre = 0;
    for (char c : orderedChar)
    {
        cTable[c] = pre;
        pre += m[c];
    }
}

void printCTable(const std::map<char, int> ctable)
{
    for (auto iter = ctable.begin(); iter != ctable.end(); ++iter)
    {
        std::cout << iter->first << " | " << iter->second << std::endl;
    }
}

void printOccurrenceTable(std::vector<std::pair<char, int>> otable)
{
    int i = 0;
    for (auto t : otable)
    {
        std::cout << i << " | " << t.first << " | " << t.second << std::endl;
        ++i;
    }
}

int occurrenceSearch(const char c, const int match, const std::vector<std::pair<char, int>> &table)
{
    int i = 0;
    for (auto t : table)
    {
        if (t.first == c && t.second == match)
        {
            return i;
        }
    }
    return -1;
}

int startHere(const std::vector<char> &Last, std::vector<int> &oTable)
{
    for (int i = 0; i < Last.size(); ++i)
    {
        if (Last[i] == SEARCH)
        {
            return oTable[i];
        }
    }
    return -1;
}
