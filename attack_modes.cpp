#include "attack_modes.h"


void BruteForceRec(const std::string& alphabet, size_t maxPasswordLength, const std::string& currentPermutation, const std::set<std::string>& hashSet, 
                         std::vector<std::tuple<std::string, std::string>>& crackedPasswords, const std::string endPermutation, bool& reachedEndPermutation, AlgorithmHandler& currentAlgorithm)
{
    if (reachedEndPermutation)
        return;

    if (currentPermutation == endPermutation && endPermutation != "")
        reachedEndPermutation = true;

    std::string currentHash = currentAlgorithm.HashPermutation(currentPermutation);

    if (hashSet.contains(currentHash))
    {
        std::cout << currentHash << ": " << currentPermutation << std::endl;
        crackedPasswords.push_back({ currentHash, currentPermutation });
    }

    if (currentPermutation.length() == maxPasswordLength)
        return;
    else
    {
        for (auto c : alphabet)
        {
            std::string nextPermutation = currentPermutation + c;
            BruteForceRec(alphabet, maxPasswordLength, nextPermutation, hashSet, crackedPasswords, endPermutation, reachedEndPermutation, currentAlgorithm);
        }
    }
}


std::vector<std::tuple<std::string, std::string>> BruteForce(const std::string& alphabet, size_t maxLength, const std::set<std::string>& hashSet, AlgorithmHandler& currentAlgorithm,
                                                                   std::string startPermutation, std::string endPermutation)
{
    std::vector<std::tuple<std::string, std::string>> crackedPasswords;

    bool reachedEndPermutation = false;

    if (startPermutation == "" && endPermutation == "")
    {
        BruteForceRec(alphabet, maxLength, startPermutation, hashSet, crackedPasswords, endPermutation, reachedEndPermutation, currentAlgorithm);
    }
    else
    {
        for (int i = startPermutation.length() - 1; i >= 0; i--)
        {
            if (startPermutation[i] == alphabet[alphabet.length() - 1] && i != 0 && i != startPermutation.length() - 1)
                continue;

            for (int j = alphabet.find(startPermutation[i]); j < alphabet.length(); j++)
            {
                std::string recStart = startPermutation;
                recStart[i] = alphabet[j];
                if (recStart[i] == startPermutation[i] && i != startPermutation.length() - 1)
                    continue;
                recStart = recStart.substr(0, i + 1);
                BruteForceRec(alphabet, maxLength, recStart, hashSet, crackedPasswords, endPermutation, reachedEndPermutation, currentAlgorithm);
            }
        }
    }
    return crackedPasswords;
}


size_t CalculatePermutationNumber(const std::string& permutation, size_t period[], const std::vector<std::string>& alphabets)
{
    size_t permutationNumber = 0;
    for (size_t i = 0; i < alphabets.size(); i++)
    {
        size_t symbolIndex = 0;
        for (size_t j = 0; j < alphabets[i].length(); j++)
        {
            if (alphabets[i][j] == permutation[i])
                symbolIndex = j;
        }
        permutationNumber += period[i] * symbolIndex;
    }
    return permutationNumber;
}


std::vector<std::tuple<std::string, std::string>> MaskBasedBruteForce(const std::set<std::string>& hashSet, const std::vector<std::string>& alphabets, AlgorithmHandler& currentAlgorithm,
                                                                      const std::string& startPermutation, const std::string& endPermutation)
{
    std::vector<std::tuple<std::string, std::string>> crackedPasswords;

    int passwordLength = alphabets.size();
    char* currentPermutation = new char[passwordLength];
    currentPermutation[passwordLength] = '\0';

    size_t* period = new size_t[passwordLength];
    period[passwordLength - 1] = 1;

    for (int i = passwordLength - 2; i >= 0; i--)
        period[i] = period[i + 1] * alphabets[i + 1].length();

    size_t startNumber = 0;
    size_t endNumber = 0;

    if (startPermutation == "" && endPermutation == "")
    {
        std::string finalPermutation;
        for (auto alphabet : alphabets)
            finalPermutation += alphabet[alphabet.length() - 1];

        endNumber = CalculatePermutationNumber(finalPermutation, period, alphabets);
    }
    else if (endPermutation == "")
    {
        startNumber = CalculatePermutationNumber(startPermutation, period, alphabets);
        std::string finalPermutation;
        for (auto alphabet : alphabets)
            finalPermutation += alphabet[alphabet.length() - 1];

        endNumber = CalculatePermutationNumber(finalPermutation, period, alphabets);
    }
    else
        endNumber = CalculatePermutationNumber(endPermutation, period, alphabets);


    for (size_t i = startNumber; i < endNumber + 1; i++)
    {
        for (size_t j = 0; j < passwordLength; j++)
        {
            int t = i / period[j];
            int r = t % alphabets[j].length();

            currentPermutation[j] = alphabets[j][r];
        }
        std::string currentHash = currentAlgorithm.HashPermutation(currentPermutation);

        if (hashSet.contains(currentHash))
        {
            std::cout << currentHash << ": " << currentPermutation << std::endl;
            crackedPasswords.push_back({ currentHash, currentPermutation });
        }
    }

    delete[] currentPermutation;
    delete[] period;

    return crackedPasswords;
}
