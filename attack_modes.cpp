#include "attack_modes.h"



void MaskBasedBruteForceAttack::Start(const std::set<std::string>& hashSet, const AlgorithmHandler& currentAlgorithm, const Range& range, const AmSpecificParams& params) 
{
    int passwordLength = params.alphabets.size();
    char* currentPermutation = new char[passwordLength];
    currentPermutation[passwordLength] = '\0';

    size_t* period = new size_t[passwordLength];
    period[passwordLength - 1] = 1;

    for (int i = passwordLength - 2; i >= 0; i--)
        period[i] = period[i + 1] * params.alphabets[i + 1].length();

    size_t startNumber = 0;
    size_t endNumber = 0;

    if (range.StartNotSet() && range.EndNotSet())
    {
        std::string finalPermutation;
        for (auto& alphabet : params.alphabets)
            finalPermutation += alphabet[alphabet.length() - 1];

        endNumber = CalculatePermutationNumber(finalPermutation, period, params.alphabets);
    }
    else if (range.EndNotSet() && !range.StartNotSet())
    {
        startNumber = CalculatePermutationNumber(range.GetStartPermutation(), period, params.alphabets);
        std::string finalPermutation;
        for (auto& alphabet : params.alphabets)
            finalPermutation += alphabet[alphabet.length() - 1];

        endNumber = CalculatePermutationNumber(finalPermutation, period, params.alphabets);
    }
    else if (range.StartNotSet() && !range.EndNotSet())
    {
        endNumber = CalculatePermutationNumber(range.GetEndPermutation(), period, params.alphabets);
    }
    else
    {
        startNumber = CalculatePermutationNumber(range.GetStartPermutation(), period, params.alphabets);
        endNumber = CalculatePermutationNumber(range.GetEndPermutation(), period, params.alphabets);
    }


    for (size_t i = startNumber; i < endNumber + 1; i++)
    {
        for (size_t j = 0; j < passwordLength; j++)
        {
            int t = i / period[j];
            int r = t % params.alphabets[j].length();

            currentPermutation[j] = params.alphabets[j][r];
        }
        std::string currentHash = currentAlgorithm.HashPermutation(currentPermutation);
        //std::cout << currentPermutation << std::endl;

        if (hashSet.contains(currentHash))
        {
            std::cout << currentHash << ": " << currentPermutation << std::endl;
           // m_CrackedPasswords.push_back({ currentHash, currentPermutation });
        }
    }

    delete[] currentPermutation;
    delete[] period;
}

std::vector<Range> MaskBasedBruteForceAttack::SubdivideRange(const Range& initialRange, const AmSpecificParams& params, int numberOfDivisions)
{
    std::vector<Range> ranges;
    ranges.emplace_back("aaaaaa", "dgnaaa");
    ranges.emplace_back("dgnaaa", "gnaaaa");
    ranges.emplace_back("gnaaaa", "jtnaaa");
    ranges.emplace_back("jtnaaa", "naaaaa");
    ranges.emplace_back("naaaaa", "qgmzzs");
    ranges.emplace_back("qgmzzs", "tnaaaa");
    ranges.emplace_back("tnaaaa", "wtnaai");
    ranges.emplace_back("wtnaai", "zzzzzz");

    return ranges;
}


size_t MaskBasedBruteForceAttack::CalculatePermutationNumber(const std::string& permutation, size_t period[], const std::vector<std::string>& alphabets)
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



#if 0

void BruteForceRec(const std::string& alphabet, size_t maxPasswordLength, const std::string& currentPermutation, const std::set<std::string>& hashSet, 
                         std::vector<hash_password_pair>& crackedPasswords, const std::string endPermutation, bool& reachedEndPermutation, AlgorithmHandler& currentAlgorithm)
{
    if (reachedEndPermutation)
        return;

    if (currentPermutation == endPermutation && endPermutation != "")
        reachedEndPermutation = true;

    //std::cout << currentPermutation << ::std::endl;
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
                                                                   const Range& range)
{
    std::vector<hash_password_pair> crackedPasswords;

    bool reachedEndPermutation = false;

    if (range.StartNotSet() && range.EndNotSet())
    {
        BruteForceRec(alphabet, maxLength, range.GetStartPermutation(), hashSet, crackedPasswords, range.GetEndPermutation(), reachedEndPermutation, currentAlgorithm);
    }
    else
    {
        for (int i = range.GetStartPermutation().length() - 1; i >= 0; i--)
        {
            if (range.GetStartPermutation()[i] == alphabet[alphabet.length() - 1] && i != 0 && i != range.GetStartPermutation().length() - 1)
                continue;

            for (int j = alphabet.find(range.GetStartPermutation()[i]); j < alphabet.length(); j++)
            {
                std::string recStart = range.GetStartPermutation();
                recStart[i] = alphabet[j];
                if (recStart[i] == range.GetStartPermutation()[i] && i != range.GetStartPermutation().length() - 1)
                    continue;
                recStart = recStart.substr(0, i + 1);
                BruteForceRec(alphabet, maxLength, recStart, hashSet, crackedPasswords, range.GetEndPermutation(), reachedEndPermutation, currentAlgorithm);
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


std::vector<std::tuple<std::string, std::string>> MaskBasedBruteForce(const std::set<std::string>& hashSet, const std::vector<std::string>& alphabets, 
                                                                            const AlgorithmHandler& currentAlgorithm, const Range& range)
{
    std::vector<hash_password_pair> crackedPasswords;

    int passwordLength = alphabets.size();
    char* currentPermutation = new char[passwordLength];
    currentPermutation[passwordLength] = '\0';

    size_t* period = new size_t[passwordLength];
    period[passwordLength - 1] = 1;

    for (int i = passwordLength - 2; i >= 0; i--)
        period[i] = period[i + 1] * alphabets[i + 1].length();

    size_t startNumber = 0;
    size_t endNumber = 0;

    if (range.StartNotSet() && range.EndNotSet())
    {
        std::string finalPermutation;
        for (auto& alphabet : alphabets)
            finalPermutation += alphabet[alphabet.length() - 1];

        endNumber = CalculatePermutationNumber(finalPermutation, period, alphabets);
    }
    else if (range.EndNotSet())
    {
        startNumber = CalculatePermutationNumber(range.GetStartPermutation(), period, alphabets);
        std::string finalPermutation;
        for (auto& alphabet : alphabets)
            finalPermutation += alphabet[alphabet.length() - 1];

        endNumber = CalculatePermutationNumber(finalPermutation, period, alphabets);
    }
    else if (range.StartNotSet())
    {
        endNumber = CalculatePermutationNumber(range.GetEndPermutation(), period, alphabets);
    }
    else
        startNumber = CalculatePermutationNumber(range.GetStartPermutation(), period, alphabets);
        endNumber = CalculatePermutationNumber(range.GetEndPermutation(), period, alphabets);


    for (size_t i = startNumber; i < endNumber + 1; i++)
    {
        for (size_t j = 0; j < passwordLength; j++)
        {
            int t = i / period[j];
            int r = t % alphabets[j].length();

            currentPermutation[j] = alphabets[j][r];
        }
        std::string currentHash = currentAlgorithm.HashPermutation(currentPermutation);
        //std::cout << currentPermutation << std::endl;

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


std::pair<int, int> FindIndexRange(const std::vector<std::string> dictionary, const Range& range)
{
    int startIndex = 0;
    int endIndex = dictionary.size() - 1;

    for (int i = 0; i < dictionary.size(); i++)
    {
        if (dictionary[i] == range.GetStartPermutation() && !range.StartNotSet())
            startIndex = i;
        else if (dictionary[i] == range.GetEndPermutation() && !range.EndNotSet())
            endIndex = i;
    }

    return { startIndex, endIndex };
}


std::vector<hash_password_pair> DictionaryAttack(const std::set<std::string>& hashSet, const std::vector<std::string>& dictionary, AlgorithmHandler& currentAlgorithm, const Range& range)
{
    std::vector<hash_password_pair> crackedPasswords;

    auto[startIndex, endIndex] = FindIndexRange(dictionary, range);

    for (int i = startIndex; i <= endIndex; i++)
    {
        //std::cout << dictionary[i] << std::endl;
        std::string currentHash = currentAlgorithm.HashPermutation(dictionary[i]);
        if (hashSet.contains(currentHash))
        {
            std::cout << currentHash << ": " << dictionary[i] << std::endl;
            crackedPasswords.push_back({ currentHash, dictionary[i]});
        }
    }
    return crackedPasswords;
}
#endif 