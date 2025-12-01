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
    bool more;
    size_t pos;

    std::vector<Workstation*> nextStations;

    // Do NOT push into m_activeLine here
    // m_activeLine stays empty during file parsing

    while (std::getline(in, line)) {
        if (line.empty()) continue;

        pos = 0u;
        more = true;

        std::string currentName = util.extractToken(line, pos, more);
        std::string nextName = more ? util.extractToken(line, pos, more) : "";

        auto curIt = std::find_if(stations.begin(), stations.end(),
            [&](Workstation* ws){ return ws->getItemName() == currentName; });

        Workstation* currentWS = *curIt;

        if (!nextName.empty()) {
            auto nextIt = std::find_if(stations.begin(), stations.end(),
                [&](Workstation* ws){ return ws->getItemName() == nextName; });

            Workstation* nextWS = *nextIt;
            currentWS->setNextStation(nextWS);
            nextStations.push_back(nextWS);
        }

        // PROBLEM LINE REMOVED ⛔
        // m_activeLine.push_back(currentWS);
    }

    // find first station: the one that is not in nextStations
    for (auto* ws : stations) {
        if (std::find(nextStations.begin(), nextStations.end(), ws) == nextStations.end()) {
            m_firstStation = ws;
            break;
        }
    }

    // Build the active line in correct order
    reorderStations();
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
        os << "Line Manager Iteration: " << ++iteration << '\n';

        if (!g_pending.empty()) {
            *m_firstStation += std::move(g_pending.front());
            g_pending.pop_front();
        }

        for (auto* ws : m_activeLine) {
            ws->fill(os);
        }

        for (auto* ws : m_activeLine) {
            ws->attemptToMoveOrder();
        }

        return (g_completed.size() + g_incomplete.size()) == m_cntCustomerOrder;
    }

    void LineManager::display(std::ostream& os) const {
        for (auto* ws : m_activeLine) {
            ws->display(os);
        }
    }

} // namespace seneca
