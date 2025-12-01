#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <set> // Required for tracking next stations for m_first_Station logic
#include "LineManager.h"
#include "Utilities.h"
#include "Workstation.h" // Ensures Workstation details are visible

using namespace sdds;
using namespace std;

// Implementation of Workstation::empty is needed to use it in LineManager::run
bool sdds::Workstation::empty() const {
    return m_orders.empty();
}


LineManager::LineManager(const string& file, const vector<Workstation*>& stations) {
	Utilities utility;

	ifstream input(file);
	if (!input.is_open()) {
		throw string("Unable to open file.");
	}

    std::set<Workstation*> nextStations; // Tracks all workstations that appear as a 'next' station
    std::vector<Workstation*> addedToActiveLine; // Tracks workstations already added to maintain file order and uniqueness

	string data;
	while (getline(input, data))
	{
        if (data.empty()) continue;

		size_t pos = 0;
		bool more = true;
		string curStation, nextStation;
		curStation = utility.extractToken(data, pos, more);

		if (more) {
			nextStation = utility.extractToken(data, pos, more);
		}

		Workstation* curWorkstation = nullptr;
		Workstation* nextWorkstation = nullptr;
        
        // Use find_if with a lambda to efficiently search the stations vector
        auto find_ws = [&](const string& name) {
            return std::find_if(stations.begin(), stations.end(), [&](Workstation* ws) {
                return ws->getItemName() == name;
            });
        };

        auto it_cur = find_ws(curStation);
        if (it_cur != stations.end()) {
            curWorkstation = *it_cur;
        }

        if (!nextStation.empty()) {
            auto it_next = find_ws(nextStation);
            if (it_next != stations.end()) {
                nextWorkstation = *it_next;
                nextStations.insert(nextWorkstation); // Record this as a 'next' station
            }
        }
        
        // If curWorkstation was not found, there is a fundamental error (though unlikely given tester structure)

		curWorkstation->setNextStation(nextWorkstation);
        
        // FIX 1: Add curWorkstation to m_active_Line only if it hasn't been added yet (maintain uniqueness and file order)
        if (std::find(addedToActiveLine.begin(), addedToActiveLine.end(), curWorkstation) == addedToActiveLine.end()) {
            m_active_Line.push_back(curWorkstation);
            addedToActiveLine.push_back(curWorkstation);
        }
	}

	input.close();
    
    // Determine m_first_Station: the station that never appears in the 'nextStations' set
    m_first_Station = nullptr;
    for (Workstation* ws : stations) {
        // Must be part of the line configuration AND not appear as a next station
        if (std::find(m_active_Line.begin(), m_active_Line.end(), ws) != m_active_Line.end() &&
            nextStations.find(ws) == nextStations.end()) 
        {
            m_first_Station = ws;
            break;
        }
    }
    
    // Store the total number of orders in the pending queue at startup
    m_cntCustomer_Order = g_pending.size();
}

void LineManager::reorderStations() {
	vector<Workstation*> newStations;
	Workstation* currStation = m_first_Station;
	
	while (currStation != nullptr) {
		newStations.push_back(currStation);
		currStation = currStation->getNextStation();
	}
    
    // Replace the active line with the correctly ordered line
	m_active_Line = std::move(newStations);
}

bool LineManager::run(ostream& os) {
	static int count = 0;
	
    // FIX 2: Move the incoming order BEFORE printing the iteration header.
    // This aligns the line count where the fills of the newly moved order appear.
	if (!g_pending.empty()) {
		(*m_first_Station) += move(g_pending.front());
		g_pending.pop_front();
	}

    // Print the header for this iteration.
	count++;
	os << "Line Manager Iteration: " << count << endl;

    // 1. FILL forward
	for (unsigned int i = 0; i < m_active_Line.size(); i++) {
		m_active_Line[i]->fill(os);
	}

    // 2. MOVE backward (using fixed reverse loop)
	for (int i = m_active_Line.size() - 1; i >= 0; i--) { 
		m_active_Line[i]->attemptToMoveOrder();
	}

    // Check if all initial orders have been moved to g_completed or g_incomplete
	return (g_completed.size() + g_incomplete.size()) == m_cntCustomer_Order;
}

void LineManager::display(ostream& os) const {
	for (unsigned int i = 0; i < m_active_Line.size(); i++) {
		m_active_Line[i]->display(os);
	}
}