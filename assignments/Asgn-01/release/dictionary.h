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

    // Rule of five
    ~Dictionary();

    Dictionary(const Dictionary& other);                // deep copy ctor
    Dictionary& operator=(const Dictionary& other);     // deep copy assign
    Dictionary(Dictionary&& other) noexcept;            // move ctor
    Dictionary& operator=(Dictionary&& other) noexcept; // move assign

    void searchWord(const char* word) const;

    std::size_t size() const { return m_size; }
    bool empty() const { return m_size == 0; }
};

// helper: print part of speech
std::ostream& operator<<(std::ostream& os, PartOfSpeech p);

} // namespace seneca

#endif
