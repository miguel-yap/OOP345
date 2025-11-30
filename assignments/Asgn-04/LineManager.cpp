#include "LineManager.h"
#include "Utilities.h"
#include <fstream>
#include <algorithm>
#include <stdexcept>

namespace seneca {

    LineManager::LineManager(const std::string& file, const std::vector<Workstation*>& stations) {
        std::ifstream in(file);
        if (!in) {
            throw std::string("Unable to open [") + file + "] file.";
        }

        Utilities util;
        std::string line;
        bool more{false};
        size_t pos{0u};

        // track which stations appear as "next"
        std::vector<Workstation*> nextStations;

        while (std::getline(in, line)) {
            if (line.empty())
                continue;

            pos = 0u;
            more = true;

            std::string currentName = util.extractToken(line, pos, more);
            std::string nextName;

            if (more) {
                nextName = util.extractToken(line, pos, more);
            }

            auto curIt = std::find_if(
                stations.begin(), stations.end(),
                [&](Workstation* ws) { return ws->getItemName() == currentName; }
            );

            if (curIt == stations.end()) {
                throw std::string("Station not found: ") + currentName;
            }

            Workstation* currentWS = *curIt;

            if (std::find(m_activeLine.begin(), m_activeLine.end(), currentWS) == m_activeLine.end()) {
                m_activeLine.push_back(currentWS);
            }

            if (!nextName.empty()) {
                auto nextIt = std::find_if(
                    stations.begin(), stations.end(),
                    [&](Workstation* ws) { return ws->getItemName() == nextName; }
                );
                if (nextIt == stations.end()) {
                    throw std::string("Next station not found: ") + nextName;
                }

                Workstation* nextWS = *nextIt;
                currentWS->setNextStation(nextWS);

                if (std::find(nextStations.begin(), nextStations.end(), nextWS) == nextStations.end()) {
                    nextStations.push_back(nextWS);
                }
            }
        }

        // find first station (one that never appears as a "next")
        m_firstStation = nullptr;
        for (auto* ws : m_activeLine) {
            if (std::find(nextStations.begin(), nextStations.end(), ws) == nextStations.end()) {
                m_firstStation = ws;
                break;
            }
        }

        if (!m_firstStation && !m_activeLine.empty()) {
            m_firstStation = m_activeLine.front();
        }

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
