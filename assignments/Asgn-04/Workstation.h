#ifndef SDDS_WORKSTATION_H
#define SDDS_WORKSTATION_H

#include <deque>
#include <iostream>
#include "CustomerOrder.h"
#include "Station.h"

using namespace std;

// Changed namespace sdds to seneca for the global deque declarations
extern deque<seneca::CustomerOrder> g_pending;
extern deque<seneca::CustomerOrder> g_completed;
extern deque<seneca::CustomerOrder> g_incomplete;

// Changed namespace sdds to seneca
namespace seneca {

    class Workstation : public Station {
        deque<CustomerOrder> m_orders;
        Workstation* m_pNextStation = nullptr;

    public:
        Workstation(const string& data);
        void fill(ostream& os);
        bool attemptToMoveOrder();
        void setNextStation(Workstation* station = nullptr);
        Workstation* getNextStation() const;
        void display(ostream& os) const;
        Workstation& operator+=(CustomerOrder&& new_Order);
        bool empty() const;
    };
};

#endif