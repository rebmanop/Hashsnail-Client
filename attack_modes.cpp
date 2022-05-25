#include "attack_modes.h"


#define PRINT_MUTATIONS 0


std::vector<hash_password_pair> AttackMode::GetCrackedPasswords() const
{
    return m_CrackedPasswords;
}


BruteForceAttack::BruteForceAttack(std::set<std::string>&& hashSet, size_t maxPassLength, const std::string& alphabet, const Range& range, std::shared_ptr<AlgorithmHandler> algorithm)
{
    m_HashSet = std::move(hashSet);
    m_MaxPasswordLength = maxPassLength;
    m_Algorithm = algorithm;
    m_Alphabet = alphabet;
    m_InitialRange = range;
}


MaskBasedBruteForceAttack::MaskBasedBruteForceAttack(std::set<std::string>&& hashSet, std::vector<std::string>&& alphabets, const Range& range, std::shared_ptr<AlgorithmHandler> algorithm)
{
    m_HashSet = std::move(hashSet);
    m_Alphabets = std::move(alphabets);
    m_InitialRange = range;
    m_Algorithm = algorithm;
}


DictionaryAttack::DictionaryAttack(std::set<std::string>&& hashSet, std::vector<std::string>&& dictionary, const Range& range, std::shared_ptr<AlgorithmHandler> algorithm)
{
    m_HashSet = std::move(hashSet);
    m_Dictionary = std::move(dictionary);
    m_InitialRange = range;
    m_Algorithm = algorithm;
}



void BruteForceAttack::StartThread(const Range& range)
{
    bool reachedEndPermutation = false;


    if (range.StartNotSet() && range.EndNotSet())
    {
        BruteForceRec(range.GetStartPermutation(), range.GetEndPermutation(), reachedEndPermutation);
    }
    else
    {
        for (int i = range.GetStartPermutation().length() - 1; i >= 0; i--)
        {
            if (range.GetStartPermutation()[i] == m_Alphabet[m_Alphabet.length() - 1] && i != 0 && i != range.GetStartPermutation().length() - 1)
                continue;

            for (int j = m_Alphabet.find(range.GetStartPermutation()[i]); j < m_Alphabet.length(); j++)
            {
                std::string recStart = range.GetStartPermutation();
                recStart[i] = m_Alphabet[j];
                if (recStart[i] == range.GetStartPermutation()[i] && i != range.GetStartPermutation().length() - 1)
                    continue;
                recStart = recStart.substr(0, i + 1);
                BruteForceRec(recStart, range.GetEndPermutation(), reachedEndPermutation);
            }
        }
    }
}



void MaskBasedBruteForceAttack::StartThread(const Range& range)
{
    int passwordLength = m_Alphabets.size();
    char* currentPermutation = new char[passwordLength + 1];
    currentPermutation[passwordLength] = '\0';

   long long* period = new long long[passwordLength];
    period[passwordLength - 1] = 1;

    for (int i = passwordLength - 2; i >= 0; i--)
        period[i] = period[i + 1] *  m_Alphabets[i + 1].length();


    long long startNumber = range.GetStartPermutationNumber();
    long long endNumber = range.GetEndPermutationNumber();
   
    for (long long i = startNumber; i < endNumber + 1; i++)
    {
        for (long long j = 0; j < passwordLength; j++)
        {
            long long t = i / period[j];
            long long r = t % m_Alphabets[j].length();

            currentPermutation[j] = m_Alphabets[j][r];
        }
        std::string currentHash = m_Algorithm->HashPermutation(currentPermutation);
        
        if (PRINT_MUTATIONS)
            std::cout << currentPermutation << std::endl;

        if (m_HashSet.contains(currentHash))
        {
            std::lock_guard<std::mutex> lock(m_CrackedPasswordsMutex);
            spdlog::trace("{}: {}", currentHash, currentPermutation);
            m_CrackedPasswords.push_back({ currentHash, currentPermutation });
        }
    }


    delete[] currentPermutation;
    delete[] period;
}



void DictionaryAttack::StartThread(const Range& range)
{

    for (long long i = range.GetStartPermutationNumber(); i <= range.GetEndPermutationNumber(); i++)
    {
         if (PRINT_MUTATIONS)
            std::cout << m_Dictionary[i] << std::endl;

        std::string currentHash = m_Algorithm->HashPermutation(m_Dictionary[i]);
        if (m_HashSet.contains(currentHash))
        {
            std::lock_guard<std::mutex> lock(m_CrackedPasswordsMutex);
            spdlog::trace("{}: {}", currentHash, m_Dictionary[i]);
            m_CrackedPasswords.push_back({ currentHash, m_Dictionary[i] });
        }
    }
}



std::vector<Range> BruteForceAttack::SubdivideRange(int numberOfDivisions)
{
    std::vector<Range> ranges;
    ranges.emplace_back("", "");
    return ranges;
}



std::vector<Range> MaskBasedBruteForceAttack::SubdivideRange(int numberOfDivisions)
{
    size_t passwordLength = m_Alphabets.size();

    long long* period = new long long[passwordLength];
    period[passwordLength - 1] = 1;

    for (int i = passwordLength - 2; i >= 0; i--)
        period[i] = period[i + 1] * m_Alphabets[i + 1].length();

    long long startPermutationNumber = CalculatePermutationNumber(m_InitialRange.GetStartPermutation(), period);
    long long endPermutationNumber = 0;

    if (m_InitialRange.EndNotSet())
    {
        std::string finalPermutation;
        for (auto& alphabet : m_Alphabets)
            finalPermutation += alphabet[alphabet.length() - 1];

        endPermutationNumber = CalculatePermutationNumber(finalPermutation, period);
    }
    else
        endPermutationNumber = CalculatePermutationNumber(m_InitialRange.GetEndPermutation(), period);

    std::vector<Range> ranges;

    long long passwordsToHash = endPermutationNumber - startPermutationNumber + 1;
    long long hashesPerThread = passwordsToHash / numberOfDivisions;

    long long tempIndex;
    for (int i = 1; i <= numberOfDivisions; i++)
    {
        tempIndex = startPermutationNumber + hashesPerThread;
        if (i == numberOfDivisions && tempIndex < endPermutationNumber)
            ranges.push_back(Range(startPermutationNumber, endPermutationNumber));
        else
            ranges.push_back(Range(startPermutationNumber, tempIndex));

        startPermutationNumber = tempIndex;
    }
     
    delete[] period;

    return ranges;
}



std::vector<Range> DictionaryAttack::SubdivideRange(int numberOfDivisions)
{
    std::vector<Range> ranges;

    auto[startIndex, endIndex] = FindIndexRange(m_InitialRange);
    
    int passwordsToHash = endIndex - startIndex + 1;
    int hashesPerThread = passwordsToHash / numberOfDivisions;

    int tempIndex;
    for (int i = 1; i <= numberOfDivisions; i++)
    {
        tempIndex = startIndex + hashesPerThread;
        if (i == numberOfDivisions && tempIndex <= endIndex)
            ranges.push_back(Range(startIndex, endIndex));
        else
            ranges.push_back(Range(startIndex, tempIndex));

        startIndex = tempIndex;
    }
    return ranges;
}



void BruteForceAttack::BruteForceRec(const std::string& currentPermutation, const std::string endPermutation, bool& reachedEndPermutation)
{
  if (reachedEndPermutation)
        return;

    if (currentPermutation == endPermutation && endPermutation != "")
        reachedEndPermutation = true;

    if (PRINT_MUTATIONS)
        std::cout << currentPermutation << ::std::endl;
    
    std::string currentHash = m_Algorithm->HashPermutation(currentPermutation);

    if (m_HashSet.contains(currentHash))
    {
        std::lock_guard<std::mutex> lock(m_CrackedPasswordsMutex);
        spdlog::trace("{}: {}", currentHash, currentPermutation);

        m_CrackedPasswords.push_back({ currentHash, currentPermutation });
    }

    if (currentPermutation.length() == m_MaxPasswordLength)
        return;
    else
    {
        for (auto c : m_Alphabet)
        {
            std::string nextPermutation = currentPermutation + c;
            BruteForceRec(nextPermutation, endPermutation, reachedEndPermutation);
        }
    }
}



std::pair<int, int> DictionaryAttack::FindIndexRange(const Range& range)
{
    int startIndex = 0;
    int endIndex = m_Dictionary.size() - 1;

    for (int i = 0; i < m_Dictionary.size(); i++)
    {
        if (m_Dictionary[i] == range.GetStartPermutation() && !range.StartNotSet())
            startIndex = i;
        else if (m_Dictionary[i] == range.GetEndPermutation() && !range.EndNotSet())
            endIndex = i;
    }

    return { startIndex, endIndex };
}



long long MaskBasedBruteForceAttack::CalculatePermutationNumber(const std::string& permutation, long long period[])
{
    long long permutationNumber = 0;
    for (size_t i = 0; i < m_Alphabets.size(); i++)
    {
        long long symbolIndex = 0;
        for (size_t j = 0; j < m_Alphabets[i].length(); j++)
        {
            if (m_Alphabets[i][j] == permutation[i])
                symbolIndex = j;
        }
        permutationNumber += period[i] * symbolIndex;
    }
    return permutationNumber;
}