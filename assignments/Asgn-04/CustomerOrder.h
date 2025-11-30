#ifndef SENECA_CUSTOMERORDER_H
#define SENECA_CUSTOMERORDER_H

#include <string>
#include <iostream>

namespace seneca {

    class Station;

    class CustomerOrder {
        struct Item {
            std::string m_itemName;
            size_t m_serialNumber{0};
            bool m_isFilled{false};

            Item(const std::string& src) : m_itemName(src) {}
        };

        std::string m_name;
        std::string m_product;
        size_t m_cntItem{0};
        Item** m_lstItem{nullptr};

        static size_t m_widthField;

    public:
        CustomerOrder() = default;
        CustomerOrder(const std::string& record);

        // no copy
        CustomerOrder(const CustomerOrder& other);
        CustomerOrder& operator=(const CustomerOrder& other) = delete;

        // move
        CustomerOrder(CustomerOrder&& other) noexcept;
        CustomerOrder& operator=(CustomerOrder&& other) noexcept;

        ~CustomerOrder();

        bool isOrderFilled() const;
        bool isItemFilled(const std::string& itemName) const;

        void fillItem(Station& station, std::ostream& os);
        void display(std::ostream& os) const;
    };

} // namespace seneca

#endif // SENECA_CUSTOMERORDER_H
