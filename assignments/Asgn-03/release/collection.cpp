/* Citation and Sources.... 
Assignment 03
Module: collection
FileName: collection.cpp
------------------------------------------------------------------
Author: Miguel Yap
StNo: 168049237
Email: myap1@myseneca.ca
Date Completion: Sun, Nov 16, 2025
------------------------------------------------------------------
- Line 47-71 was assisted with the help of ChatGPT.
- Line 75-85 was assisted with the help of ChatGPT.
- Line 89-99 was assisted with the help of ChatGPT.
- Line 103-123 was assisted with the help of ChatGPT.
- Line 127-147 was assisted with the help of ChatGPT.
------------------------------------------------------------------*/
#include <stdexcept>
#include <numeric>
#include <functional>
#include "collection.h"

namespace seneca
{
    Collection::Collection(const std::string& name)
        : m_name{name}
    {
    }

    Collection::~Collection()
    {
        for (auto ptr : m_items)
            delete ptr;
    }

    const std::string& Collection::name() const
    {
        return m_name;
    }

    size_t Collection::size() const
    {
        return m_items.size();
    }

    void Collection::setObserver(void (*observer)(const Collection&, const MediaItem&))
    {
        m_observer = observer;
    }

    // The following display function implementation was developed with guidance from ChatGPT.
    Collection& Collection::operator+=(MediaItem* item)
    {
        if (!item)
            return *this;

        auto it = std::find_if(
            m_items.begin(), m_items.end(),
            [t = item->getTitle()](MediaItem* existing)
            {
                return existing && existing->getTitle() == t;
            });

        if (it == m_items.end())
        {
            m_items.push_back(item);
            if (m_observer)
                m_observer(*this, *item);
        }
        else
        {
            delete item; // not added – avoid leak
        }

        return *this;
    }

    // The following display function implementation was developed with guidance from ChatGPT.
    MediaItem* Collection::operator[](size_t idx) const
    {
        if (idx >= m_items.size())
        {
            std::string msg = "Bad index [" + std::to_string(idx)
                + "]. Collection has [" + std::to_string(m_items.size())
                + "] items.";
            throw std::out_of_range(msg);
        }
        return m_items[idx];
    }

    // The following display function implementation was developed with guidance from ChatGPT.
    MediaItem* Collection::operator[](const std::string& title) const
    {
        auto it = std::find_if(
            m_items.begin(), m_items.end(),
            [t = title](MediaItem* item)
            {
                return item && item->getTitle() == t;
            });

        return (it != m_items.end()) ? *it : nullptr;
    }

    // The following display function implementation was developed with guidance from ChatGPT.
    void Collection::removeQuotes()
    {
        std::for_each(
            m_items.begin(), m_items.end(),
            [](MediaItem* item)
            {
                if (!item) return;

                auto strip = [](std::string s)
                {
                    if (!s.empty() && s.front() == '"')
                        s.erase(0, 1);
                    if (!s.empty() && s.back() == '"')
                        s.pop_back();
                    return s;
                };

                item->setTitle(strip(item->getTitle()));
                item->setSummary(strip(item->getSummary()));
            });
    }

    // The following display function implementation was developed with guidance from ChatGPT.
    void Collection::sort(const std::string& field)
    {
        if (field == "title")
        {
            std::sort(
                m_items.begin(), m_items.end(),
                [](MediaItem* a, MediaItem* b)
                {
                    return a->getTitle() < b->getTitle();
                });
        }
        else if (field == "year")
        {
            std::sort(
                m_items.begin(), m_items.end(),
                [](MediaItem* a, MediaItem* b)
                {
                    return a->getYear() < b->getYear();
                });
        }
    }

    std::ostream& operator<<(std::ostream& out, const Collection& c)
    {
        for (size_t i = 0; i < c.size(); ++i)
        {
            MediaItem* item = c.m_items[i];
            if (item)
                out << *item;
        }
        return out;
    }
}
