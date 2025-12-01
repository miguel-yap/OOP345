/* Citation and Sources.... 
Assignment 04
Module: CustomerOrder
FileName: CustomerOrder.h
------------------------------------------------------------------
Author: Miguel Yap
StNo: 168049237
Email: myap1@myseneca.ca
Date Completion: Sun, Nov 30, 2025
------------------------------------------------------------------
I declare that this submission is the result of my own work and I only copied the code that my professor provided to complete my assignments. 
This submitted piece of work has not been shared with any other student or 3rd party content provider.
------------------------------------------------------------------*/

#ifndef SENECA_CUSTOMERORDER_H
#define SENECA_CUSTOMERORDER_H

#include <iostream>
#include <string>
#include <utility>
#include "Station.h"

namespace seneca {
    struct Item {
        std::string m_itemName;
        size_t m_serialNumber{0};
        bool m_isFilled{false};

        Item(const std::string& src) : m_itemName(src) {};
    };

    class CustomerOrder {
        std::string m_name;
        std::string m_product;
        size_t m_cntItem;
        Item** m_lstItem;

        static size_t m_widthField;

    public:
        CustomerOrder();
        CustomerOrder(const std::string& record);
        CustomerOrder(const CustomerOrder& other); 
        CustomerOrder& operator=(const CustomerOrder& other) = delete; 

        CustomerOrder(CustomerOrder&& other) noexcept;
        CustomerOrder& operator=(CustomerOrder&& other) noexcept;
        
        ~CustomerOrder();

        bool isOrderFilled() const;
        bool isItemFilled(const std::string& itemName) const;
        void fillItem(Station& station, std::ostream& os);
        void display(std::ostream& os) const;
    };
}

#endif // SENECA_CUSTOMERORDER_H