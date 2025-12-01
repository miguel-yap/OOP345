#include "LineManager.h"
#include "Utilities.h"
#include <fstream>
#include <algorithm>
#include <stdexcept>

namespace seneca {

    LineManager::LineManager(const std::string& file, const std::vector<Workstation*>& stations) {
        std::ifstream in(file);
        if (!in)
            throw std::string("Unable to open [") + file + "] file.";

        Utilities util;
        std::string line;
        bool more = false;
        size_t pos = 0u;

        std::vector<Workstation*> nextStations;

        // IMPORTANT: build m_activeLine IN THE ORDER OF THE CONFIG FILE
        while (std::getline(in, line)) {
            if (line.empty()) continue;

            pos = 0u;
            more = true;

            std::string currentName = util.extractToken(line, pos, more);
            std::string nextName = more ? util.extractToken(line, pos, more) : "";

            // find current station
            auto curIt = std::find_if(stations.begin(), stations.end(),
                [&](Workstation* ws) { return ws->getItemName() == currentName; });

            Workstation* currentWS = *curIt;

            // ADD currentWS IN FILE ORDER
            m_activeLine.push_back(currentWS);

            // link next station if exists
            if (!nextName.empty()) {
                auto nextIt = std::find_if(stations.begin(), stations.end(),
                    [&](Workstation* ws) { return ws->getItemName() == nextName; });

                Workstation* nextWS = *nextIt;
                currentWS->setNextStation(nextWS);
                nextStations.push_back(nextWS);
            }
        }

        // find FIRST station (never appears on right side of config file)
        for (auto* ws : stations) {
            if (std::find(nextStations.begin(), nextStations.end(), ws) == nextStations.end()) {
                m_firstStation = ws;
                break;
            }
        }

        // DO NOT CALL reorderStations() HERE !!! (tester_3 calls it later)
        m_cntCustomerOrder = g_pending.size();
    }


    void LineManager::reorderStations() {
        std::vector<Workstation*> ordered;
        Workstation* current = m_firstStation;

        while (current) {
            ordered.push_back(current);
            current = current->getNextStation();
        }

        m_activeLine = std::move(ordered);
    }


    bool LineManager::run(std::ostream& os) {
        static size_t iteration = 0u;
        os << "Line Manager Iteration: " << ++iteration << "\n";

        // move new order to first station if available
        if (!g_pending.empty()) {
            *m_firstStation += std::move(g_pending.front());
            g_pending.pop_front();
        }

        // 1) Fill in FORWARD order
        for (auto* ws : m_activeLine)
            ws->fill(os);

        // 2) Move in REVERSE order
        for (auto it = m_activeLine.rbegin(); it != m_activeLine.rend(); ++it)
            (*it)->attemptToMoveOrder();

        return (g_completed.size() + g_incomplete.size()) == m_cntCustomerOrder;
    }


    void LineManager::display(std::ostream& os) const {
        for (auto* ws : m_activeLine)
            ws->display(os);
    }

}
