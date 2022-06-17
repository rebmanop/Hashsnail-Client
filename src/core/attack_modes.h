#pragma once
#include <set>

typedef std::pair<std::string, std::string> hash_password_pair;

class AttackMode
{
public:
    virtual void StartThread(const Range& range) = 0;
    virtual std::vector<Range> SubdivideRange(int numberOfDivisions) = 0;
    
    std::vector<hash_password_pair> GetCrackedPasswords() const;

protected:
    std::vector<hash_password_pair> m_CrackedPasswords;
    std::mutex m_CrackedPasswordsMutex;
    std::set<std::string> m_HashSet;
    std::shared_ptr<AlgorithmHandler> m_Algorithm;
    Range m_InitialRange;
};



class BruteForceAttack : public AttackMode
{
public:
    BruteForceAttack(std::set<std::string>&& hashSet, size_t maxPassLength, const std::string& alphabet, const Range& range, std::shared_ptr<AlgorithmHandler> algorithm);

    void StartThread(const Range& range) override;
    std::vector<Range> SubdivideRange(int numberOfDivisions)  override;

private:
    void BruteForceRec(const std::string& currentPermutation, const std::string endPermutation, bool& reachedEndPermutation);

private:
    size_t m_MaxPasswordLength;
    std::string m_Alphabet;
};



class MaskBasedBruteForceAttack : public AttackMode
{
public:
    
    MaskBasedBruteForceAttack(std::set<std::string>&& hashSet, std::vector<std::string>&& alphabets, const Range& range, std::shared_ptr<AlgorithmHandler> algorithm);


    void StartThread(const Range& range) override;
    std::vector<Range> SubdivideRange(int numberOfDivisions)  override;


private:
    long long CalculatePermutationNumber(const std::string& permutation, long long period[]);

private:  
    std::vector<std::string> m_Alphabets;
};



class DictionaryAttack : public AttackMode
{
public:
    DictionaryAttack(std::set<std::string>&& hashSet, std::vector<std::string>&& dictionary, const Range& range, std::shared_ptr<AlgorithmHandler> algorithm);


    void StartThread(const Range& range) override;
    std::vector<Range> SubdivideRange(int numberOfDivisions)  override;


private:
    std::pair<int, int> FindIndexRange(const Range& range);

private:
    std::vector<std::string> m_Dictionary;
};