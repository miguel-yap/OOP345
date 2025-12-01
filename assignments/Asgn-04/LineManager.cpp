#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include "LineManager.h"
#include "Utilities.h"
#include "Workstation.h" // Ensure Workstation.h is included for the deque declarations

namespace seneca {
    LineManager::LineManager(const std::string& file, const std::vector<Workstation*>& stations) {
        std::ifstream f(file);
        if (!f) {
            throw std::string("Unable to open [") + file + "] file.";
        }

        Utilities util;
        std::string record;
        // Use temporary vectors to help determine the start station and links
        std::vector<std::string> current_station_names;
        std::vector<std::string> next_station_names;

        // --- 1. Load linkage file and determine order/link names ---
        while (std::getline(f, record)) {
            if (record.empty()) continue;

            size_t next_pos = 0;
            bool more = true;
            std::string current_station_name;
            std::string next_station_name;

            try {
                // WORKSTATION (Current Station Name)
                current_station_name = util.extractToken(record, next_pos, more);
                current_station_names.push_back(current_station_name);

                // NEXT_WORKSTATION (optional)
                if (more) {
                    next_station_name = util.extractToken(record, next_pos, more);
                    next_station_names.push_back(next_station_name);
                } else {
                    next_station_names.push_back(""); // No next station
                }
            } catch (...) {
                // Ignore records that cannot be parsed fully
            }
        }
        f.close();

        // --- 2. Populate m_activeLine in file order and set linkages ---
        
        // m_activeLine stores the stations in the order read from the file.
        // This fixes the "Display Stations (loaded)" mismatch.
        for (const auto& name : current_station_names) {
            auto it = std::find_if(stations.begin(), stations.end(),
                                   [&](Workstation* ws) { return ws->getItemName() == name; });
            if (it != stations.end()) {
                m_activeLine.push_back(*it);
            }
        }

        // Now set the linkages using the names read earlier
        for (size_t i = 0; i < current_station_names.size(); ++i) {
            if (!next_station_names[i].empty()) {
                
                // Find the current station object from the full list
                auto current_ws_it = std::find_if(stations.begin(), stations.end(), 
                                               [&](Workstation* ws) { return ws->getItemName() == current_station_names[i]; });
                
                // Find the next station object from the full list
                auto next_ws_it = std::find_if(stations.begin(), stations.end(), 
                                            [&](Workstation* ws) { return ws->getItemName() == next_station_names[i]; });
                
                if (current_ws_it != stations.end() && next_ws_it != stations.end()) {
                    (*current_ws_it)->setNextStation(*next_ws_it);
                }
            }
        }


        // --- 3. Identify the first station (The station that is NOT a 'next' station) ---
        
        // Get all stations that are pointed to (i.e., every station except the first)
        std::vector<Workstation*> next_stations;
        std::for_each(stations.begin(), stations.end(), 
                      [&](Workstation* ws) { 
                          Workstation* next = ws->getNextStation();
                          if (next) {
                              next_stations.push_back(next);
                          }
                      });
        
        // Find the station that is not present in the 'next_stations' list.
        auto first_it = std::find_if(stations.begin(), stations.end(), 
                                     [&](Workstation* ws) {
                                         // If the station is not found in the 'next_stations' list, it's the start.
                                         return std::find(next_stations.begin(), next_stations.end(), ws) == next_stations.end();
                                     });

        if (first_it != stations.end()) {
            m_firstStation = *first_it;
        }

        // --- 4. Update total order count ---
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

        // Overwrite the member variable with the ordered collection
        m_activeLine = reordered_line;
    }

    bool LineManager::run(std::ostream& os) {
        static size_t iteration_count = 0;
        iteration_count++;

        os << "Line Manager Iteration: " << iteration_count << std::endl;

        // 1. Move the order from g_pending to the first station
        if (!g_pending.empty()) {
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