#ifndef SDDS_STATION_H
#define SDDS_STATION_H

#include <string>

using namespace std;

// Changed namespace sdds to seneca
namespace seneca {
    class Station {
        int id;
        string name;
        string desc;
        unsigned int nextSerialNo;
        unsigned int noItems;

        static size_t m_widthField;
        static int id_generator;

    public:
        Station(const string& record);
        const string& getItemName() const;
        size_t getNextSerialNumber();
        size_t getQuantity() const;
        void updateQuantity();
        void display(ostream& os, bool full) const;
    };
};


#endif