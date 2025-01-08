#ifndef GPS_NIVA_DATAREADING_H
#define GPS_NIVA_DATAREADING_H

#include <string>
#include <vector>

namespace GPS::NIVA
{

// Stores the format code and fields of a NIVA data reading.
// Post-condition: the format string must consist of alphabet characters in block capitals.
struct DataReading
{
    std::string format;

    std::vector<std::string> dataFields;
};

}

#endif
