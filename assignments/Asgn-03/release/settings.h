/* Citation and Sources.... 
Assignment 03
Module: settings
FileName: settings.h
------------------------------------------------------------------
Author: Miguel Yap
StNo: 168049237
Email: myap1@myseneca.ca
Date Completion: Sun, Nov 16, 2025
------------------------------------------------------------------
I declare that this submission is the result of my own work and I only copied the code that my professor provided to complete my assignments. 
This submitted piece of work has not been shared with any other student or 3rd party content provider.
------------------------------------------------------------------*/

#ifndef SENECA_SETTINGS_H
#define SENECA_SETTINGS_H

namespace seneca
{
    struct Settings
    {
        short m_maxSummaryWidth{80};
        bool m_tableView{false};
    };

    extern Settings g_settings;
}

#endif
