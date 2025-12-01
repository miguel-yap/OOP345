#include "LineManager.h"
#include "Utilities.h"
#include <fstream>
#include <algorithm>
#include <set> 

namespace seneca {

    LineManager::LineManager(const std::string& file, const std::vector<Workstation*>& stations) {
        std::ifstream fin(file);
        if (!fin)
            throw std::string("Unable to open [") + file + "]";

        Utilities util;
        std::string line;
        bool more = false;
        size_t pos = 0u;

        std::set<Workstation*> nextStations;
        std::vector<Workstation*> addedToActiveLine; 

        while (std::getline(fin, line)) {
            if (line.empty()) continue;

            pos = 0;
            more = true;
            std::string curName = util.extractToken(line, pos, more);
            std::string nextName = more ? util.extractToken(line, pos, more) : "";

            Workstation* curWS = nullptr;
            Workstation* nextWS = nullptr;

            // Find matching workstation pointers
            for (auto* ws : stations) {
                if (ws->getItemName() == curName)
                    curWS = ws;
                if (!nextName.empty() && ws->getItemName() == nextName)
                    nextWS = ws;
            }
            
            if (!curWS) {
                throw std::string("Error: Workstation '") + curName + "' not found in stations list.";
            }

            // Set next station link
            curWS->setNextStation(nextWS);

            // FIX: Only add curWS if it hasn't been added yet (maintain uniqueness + file order)
            if (std::find(addedToActiveLine.begin(), addedToActiveLine.end(), curWS) == addedToActiveLine.end()) {
                m_activeLine.push_back(curWS);
                addedToActiveLine.push_back(curWS);
            }

            // Record the next station
            if (nextWS)
                nextStations.insert(nextWS);
        }

        fin.close();

        // Find FIRST station: the one that is NEVER a 'next' station
        bool firstFound = false;
        for (auto* ws : stations) {
            if (nextStations.find(ws) == nextStations.end()) {
                if (std::find(m_activeLine.begin(), m_activeLine.end(), ws) != m_activeLine.end()) {
                    m_firstStation = ws;
                    firstFound = true;
                    break;
                }
            }
        }
        
        if (!firstFound) {
            throw std::string("Error: Could not determine the first station on the line.");
        }

        m_cntCustomerOrder = g_pending.size();
    }


    void LineManager::reorderStations() {
        std::vector<Workstation*> ordered;
        Workstation* curr = m_firstStation;

        while (curr != nullptr) {
            ordered.push_back(curr);
            curr = curr->getNextStation();
        }

        m_activeLine = std::move(ordered);
    }


    // Corrected LineManager::run() - FINAL VERSION

bool LineManager::run(std::ostream& os) {
    static size_t iteration = 0u;
    
    // 1. Move one order from g_pending queue to the first station
    if (!g_pending.empty()) {
        *m_firstStation += std::move(g_pending.front());
        g_pending.pop_front();
    }
    
    // 2. Print the header for this iteration.
    os << "Line Manager Iteration: " << ++iteration << "\n";

    // 3. FILL forward (This will include the order just moved from pending)
    for (auto* ws : m_activeLine) 
        ws->fill(os);

    // 4. MOVE backward (This allows orders to progress one step)
    for (auto it = m_activeLine.rbegin(); it != m_activeLine.rend(); ++it)
        (*it)->attemptToMoveOrder();

    return (g_completed.size() + g_incomplete.size()) == m_cntCustomerOrder;
}


    void LineManager::display(std::ostream& os) const {
        for (auto* ws : m_activeLine)
            ws->display(os);
    }

}