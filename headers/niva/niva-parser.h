#ifndef GPS_NIVA_PARSER_H
#define GPS_NIVA_PARSER_H

#include <string>
#include <vector>
#include <istream>

#include "niva-data_reading.h"
#include "waypoint.h"

namespace GPS::NIVA
{

  // Check whether the provided format code is one of the formats known to the parser (and hence can be parsed).
  bool isKnownFormat(std::string);



  // Check whether the provided string is in the general form of a NIVA data reading.
  // Does not check the details of any specific NIVA format.
  bool isWellformedDataReading(std::string);



  // Compute the XOR reduction of all of the ASCII character codes in the provided raw text.
  unsigned int computeChecksum(std::string);



  // Check whether the (decimal notation) checksum value in the provided raw text of a NIVA data reading matches
  // the checksum computed from the raw text between the vertical bars.
  // Pre-condition: the provided string is a well-formed NIVA data reading.
  bool hasMatchingChecksum(std::string);



  // Parses the raw text of a NIVA data reading and stores the contents in a structured form in a NIVA::DataReading.
  // Pre-condition: the provided string is a well-formed NIVA data reading.
  NIVA::DataReading parseDataReading(std::string);



  // Checks whether the data reading contains the correct number of fields for the reading type of the format code.
  // Pre-condition: the reading contains a known format code in block capitals.
  bool hasCorrectNumberOfFields(NIVA::DataReading);



  // Extracts the GPS location data from the data reading and constructs a Waypoint object.
  // Pre-conditions:
  //    - the data reading contains a known format code in block capitals.
  //    - the data reading has the correct number of fields.
  GPS::Waypoint extractWaypointFromReading(NIVA::DataReading);



  // Extracts the GPS location data from a stream of raw text NIVA readings.
  // The stream is assummed to be a sequence of NIVA data readings, optionally separated by whitespace.
  // Any text in the stream that is not a well-formed data reading is skipped.
  // Data readings with erroneus values in the fields are also skipped.
  std::vector<GPS::Waypoint> extractWaypointsFromLog(std::istream&);
}

#endif
