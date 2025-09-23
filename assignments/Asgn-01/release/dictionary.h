/*
I declare that this submission is the result of my own work and I only copied the code that my professor provided to complete my assignments. 
This submitted piece of work has not been shared with any other student or 3rd party content provider.
*/

#ifndef SENECA_DICTIONARY_H
#define SENECA_DICTIONARY_H

#include <string>
#include <iostream>
#include <cstddef>

namespace seneca {

enum class PartOfSpeech {
    Unknown,
    Noun,
    Pronoun,
    Adjective,
    Adverb,
    Verb,
    Preposition,
    Conjunction,
    Interjection,
};

struct Word {
    std::string m_word{};
    std::string m_definition{};
    PartOfSpeech m_pos{PartOfSpeech::Unknown};
};

class Dictionary {
    Word* m_words{nullptr};
    std::size_t m_size{0};

    static std::string trim(const std::string& s);
    static PartOfSpeech posFrom(const std::string& raw);

public:
    Dictionary() = default;
    explicit Dictionary(const char* filename);

    ~Dictionary();

    Dictionary(const Dictionary& other);                
    Dictionary& operator=(const Dictionary& other);     
    Dictionary(Dictionary&& other) noexcept;           
    Dictionary& operator=(Dictionary&& other) noexcept; 

    void searchWord(const char* word) const;

    std::size_t size() const { return m_size; }
    bool empty() const { return m_size == 0; }
};

std::ostream& operator<<(std::ostream& os, PartOfSpeech p);

} // namespace seneca

#endif
