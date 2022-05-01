#include <sstream>
#include <fstream>
#include "timer.h"
#include "alphabets.h"
#include "attack_modes.h"
#include "algorithms.h"


int main()
{
   Timer timerBruteForce;
   Timer timerMaskedBruteForce;
   MD5Handler md5Algorithm;
   SHA1Handler sha1Algorithm;


   std::vector<std::string> alphabets;
   alphabets.push_back(alphabetTest);
   alphabets.push_back(alphabetTest);
   alphabets.push_back(alphabetTest);
 

   std::vector<std::tuple<std::string, std::string>> crackedPasswords;
   std::set<std::string> hashSet;


   std::ifstream infile("example0.hash");
   std::string line;
   
   while (std::getline(infile, line))
        hashSet.insert(line);

   
   timerBruteForce.Start();
   crackedPasswords = BruteForce(alphabetL, 6, hashSet, md5Algorithm);
   timerBruteForce.End();
   std::cout << timerBruteForce.GetTime("s")<< "s" << std::endl;
   
   std::cout << "------------------------------------" << std::endl;
   
   timerMaskedBruteForce.Start();
   crackedPasswords = MaskBasedBruteForce(hashSet, alphabets, md5Algorithm);
   timerMaskedBruteForce.End();
   std::cout << timerMaskedBruteForce.GetTime("s") << "s" << std::endl;
   
   std::cout << "done." << std::endl;

   return 0;
}



