#include "LineManager.h"
#include "Utilities.h"
#include <fstream>
#include <algorithm>

namespace seneca {

    LineManager::LineManager(const std::string& file, const std::vector<Workstation*>& stations) {
        std::ifstream fin(file);
        if (!fin)
            throw std::string("Unable to open [") + file + "]";

        Utilities util;
        std::string line;
        bool more = false;
        size_t pos = 0u;

        std::vector<Workstation*> nextStations;

        while (std::getline(fin, line)) {
            if (line.empty()) continue;

            pos = 0;
            more = true;
            std::string curName = util.extractToken(line, pos, more);
            std::string nextName = more ? util.extractToken(line, pos, more) : "";

            Workstation* curWS = nullptr;
            Workstation* nextWS = nullptr;

            // find matching workstation pointers
            for (auto* ws : stations) {
                if (ws->getItemName() == curName)
                    curWS = ws;
                if (!nextName.empty() && ws->getItemName() == nextName)
                    nextWS = ws;
            }

            // set next station link
            curWS->setNextStation(nextWS);

            // build active line IN LOADED ORDER
            m_activeLine.push_back(curWS);

            if (nextWS)
                nextStations.push_back(nextWS);
        }

        // find FIRST station: the one that is NEVER a nextStation
        for (auto* ws : stations) {
            if (std::find(nextStations.begin(), nextStations.end(), ws) == nextStations.end()) {
                m_firstStation = ws;
                break;
            }
        }

        // IMPORTANT: DO NOT reorder here (tester 3 will call reorderStations())
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
        os << "Line Manager Iteration: " << ++iteration << "\n";

        // add incoming order to first station
        if (!g_pending.empty()) {
            *m_firstStation += std::move(g_pending.front());
            g_pending.pop_front();
        }

        // 1) FILL forward
        for (auto* ws : m_activeLine)
            ws->fill(os);

        // 2) MOVE backward (critical for tester 3 timing)
        for (auto it = m_activeLine.rbegin(); it != m_activeLine.rend(); ++it)
            (*it)->attemptToMoveOrder();

        return (g_completed.size() + g_incomplete.size()) == m_cntCustomerOrder;
    }


    void LineManager::display(std::ostream& os) const {
        for (auto* ws : m_activeLine)
            ws->display(os);
    }

}
