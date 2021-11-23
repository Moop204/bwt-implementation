#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <map>
#include <set>
#include <fstream>
#include <ios>
#include "bwt.h"

static bool DEBUG = false;
static bool GENERAL = true;
static bool TIME = true;

static std::set<char> ALPHABET = {'\n', 'A', 'C', 'G', 'T'};
static int BUFSIZE = 4;

/** 
 * Cumulative frequency table
 **/
static std::map<char, int> CTable;

/**
 * \brief Increases cumulative table according to a set order
 **/
void updateCumulativeTable(char c)
{
  switch (c)
  {
  case '\n':
    ++CTable['A'];
  case 'A':
    ++CTable['C'];
  case 'C':
    ++CTable['G'];
  case 'G':
    ++CTable['T'];
  case 'T':
  default:
    break;
  }
}

/**
      *   \brief Parses string input.
      *
      *   Generates last column table and otable which counts how many duplicates of the character have been found so far.
      *   
      *   \param Last Stores the last table which holds last character of all rows.
      *   \param oTable How frequently a character at an index has been found before.
      *   \param fname Name of the file to be decoded.
      *   \return Number of characters in the opened file.
      *
      **/
int takeInput(std::vector<char> &Last, std::vector<int> &oTable, std::ifstream &stream)
{
  char tmp;
  int size = 0;

  char *buffer = new char[1024];

  std::map<char, int> m;
  m['\n'] = 0;
  m['A'] = 0;
  m['C'] = 0;
  m['G'] = 0;
  m['T'] = 0;

  while (stream.read(buffer, 1))
  {
    for (int i = 0; i < 1; ++i)
    {

      Last.push_back(buffer[i]);
      oTable.push_back(m[buffer[i]]);
      if (buffer[i] == '\n')
      {
        size = i;
      }
      updateCumulativeTable(buffer[i]);
      ++m[buffer[i]];
    }
  }

  delete[] buffer;
  stream.close();
  return size;
}

/** 
 *   \brief Produces cumulative frequency table.
 *
 *   Generates cumulative frequency table from frequency table. Needed for TODO: Algorithm.
 *   
 *   \param m Frequency table.
 *   \return Cumulative frequency table.
 *
**/
std::map<char, int> buildCTable(std::map<char, int> &m)
{
  std::map<char, int> cTable;
  int pre = 0;
  for (char c : ALPHABET)
  {
    cTable[c] = pre;
    pre += m[c];
  }
  return cTable;
}

void writingToFile(int pos, int total, std::ofstream stream, char *buf)
{
  stream.seekp(total - pos - 1);
  stream << buf;
}

/** 
 * \brief Initialises cumulative frequency table
 **/
void cInit()
{
  CTable['\n'] = 0;
  CTable['A'] = 0;
  CTable['C'] = 0;
  CTable['G'] = 0;
  CTable['T'] = 0;
}

int main(int argc, char *argv[])
{
  cInit();
  std::string fname = argv[1];
  std::vector<char> Last;
  std::vector<int> oTable;

  if (TIME)
    std::cout << "Taking input..." << std::endl;

  std::ifstream sin(fname, std::ios_base::in);
  if (!sin.is_open())
  {
    std::cout << "Error: Could not open file " << fname << std::endl;
    return 1;
  }
  return 0;

  // Heaviest wait
  int pos = takeInput(Last, oTable, sin);

  if (TIME)
    std::cout << "Calculate oShift..." << std::endl;

  int oShift = pos;
  int cShift = 0;

  if (TIME)
    std::cout << "Backwards BWT..." << std::endl;
  // Second heaviest wait

  std::ofstream ofile(argv[2]);
  ofile.unsetf(std::ios_base::skipws);

  char c = '\n';
  int bLoc = 1;
  std::string buf;
  buf.resize(BUFSIZE, 'K');
  buf[0] = '\n';
  if (DEBUG)
    std::cout << c << " AT " << bLoc << std::endl;
  for (int i = 0; i < Last.size() - 1; ++i)
  {
    c = Last[oShift + cShift];
    oShift = oTable[oShift + cShift];
    cShift = CTable[c];
    buf[bLoc] = c;
    if (DEBUG)
      std::cout << c << " AT " << bLoc << std::endl;
    bLoc = (bLoc + 1) % BUFSIZE;

    if (bLoc == 0)
    {
      ofile.seekp(Last.size() - i - 2);
      std::reverse(buf.begin(), buf.end());
      if (DEBUG)
      {
        std::cout << "AT POS " << Last.size() - i - 2 << std::endl;
        std::cout << "I IS " << i << std::endl;
        std::cout << "SEG " << buf << std::endl;
      }
      ofile << buf;
    }
  }

  if (bLoc != 0)
  {
    ofile.seekp(0);
    std::string tmp = buf.substr(0, bLoc);
    std::reverse(tmp.begin(), tmp.end());
    if (DEBUG)
      std::cout << "LAST ONE: " << bLoc << " IS " << tmp << " SIZE: " << tmp.size() << std::endl;
    ofile << tmp;
  }

  ofile.close();
  return 0;
}
