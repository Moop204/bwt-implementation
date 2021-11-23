#include <map>
#include <vector>

#define SEARCH '\n'

void buildTables(std::string &encoded, std::map<char, int> &cTable,
                 std::vector<std::pair<char, int>> &oTable);
void printCTable(const std::map<char, int> ctable);
void printOccurrenceTable(std::vector<std::pair<char, int>> otable);
int occurrenceSearch(const char c, const int match, const std::vector<std::pair<char, int>> &table);
int startHere(const std::vector<char> &Last, std::vector<int> &oTable);
