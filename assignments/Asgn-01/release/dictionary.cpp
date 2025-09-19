#include "dictionary.h"
#include "settings.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cctype>

namespace seneca {

static inline bool iequals(const std::string& a, const std::string& b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i)
        if (std::tolower(static_cast<unsigned char>(a[i])) !=
            std::tolower(static_cast<unsigned char>(b[i])))
            return false;
    return true;
}

std::string Dictionary::trim(const std::string& s) {
    auto b = s.find_first_not_of(" \t\r\n");
    auto e = s.find_last_not_of(" \t\r\n");
    if (b == std::string::npos) return "";
    return s.substr(b, e - b + 1);
}

PartOfSpeech Dictionary::posFrom(const std::string& rawIn) {
    std::string raw = trim(rawIn);
    if (raw == "n." || raw == "n. pl.")          return PartOfSpeech::Noun;
    if (raw == "adv.")                           return PartOfSpeech::Adverb;
    if (raw == "a.")                             return PartOfSpeech::Adjective;
    if (raw == "v." || raw == "v. i." ||
        raw == "v. t." || raw == "v. t. & i.")   return PartOfSpeech::Verb;
    if (raw == "prep.")                          return PartOfSpeech::Preposition;
    if (raw == "pron.")                          return PartOfSpeech::Pronoun;
    if (raw == "conj.")                          return PartOfSpeech::Conjunction;
    if (raw == "interj.")                        return PartOfSpeech::Interjection;
    return PartOfSpeech::Unknown;
}

// constructor: load from file
Dictionary::Dictionary(const char* filename) {
    std::ifstream fin(filename);
    if (!fin) {
        m_words = nullptr;
        m_size = 0;
        return;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(fin, line)) {
        if (!line.empty()) lines.push_back(line);
    }
    fin.close();

    m_size = lines.size();
    m_words = (m_size > 0) ? new Word[m_size] : nullptr;

    for (std::size_t i = 0; i < m_size; ++i) {
        const std::string& s = lines[i];
        auto c1 = s.find(',');
        if (c1 == std::string::npos) {
            m_words[i].m_word = trim(s);
            m_words[i].m_pos = PartOfSpeech::Unknown;
            m_words[i].m_definition.clear();
            continue;
        }
        auto c2 = s.find(',', c1 + 1);

        std::string word   = trim(s.substr(0, c1));
        std::string posRaw = (c2 == std::string::npos)
                                ? trim(s.substr(c1 + 1))
                                : trim(s.substr(c1 + 1, c2 - (c1 + 1)));
        std::string def    = (c2 == std::string::npos)
                                ? std::string{}
                                : trim(s.substr(c2 + 1));

        m_words[i].m_word = word;
        m_words[i].m_pos = posFrom(posRaw);
        m_words[i].m_definition = def;
    }
}

// destructor
Dictionary::~Dictionary() {
    delete[] m_words;
}

// copy ctor
Dictionary::Dictionary(const Dictionary& other) {
    m_size = other.m_size;
    if (m_size > 0) {
        m_words = new Word[m_size];
        for (std::size_t i = 0; i < m_size; ++i) {
            m_words[i] = other.m_words[i];
        }
    }
}

// copy assign
Dictionary& Dictionary::operator=(const Dictionary& other) {
    if (this != &other) {
        delete[] m_words;
        m_size = other.m_size;
        if (m_size > 0) {
            m_words = new Word[m_size];
            for (std::size_t i = 0; i < m_size; ++i) {
                m_words[i] = other.m_words[i];
            }
        } else {
            m_words = nullptr;
        }
    }
    return *this;
}

// move ctor
Dictionary::Dictionary(Dictionary&& other) noexcept
    : m_words(other.m_words), m_size(other.m_size) {
    other.m_words = nullptr;
    other.m_size = 0;
}

// move assign
Dictionary& Dictionary::operator=(Dictionary&& other) noexcept {
    if (this != &other) {
        delete[] m_words;
        m_words = other.m_words;
        m_size = other.m_size;
        other.m_words = nullptr;
        other.m_size = 0;
    }
    return *this;
}

// output PartOfSpeech
std::ostream& operator<<(std::ostream& os, PartOfSpeech p) {
    switch (p) {
        case PartOfSpeech::Noun:         os << "noun"; break;
        case PartOfSpeech::Pronoun:      os << "pronoun"; break;
        case PartOfSpeech::Adjective:    os << "adjective"; break;
        case PartOfSpeech::Adverb:       os << "adverb"; break;
        case PartOfSpeech::Verb:         os << "verb"; break;
        case PartOfSpeech::Preposition:  os << "preposition"; break;
        case PartOfSpeech::Conjunction:  os << "conjunction"; break;
        case PartOfSpeech::Interjection: os << "interjection"; break;
        default:                         os << "unknown"; break;
    }
    return os;
}


// searchWord
void Dictionary::searchWord(const char* w) const {
    std::string query = w ? w : "";
    if (query.empty()) {
        std::cout << "Word '' was not found in the dictionary.\n";
        return;
    }

    bool foundAny = false;
    for (std::size_t i = 0; i < m_size; ++i) {
        if (!iequals(m_words[i].m_word, query)) continue;

        const std::string& WORD = m_words[i].m_word;
        std::string indent(WORD.size(), ' ');

        auto printOne = [&](bool firstLine, const Word& rec) {
            if (firstLine) {
                std::cout << WORD << " - ";
            } else {
                std::cout << indent << " - ";
            }
            if (g_settings.m_verbose && rec.m_pos != PartOfSpeech::Unknown) {
                std::cout << "(" << rec.m_pos << ") ";
            }
            std::cout << rec.m_definition << '\n';
        };

        printOne(true, m_words[i]);
        foundAny = true;
        if (!g_settings.m_show_all) return;

        for (std::size_t j = i + 1; j < m_size; ++j) {
            if (iequals(m_words[j].m_word, query)) {
                printOne(false, m_words[j]);
            }
        }
        return;
    }

    if (!foundAny) {
        std::cout << "Word '" << query << "' was not found in the dictionary.\n";
    }
}

} // namespace seneca
