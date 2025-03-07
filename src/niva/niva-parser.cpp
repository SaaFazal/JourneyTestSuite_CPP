#include <iostream>
#include <algorithm> // For std::transform
#include <stdexcept>
#include <cmath>
#include <set>
#include <string>

#include "geometry.h"
#include "niva/niva-parser.h"

namespace GPS
{
namespace NIVA
{
using namespace std;

// Overload for std::string
unsigned int computeChecksum(const std::string& s) {
    unsigned int checksum = 0;
    for (char c : s) {
        checksum ^= static_cast<unsigned int>(c);
    }
    return checksum;
}

// Overload for const char* (to handle string literals)
unsigned int computeChecksum(const char* s) {
    return computeChecksum(std::string(s));
}
// Overload for std::string
bool hasMatchingChecksum(const std::string& s) {
    // Find the position of the last vertical bar
    size_t lastBarPos = s.find_last_of('|', s.find(';') - 1);

    // Extract the data between the first and last vertical bars
    size_t firstBarPos = s.find('|');
    std::string dataSection = s.substr(firstBarPos + 1, lastBarPos - firstBarPos - 1);

    // Extract the checksum from the string
    std::string checksumStr = s.substr(lastBarPos + 1, 3);
    unsigned int providedChecksum = std::stoi(checksumStr);

    // Compute the checksum of the data section
    unsigned int calculatedChecksum = computeChecksum(dataSection);

    // Compare the checksums
    return providedChecksum == calculatedChecksum;
}

// Overload for const char* (to handle string literals)
bool hasMatchingChecksum(const char* s) {
    return hasMatchingChecksum(std::string(s));
}

// Overload for std::string
bool isKnownFormat(const std::string& s) {
    // Check if the input string has exactly 4 characters
    if (s.size() != 4) {
        return false;
    }

    // Convert the input string to uppercase
    std::string format = s;
    std::transform(format.begin(), format.end(), format.begin(), ::toupper);

    // Define a set of known formats for easy lookup
    static const std::set<std::string> knownFormats = {"NEIL", "NUNO", "ISMA"};

    // Check if the format is in the set of known formats
    return knownFormats.find(format) != knownFormats.end();
}

// Overload for const char* (to handle string literals)
bool isKnownFormat(const char* s) {
    // Convert the C-string to a std::string and call the existing function
    return isKnownFormat(std::string(s));
}

// Helper function to check if a character is a valid format code character (A-Z or a-z)
bool isValidCharacter(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool isWellformedDataReading(const std::string& s) {
    // Check basic structure: must start with '~', have at least 6 characters, and have a '|' at position 5
    if (s.empty() || s[0] != '~' || s.size() < 6 || s[5] != '|') {
        return false;
    }

    // Validate the format code (positions 1-4)
    for (size_t i = 1; i <= 4; ++i) {
        if (!isValidCharacter(s[i])) {
            return false;
        }
    }

    // Find the positions of the last '|' and ';'
    size_t lastPipe = s.rfind('|');
    size_t semicolon = s.rfind(';');

    // Validate the positions of '|' and ';'
    if (lastPipe == std::string::npos || semicolon == std::string::npos || semicolon <= lastPipe) {
        return false;
    }

    // Validate the checksum (must be exactly 3 digits between the last '|' and ';')
    size_t checksumLength = semicolon - (lastPipe + 1);
    if (checksumLength != 3) {
        return false; // Checksum must be exactly 3 digits
    }
    for (size_t i = lastPipe + 1; i < semicolon; ++i) {
        if (!isdigit(s[i])) {
            return false; // Checksum must consist of digits
        }
    }

    // Validate that reserved characters (~, ;, |) are not present in the data fields
    size_t firstPipe = s.find('|');
    size_t dataSectionStart = firstPipe + 1;
    size_t dataSectionEnd = lastPipe;

    for (size_t i = dataSectionStart; i < dataSectionEnd; ++i) {
        char c = s[i];
        if (c == '~' || c == ';' || c == '|') {
            return false; // Reserved characters are not allowed in data fields
        }
    }

    return true;
}

NIVA::DataReading parseDataReading(const std::string& s) {
    // Find the position of the first '|'
    size_t firstPipe = s.find('|');
    if (firstPipe == std::string::npos || firstPipe < 5) {
        throw std::invalid_argument("Invalid NIVA data reading: missing format code or fields.");
    }

    // Extract the format code (characters between '~' and the first '|')
    std::string format = s.substr(1, firstPipe - 1);
    std::transform(format.begin(), format.end(), format.begin(), ::toupper);

    // Extract the fields (data between the first '|' and the last '|')
    std::vector<std::string> fields;
    size_t start = firstPipe + 1;
    size_t end = s.find('|', start);

    while (start < end) {
        // Find the next comma or the end of the field
        size_t nextComma = s.find(',', start);
        if (nextComma == std::string::npos || nextComma > end) {
            nextComma = end;
        }

        // Extract the field and add it to the vector
        std::string field = s.substr(start, nextComma - start);
        fields.push_back(field);

        // Move to the next field
        start = nextComma + 1;
    }

    // Handle trailing commas (empty fields at the end)
    if (start == end && s[start - 1] == ',') {
        fields.push_back(""); // Add an empty field for the trailing comma
    }

    // Return the parsed data reading
    return {format, fields};
}

bool hasCorrectNumberOfFields(const NIVA::DataReading& d) {
    // Convert the format code to uppercase
    std::string format = d.format;
    std::transform(format.begin(), format.end(), format.begin(), ::toupper);

    // Check the number of fields based on the format
    if (format == "NEIL" && d.dataFields.size() == 3) {
        return true;
    } else if (format == "NUNO" && d.dataFields.size() == 4) {
        return true;
    } else if (format == "ISMA" && d.dataFields.size() == 5) {
        return true;
    }

    // If the format is unknown or the number of fields is incorrect, return false
    return false;
}

// Helper function to parse a coordinate string into a double
double parseCoordinate(const std::string& coord) {
    try {
        return std::stod(coord);
    } catch (const std::invalid_argument& e) {
        throw std::domain_error("Invalid coordinate value: " + coord);
    }
}
double parseDMS(const std::string& dms) {
    try {
        size_t degPos = dms.find('o');
        size_t minPos = dms.find('\'');
        size_t secPos = dms.find('"');

        if (degPos == std::string::npos || minPos == std::string::npos || secPos == std::string::npos) {
            throw std::domain_error("Invalid DMS format: missing delimiters in " + dms);
        }

        int degrees = std::stoi(dms.substr(0, degPos));
        int minutes = std::stoi(dms.substr(degPos + 1, minPos - (degPos + 1)));
        int seconds = std::stoi(dms.substr(minPos + 1, secPos - (minPos + 1)));

        if (degrees < 0 || minutes < 0 || seconds < 0) {
            throw std::domain_error("Invalid DMS values: negative values in " + dms);
        }

        return degrees + (minutes / 60.0) + (seconds / 3600.0);
    } catch (const std::exception& e) {
        throw std::domain_error("Failed to parse DMS: " + std::string(e.what()));
    }
}

GPS::Waypoint extractWaypointFromReading(const NIVA::DataReading& d) {
    // Handle NEIL format
    if (d.format == "NEIL") {
        double lat = parseCoordinate(d.dataFields[0]);
        double lon = parseCoordinate(d.dataFields[1]);
        double alt = parseCoordinate(d.dataFields[2]);
        return GPS::Waypoint(lat, lon, alt);
    }
    // Handle NUNO format
    else if (d.format == "NUNO") {
        double lat = parseCoordinate(d.dataFields[3]);
        double lon = parseCoordinate(d.dataFields[2]);
        double alt = parseCoordinate(d.dataFields[1]);
        return GPS::Waypoint(lat, lon, alt);
    }
    // Handle ISMA format
    else if (d.format == "ISMA") {
        // Parse latitude in DMS format
        double lat = parseDMS(d.dataFields[0]);
        std::string latBearing = d.dataFields[1];
        if (latBearing != "N" && latBearing != "S") {
            throw std::domain_error("Invalid latitude bearing: " + latBearing);
        }
        if (latBearing == "S") {
            lat = -lat; // Apply bearing for southern hemisphere
        }

        // Parse longitude in DMS format
        double lon = parseDMS(d.dataFields[2]);
        std::string lonBearing = d.dataFields[3];
        if (lonBearing != "E" && lonBearing != "W") {
            throw std::domain_error("Invalid longitude bearing: " + lonBearing);
        }
        if (lonBearing == "W") {
            lon = -lon; // Apply bearing for western hemisphere
        }

        // Parse altitude
        double alt = parseCoordinate(d.dataFields[4]);

        // Validate latitude and longitude ranges
        if (lat < -90.0 || lat > 90.0) {
            throw std::invalid_argument("Latitude values must not exceed 90.000000 degrees.");
        }
        if (lon < -180.0 || lon > 180.0) {
            throw std::invalid_argument("Longitude values must not exceed 180.000000 degrees.");
        }

        return GPS::Waypoint(lat, lon, alt);
    }
    // Throw an exception for unknown formats
    else {
        throw std::invalid_argument("Unknown format: " + d.format);
    }
}

std::vector<GPS::Waypoint> extractWaypointsFromLog(std::istream& is) {
    std::vector<Waypoint> waypoints;
    std::string content;
    std::string line;

    // Read the entire file line by line and concatenate
    while (std::getline(is, line)) {
        content += line + "\\n";
    }

    size_t pos = 0;
    while (pos < content.length()) {
        // Find the start of a NIVA reading
        size_t startPos = content.find('~', pos);
        if (startPos == std::string::npos) break;

        // Find the end of the reading (the semicolon)
        size_t endPos = content.find(';', startPos);
        if (endPos == std::string::npos) break;

        // Extract the complete reading
        std::string reading = content.substr(startPos, endPos - startPos + 1);

        // Process the reading if it's well-formed
        if (isWellformedDataReading(reading)) {
            try {
                DataReading dataReading = parseDataReading(reading);

                if (isKnownFormat(dataReading.format) && hasCorrectNumberOfFields(dataReading)) {
                    try {
                        Waypoint waypoint = extractWaypointFromReading(dataReading);
                        waypoints.push_back(waypoint);
                    } catch (const std::exception&) {
                        // Skip readings with invalid field values
                    }
                }
            } catch (const std::exception&) {
                // Skip readings that can't be parsed
            }
        }

        // Move to position after the current reading
        pos = endPos + 1;
    }

    return waypoints;
}
} // namespace NIVA
} // namespace GPS
