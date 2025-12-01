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


    bool LineManager::run(std::ostream& os) {
        static size_t iteration = 0u;

        // FIX: Move the incoming order BEFORE printing the iteration header.
        // This ensures the FILL operations of the current iteration (which
        // include filling the newly moved order) are printed BEFORE the
        // header of the NEXT run() call, aligning the output.
        if (!g_pending.empty()) {
            *m_firstStation += std::move(g_pending.front());
            g_pending.pop_front();
        }
        
        // Print the header for this iteration.
        os << "Line Manager Iteration: " << ++iteration << "\n";

        // 1. FILL forward
        for (auto* ws : m_activeLine) 
            ws->fill(os);

        // 2. MOVE backward
        for (auto it = m_activeLine.rbegin(); it != m_activeLine.rend(); ++it)
            (*it)->attemptToMoveOrder();

        return (g_completed.size() + g_incomplete.size()) == m_cntCustomerOrder;
    }


    void LineManager::display(std::ostream& os) const {
        for (auto* ws : m_activeLine)
            ws->display(os);
    }

}