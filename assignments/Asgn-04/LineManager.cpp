#include "LineManager.h"
#include "Utilities.h"
#include <fstream>
#include <algorithm>
#include <set> // Using std::set for temporary tracking of stations to avoid duplicates

namespace seneca {

    LineManager::LineManager(const std::string& file, const std::vector<Workstation*>& stations) {
        std::ifstream fin(file);
        if (!fin)
            throw std::string("Unable to open [") + file + "]";

        Utilities util;
        std::string line;
        bool more = false;
        size_t pos = 0u;

        std::set<Workstation*> nextStations; // Stores all workstations that appear as a 'next' station
        std::vector<Workstation*> addedToActiveLine; // Stores stations already added to m_activeLine

        while (std::getline(fin, line)) {
            if (line.empty()) continue;

            pos = 0;
            more = true;
            std::string curName = util.extractToken(line, pos, more);
            std::string nextName = more ? util.extractToken(line, pos, more) : "";

            Workstation* curWS = nullptr;
            Workstation* nextWS = nullptr;

            // Find matching workstation pointers in the provided collection
            for (auto* ws : stations) {
                if (ws->getItemName() == curName)
                    curWS = ws;
                if (!nextName.empty() && ws->getItemName() == nextName)
                    nextWS = ws;
            }
            
            // Check if curWS was actually found
            if (!curWS) {
                throw std::string("Error: Workstation '") + curName + "' not found in stations list.";
            }

            // Set next station link
            curWS->setNextStation(nextWS);

            // Build m_activeLine: only add curWS if it hasn't been added yet (maintain uniqueness + file order)
            if (std::find(addedToActiveLine.begin(), addedToActiveLine.end(), curWS) == addedToActiveLine.end()) {
                m_activeLine.push_back(curWS);
                addedToActiveLine.push_back(curWS);
            }

            // Record the next station
            if (nextWS)
                nextStations.insert(nextWS);
        }

        fin.close(); // Close the file stream

        // Find FIRST station: the one that is NEVER a 'next' station
        bool firstFound = false;
        for (auto* ws : stations) {
            // Check if 'ws' is NOT found in the set of 'next' stations
            if (nextStations.find(ws) == nextStations.end()) {
                // Ensure this station is part of the overall line configured by the file
                if (std::find(m_activeLine.begin(), m_activeLine.end(), ws) != m_activeLine.end()) {
                    m_firstStation = ws;
                    firstFound = true;
                    break;
                }
            }
        }
        
        if (!firstFound) {
            // This happens if the assembly line file defines a loop (A->B, B->A) or is empty
            throw std::string("Error: Could not determine the first station on the line.");
        }


        // IMPORTANT: Total number of customer orders is initially the size of g_pending
        m_cntCustomerOrder = g_pending.size();
    }


    void LineManager::reorderStations() {
        std::vector<Workstation*> ordered;
        Workstation* curr = m_firstStation;

        // Traverse the linked list starting from the first station
        while (curr != nullptr) {
            ordered.push_back(curr);
            curr = curr->getNextStation();
        }

        // Replace the current active line (which was in file order) with the reordered line
        m_activeLine = std::move(ordered);
    }


    bool LineManager::run(std::ostream& os) {
        static size_t iteration = 0u;
        os << "Line Manager Iteration: " << ++iteration << "\n";

        // 1. Move one order from g_pending queue to the first station
        if (!g_pending.empty()) {
            *m_firstStation += std::move(g_pending.front());
            g_pending.pop_front();
        }

        // 2. For each station, execute one fill operation (forward iteration)
        for (auto* ws : m_activeLine)
            ws->fill(os);

        // 3. For each station, attempt to move an order down the line (backward iteration for reliability)
        // Must iterate backwards to ensure orders move through the line correctly in a single run()
        for (auto it = m_activeLine.rbegin(); it != m_activeLine.rend(); ++it)
            (*it)->attemptToMoveOrder();

        // Return true if all initial orders are now in g_completed or g_incomplete
        return (g_completed.size() + g_incomplete.size()) == m_cntCustomerOrder;
    }


    void LineManager::display(std::ostream& os) const {
        for (auto* ws : m_activeLine)
            ws->display(os);
    }

}