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
static int IBUFSIZE = 8;

static std::map<char, int> CTable;
static std::vector<int> AOcc{};
static std::vector<int> COcc{};
static std::vector<int> GOcc{};
static std::vector<int> TOcc{};

char whatChar(std::ifstream &fp, int pos) {
  fp.seekg(pos, std::ios::beg);
  char buf(1);
  fp.read(&buf, 1);
  return buf; 
}

int takeInput(std::string &fname)
{
  int size = 0;

  std::ifstream sin(fname, std::ios_base::in);
  if (sin.is_open())
  {

    char *buffer = new char[IBUFSIZE];
    bool die = false;

    std::map<char, int> m;
    m['\n'] = 0;
    m['A'] = 0;
    m['C'] = 0;
    m['G'] = 0;
    m['T'] = 0;

    int aCount = 0;
    int cCount = 0;
    int gCount = 0;
    int tCount = 0;

    while (sin.read(buffer, IBUFSIZE))
    {
      for (int i = 0; i < IBUFSIZE; ++i)
      {
        switch (buffer[i]) {
        case 'A':
          ++aCount;
          break;
        case 'C':
          ++cCount;
          break;
        case 'G':
          ++gCount;
          break;
        case 'T':
          ++tCount;
          break;
        case '\n':
          break;
        default:
          die=true;
        }

        switch (buffer[i])
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
          break;
        default:
          die=true;
        }
        ++m[buffer[i]];
        if(size%IBUFSIZE==0 || die){
          AOcc.push_back(m['A']);
          COcc.push_back(m['C']);
          GOcc.push_back(m['G']);
          TOcc.push_back(m['T']);
          if(DEBUG) {
            std::cout << size << " A: "<<m['A']<<" C: "<<m['C']<<" G: "<<m['G']<<" T: "<<m['T']<<std::endl;
          }
        }
        if(die) break;
        ++size;
      }
      
      if (die)
        break;
    }

    delete[] buffer;
    sin.close();
  }
  else
  {
    std::cerr << "Cannot open file:" << fname << std::endl;
  }

  return size;
}

void writingToFile(int pos, int total, std::ofstream stream, char *buf)
{
  stream.seekp(total - pos - 1);
  stream << buf;
}

void cInit()
{
  CTable['\n'] = 0;
  CTable['A'] = 0;
  CTable['C'] = 0;
  CTable['G'] = 0;
  CTable['T'] = 0;
}

char nextChar(char c) {
  switch(c) {
    case '\n': 
      return 'A';
    case 'A': 
      return 'C';
    case 'C': 
      return 'G';
    case 'G': 
      return 'T';
    default:
      return 'm';
  }
}

int occLookup(char c, int occIdx) {
  switch(c) {
    case '\n': 
      return -2;
    case 'A': 
      return AOcc[occIdx];
    case 'C': 
      return COcc[occIdx];
    case 'G': 
      return GOcc[occIdx];
    case 'T': 
      return TOcc[occIdx];
  }
  return -1;
}

int occSearch(std::ifstream &fp, char cz, int loc, int size) {
  if(loc >= size) {
    loc = size-1;
  }
  int occIdx = loc/IBUFSIZE;
  int ret = occLookup(cz, occIdx);
  if(ret == -2) return 0;

  int start = occIdx*IBUFSIZE;           
  int diff = loc - start;                
  for(int i=1; (i<=diff) && (start+i<size); ++i) {
    if(whatChar(fp, start+i) == cz) {
      ++ret;
    }
  }
  if(DEBUG) 
  std::cout << "OCC FOR " << cz << " "<< occIdx << " IS " << ret << std::endl;

  return ret-1;
}

void displayCTable() {
    for(auto elm : ALPHABET) {
      std::cout << elm << " = " << CTable[elm] << std::endl;
    }
}


int main(int argc, char *argv[])
{
  cInit();
  std::string fname = argv[1];
  if (TIME)
    std::cout << "Taking input..." << std::endl;
  int size = takeInput(fname);
  std::ifstream test(fname);
  std::cout << whatChar(test, 0) << whatChar(test, 1) <<std::endl;

  if(DEBUG) { 
    displayCTable();
    std::cout << std::endl;

    /*
    int asdIdx = 0;
    auto delMe = Last;
    std::sort(delMe.begin(), delMe.end());
    for (auto elm : Last) {
      auto hi = delMe[asdIdx];
      if(elm == '\n') {
        elm = 'n';
      }
      if(hi == '\n') {
        hi = 'n';
      }
      std::cout << asdIdx << " " <<elm << " " << hi << std::endl; 
      ++asdIdx;
    }
    */
  
  }

  std::string search;
  while (std::getline(std::cin, search))
  {
    int i = search.size()-1;
    char cSearch = search[i];
    int first = CTable[cSearch];
    char cNext = nextChar(cSearch);
    int last;
    if(cNext == 'm') {
      last = size-1;
    } else {
      last = CTable[cNext]-1;
    }
    if(DEBUG) {

      std::cout << "ILooking for: "<<cSearch<<std::endl;
      std::cout << "First: " << first << std::endl;
      std::cout << "Last: " << last << std::endl;
      std::cout << "i: " << i <<std::endl;
    }

    while(first <= last && i > 0) { 

      --i;
      cSearch = search[i];
      int cVal = CTable[cSearch];
      int osF = occSearch(test, cSearch, first-1, size)+1;
      int osL = occSearch(test, cSearch, last, size);
      first = cVal + osF;
      last = cVal + osL;
      if(DEBUG) {
        std::cout << "c: "<<cSearch;
        std::cout << "     First: " << cVal << " + " << osF << " = " << first;
        std::cout << "   Last: " << cVal << " + " << osL << " = " <<last<<std::endl;
      }
    } 
    if(last < first) {
      std::cout<< 0<<std::endl;
    } 
    else 
    {
      std::cout<< last - first +1 <<std::endl; 
    }
  };



}
