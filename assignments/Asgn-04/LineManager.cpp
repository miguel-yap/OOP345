/* Citation and Sources.... 
Assignment 04
Module: LineManager
FileName: LineManager.cpp
------------------------------------------------------------------
Author: Miguel Yap
StNo: 168049237
Email: myap1@myseneca.ca
Date Completion: Sun, Nov 30, 2025
------------------------------------------------------------------
- Line 29-106 was assisted with the help of ChatGPT.
- Line 109-121 was assisted with the help of ChatGPT.
- Line 124-142 was assisted with the help of ChatGPT.
------------------------------------------------------------------*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include "LineManager.h"
#include "Utilities.h"
#include "Workstation.h"

namespace seneca {

    // The following display function implementation was developed with guidance from ChatGPT.
    LineManager::LineManager(const std::string& file, const std::vector<Workstation*>& stations) {
        std::ifstream f(file);
        if (!f) {
            throw std::string("Unable to open [") + file + "] file.";
        }

        Utilities util;
        std::string record;
        std::vector<std::string> current_station_names;
        std::vector<std::string> next_station_names;

        while (std::getline(f, record)) {
            if (record.empty()) continue;

            size_t next_pos = 0;
            bool more = true;
            std::string current_station_name;
            std::string next_station_name;

            try {
                current_station_name = util.extractToken(record, next_pos, more);
                current_station_names.push_back(current_station_name);

                if (more) {
                    next_station_name = util.extractToken(record, next_pos, more);
                    next_station_names.push_back(next_station_name);
                } else {
                    next_station_names.push_back("");
                }
            } catch (...) {
            }
        }
        f.close();

        for (const auto& name : current_station_names) {
            auto it = std::find_if(stations.begin(), stations.end(),
                                   [&](Workstation* ws) { return ws->getItemName() == name; });
            if (it != stations.end()) {
                m_activeLine.push_back(*it);
            }
        }

        for (size_t i = 0; i < current_station_names.size(); ++i) {
            if (!next_station_names[i].empty()) {
                
                auto current_ws_it = std::find_if(stations.begin(), stations.end(), 
                                               [&](Workstation* ws) { return ws->getItemName() == current_station_names[i]; });
                
                auto next_ws_it = std::find_if(stations.begin(), stations.end(), 
                                            [&](Workstation* ws) { return ws->getItemName() == next_station_names[i]; });
                
                if (current_ws_it != stations.end() && next_ws_it != stations.end()) {
                    (*current_ws_it)->setNextStation(*next_ws_it);
                }
            }
        }


        std::vector<Workstation*> next_stations;
        std::for_each(stations.begin(), stations.end(), 
                      [&](Workstation* ws) { 
                          Workstation* next = ws->getNextStation();
                          if (next) {
                              next_stations.push_back(next);
                          }
                      });
        
        auto first_it = std::find_if(stations.begin(), stations.end(), 
                                     [&](Workstation* ws) {
                                         return std::find(next_stations.begin(), next_stations.end(), ws) == next_stations.end();
                                     });

        if (first_it != stations.end()) {
            m_firstStation = *first_it;
        }

        m_cntCustomerOrder = g_pending.size();
    }

    // The following display function implementation was developed with guidance from ChatGPT.
    void LineManager::reorderStations() {
        if (!m_firstStation) return;

        std::vector<Workstation*> reordered_line;
        Workstation* current = m_firstStation;

        while (current) {
            reordered_line.push_back(current);
            current = current->getNextStation();
        }

        m_activeLine = reordered_line;
    }

    // The following display function implementation was developed with guidance from ChatGPT.
    bool LineManager::run(std::ostream& os) {
        static size_t iteration_count = 0;
        iteration_count++;

        os << "Line Manager Iteration: " << iteration_count << std::endl;

        if (!g_pending.empty()) {
            *m_firstStation += std::move(g_pending.front());
            g_pending.pop_front();
        }

        std::for_each(m_activeLine.begin(), m_activeLine.end(), 
                      [&os](Workstation* ws) { ws->fill(os); });

        std::for_each(m_activeLine.begin(), m_activeLine.end(), 
                      [](Workstation* ws) { ws->attemptToMoveOrder(); });

        return m_cntCustomerOrder == (g_completed.size() + g_incomplete.size());
    }

    void LineManager::display(std::ostream& os) const {
        std::for_each(m_activeLine.begin(), m_activeLine.end(), 
                      [&os](Workstation* ws) { ws->display(os); });
    }
}