#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include "LineManager.h"
#include "Utilities.h"

namespace seneca {
    LineManager::LineManager(const std::string& file, const std::vector<Workstation*>& stations) {
        std::ifstream f(file);
        if (!f) {
            throw std::string("Unable to open [") + file + "] file.";
        }

        Utilities util;
        std::string record;
        std::vector<Workstation*> line_parts; // Holds stations in file order
        std::vector<std::string> next_station_names;

        // --- 1. Load linkage and populate line_parts ---
        while (std::getline(f, record)) {
            if (record.empty()) continue;

            size_t next_pos = 0;
            bool more = true;
            std::string current_station_name;
            std::string next_station_name;

            try {
                // WORKSTATION
                current_station_name = util.extractToken(record, next_pos, more);

                // Find the current station object
                auto current_it = std::find_if(stations.begin(), stations.end(), 
                                               [&](Workstation* ws) { return ws->getItemName() == current_station_name; });
                if (current_it != stations.end()) {
                    line_parts.push_back(*current_it);
                }

                // NEXT_WORKSTATION (optional)
                if (more) {
                    next_station_name = util.extractToken(record, next_pos, more);
                    next_station_names.push_back(next_station_name);
                } else {
                    next_station_names.push_back(""); // No next station
                }
            } catch (const std::string& msg) {
                // Ignore records that cannot be parsed fully (e.g., if a next station isn't specified)
            }
        }
        f.close();

        // --- 2. Link stations ---
        for (size_t i = 0; i < line_parts.size(); ++i) {
            if (!next_station_names[i].empty()) {
                auto next_it = std::find_if(stations.begin(), stations.end(), 
                                            [&](Workstation* ws) { return ws->getItemName() == next_station_names[i]; });
                if (next_it != stations.end()) {
                    line_parts[i]->setNextStation(*next_it);
                }
            }
        }

        // --- 3. Identify the first station ---
        // Stations that are pointed to by *any* other station are not the first station.
        std::vector<Workstation*> next_stations;
        for (Workstation* ws : stations) {
            Workstation* next = ws->getNextStation();
            if (next) {
                next_stations.push_back(next);
            }
        }
        
        // Find the station that is NOT in the list of next_stations
        auto first_it = std::find_if(stations.begin(), stations.end(), 
                                     [&](Workstation* ws) {
                                         return std::find(next_stations.begin(), next_stations.end(), ws) == next_stations.end();
                                     });

        if (first_it != stations.end()) {
            m_firstStation = *first_it;
        }

        // Initialize m_activeLine with all stations (will be reordered later)
        m_activeLine = stations; 

        // --- 4. Update order count ---
        m_cntCustomerOrder = g_pending.size();
    }

    void LineManager::reorderStations() {
        if (!m_firstStation) return;

        std::vector<Workstation*> reordered_line;
        Workstation* current = m_firstStation;

        // Traverse the line from the first station to the end
        while (current) {
            reordered_line.push_back(current);
            current = current->getNextStation();
        }

        // Overwrite the member variable
        m_activeLine = reordered_line;
    }

    bool LineManager::run(std::ostream& os) {
        static size_t iteration_count = 0;
        iteration_count++;

        os << "Line Manager Iteration: " << iteration_count << std::endl;

        // 1. Move the order from g_pending to the first station
        if (!g_pending.empty()) {
            // Use operator+= to move the order
            *m_firstStation += std::move(g_pending.front());
            g_pending.pop_front();
        }

        // 2. Execute one fill operation for each station
        std::for_each(m_activeLine.begin(), m_activeLine.end(), 
                      [&os](Workstation* ws) { ws->fill(os); });

        // 3. Attempt to move an order down the line for each station
        std::for_each(m_activeLine.begin(), m_activeLine.end(), 
                      [](Workstation* ws) { ws->attemptToMoveOrder(); });

        // 4. Return true if all orders are processed
        return m_cntCustomerOrder == (g_completed.size() + g_incomplete.size());
    }

    void LineManager::display(std::ostream& os) const {
        std::for_each(m_activeLine.begin(), m_activeLine.end(), 
                      [&os](Workstation* ws) { ws->display(os); });
    }
}