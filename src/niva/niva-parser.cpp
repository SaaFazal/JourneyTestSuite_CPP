#include <iostream>
#include <stdexcept>
#include <cmath>

#include "geometry.h"
#include "niva/niva-parser.h"

namespace GPS
{

namespace NIVA
{
  using namespace std;

  bool isKnownFormat(string s)
  {
      if (s.size() != 4)
      {
          return false;
      }

      // Convert to upper case.
      for (unsigned int i = 0; i < 4; ++i)
      {
          if (s[i] >= 'a' && s[i] <= 'z')
          {
              s[i] = s[i] - 32;
          }
      }

      if (s == "NEIL" || s == "NUNO" || s == "ISMA")
      {
          return true;
      }
      else
      {
          return false;
      }
  }

  bool isWellformedDataReading(string s)
  {
      char c;
      unsigned int i;
      if (s.empty()) return false; // Need this or next line could crash
      if (s[0] != '~') return false;
      if (s.size() < 5) return false; // Need this or next line could crash
      // cout << "Debug A" << endl;
      c = s[1];
      if (((c < 65) || (c > 90)) && ((c < 97) || (c > 122))) return false;
      c = s[2];
      if (((c < 65) || (c > 90)) && ((c < 97) || (c > 122))) return false;
      c = s[3];
      if (((c < 65) || (c > 90)) && ((c < 97) || (c > 122))) return false;
      c = s[4];
      if (((c < 'A') || (c > 'Z')) && ((c < 'a') || (c > 'z'))) return false; // Hey Ken: we can do it like this instead of using the ASCII codes.  Shall I change the previous two like this as well?
      // cout << "Debug B" << endl;
      if (s.size() < 6) return false; // Need this or next line could crash.
      if (s[5] != '|') return false;
      // cout << "Debug C" << endl;
      /*
      i = 6;
      while (true)
      {
          if (i == s.size()) return false;
          if (s[i] == '~') return false; // Not allowed here.
          if (s[i] == ';') break;
          ++i;
      }
      */
      // Nadia: for-loop is better.
      for (i = 6; true; ++i)
      {
          if (i == s.size()) return false;
          if (s[i] == '~') return false; // Not allowed here.
          if (s[i] == ';') return false; // Not allowed here.
          if (s[i] == '|') break;
      }
      // cout << "Debug D" << endl;
      if (s.size() < i+4) return false; // Note: i+4 NOT i+3.  This is confusing.
      c = s[i+1];
      if ((c < 48) || (c > 57)) return false;
      c = s[i+2];
      if ((c < 48) || (c > 57)) return false;
      c = s[i+3];
      if ((c < 48) || (c > 57)) return false;
      // cout << "Debug E" << endl;
      // cout << s.size() << endl;
      // cout << i << endl;
      if (s[i+4] != ';') return false;
      if (s.size() != i+5) return false; // Note: i+5 NOT i+4.  This is confusing.
      // Alice: It's because indexing starts at 0, so the index number is one less.
      // Nadia: No, it's because we're using '<' rather than '<='.  We need a char at i+4, so we say < i+5.
      // Ken: But indexing normally starts at 0, and we normally use <. So why is this different?
      return true;
  }

  // Function Stub - to be implemented
  // Computes the checksum using XOR on all characters in the string.
unsigned int computeChecksum(const std::string& s) {
    if (s.empty()) return 0;  // Edge case for empty string

    unsigned int checksum = static_cast<unsigned int>(s[0]); // Start with first char
    for (size_t i = 1; i < s.length(); ++i) {
        checksum ^= static_cast<unsigned int>(s[i]);  // XOR each character
    }
    return checksum;
}



// Validates if the provided checksum matches the computed checksum
bool hasMatchingChecksum(const std::string& s) {
    // Find the positions of the first '~', last '|', and last ';'
    size_t firstTilde = s.find('~');
    size_t lastPipe = s.find_last_of('|');
    size_t semicolon = s.find_last_of(';');

    // Validate the positions
    if (firstTilde == std::string::npos || lastPipe == std::string::npos || semicolon == std::string::npos || semicolon <= lastPipe) {
        return false; // Malformed input
    }

    // Extract the data from after '~' to before the last '|'
    std::string data = s.substr(firstTilde + 1, lastPipe - firstTilde - 1);

    // Extract the checksum between the last '|' and ';'
    std::string checksumStr = s.substr(lastPipe + 1, semicolon - lastPipe - 1);

    // Validate that the checksum string is numeric
    if (!std::all_of(checksumStr.begin(), checksumStr.end(), ::isdigit)) {
        return false; // Invalid checksum format
    }

    // Compute the checksum of the data
    unsigned int computedChecksum = computeChecksum(data);

    // Convert the provided checksum to an integer
    unsigned int providedChecksum = std::stoul(checksumStr);

    // Compare the computed checksum with the provided checksum
    return computedChecksum == providedChecksum;
}

  NIVA::DataReading parseDataReading(std::string s)
  {
      unsigned int i;
      char c;

      string format;
      for (i = 1; true; ++i)
      {
          if (i >= s.size()) throw std::invalid_argument(s + " is not a well-formed NIVA data reading.");
          c = s[i];
          if (c == '|')
          {
              if (i == 1)
              {
                 throw std::invalid_argument(s + " is not a well-formed NIVA data reading.");
              }
              else
              {
                 break;
              }
          }
          if (c >= 'A' && c <= 'Z')
          {
              format = format + c;
          }
          if (c >= 'a' && c <= 'z')
          {
              format += (c - 32); // Convert to upper case
          }
      }

      vector<string> fields;
      do
      {
          i++;
          if (i >= s.size()) throw std::invalid_argument(s + " is not a well-formed NIVA data reading.");
          string field;
          for(; (s[i] != ',') && (s[i] != '|'); ++i)
          {
              if (i >= s.size()) throw std::invalid_argument(s + " is not a well-formed NIVA data reading.");
              field += s[i];
          }
          fields.push_back(field);
      } while (s[i] != '|');

      return {format,fields};
  }

  bool hasCorrectNumberOfFields(NIVA::DataReading d)
  {
      string s = d.format;
      unsigned int n = d.dataFields.size();

      // Convert to uppercase
      for (unsigned int i = 0; i < s.size(); ++i)
      {
          if (s[i] >= 97 && s[i] <= 122)
          {
              s[i] = s[i] - 32;
          }
      }

      if (s == "NEIL" && n == 3)
      {
          return true;
      }
      else if (s == "NUNO" && n == 4)
      {
          return true;
      }
      else if (s == "ISMA" && n == 5)
      {
          return true;
      }
      else
      {
          return false;
      }
  }

  Waypoint extractWaypointFromReading(NIVA::DataReading d)
  {
      using namespace std;
      string latText, lonText, altText, latBearingText, lonBearingText;
      double lat, lon, alt;

      if (d.format == "NEIL")
      {
          latText = d.dataFields[0];
          lonText = d.dataFields[1];
          altText = d.dataFields[2];
          try
          {
              lat = stod(latText);
              lon = stod(lonText);
              alt = stod(altText);
              return Waypoint(lat,lon,alt);
          }
          catch (const invalid_argument& e)
          {
              throw domain_error(string("Ill-formed NEIL data field: ") + e.what());
          }
      }
      else if (d.format == "NUNO")
      {
          latText = d.dataFields[3];
          lonText = d.dataFields[2];
          altText = d.dataFields[1];
          try
          {
              lat = stod(latText);
              lon = stod(lonText);
              alt = stod(altText);
          }
          catch (const invalid_argument& e)
          {
              throw domain_error(string("Ill-formed NUNO data field: ") + e.what());
          }
          try
          {
              return Waypoint(lat,lon,alt);
          }
          catch (const invalid_argument& e)
          {
              throw domain_error(string("Ill-formed NUNO data field: ") + e.what());
          }
      }
      else // ISMA
      {
          latText = d.dataFields[0];
          latBearingText = d.dataFields[1];
          lonText = d.dataFields[2];
          lonBearingText = d.dataFields[3];
          altText = d.dataFields[4];
          try
          {
              std::string currentlyParsing = "degrees";
              string degText = "";
              string minText = "";
              string secText = "";
              for (unsigned int i = 0; i < latText.size(); ++i)
              {
                  char c = latText[i];
                  if (currentlyParsing == "degrees")
                  {
                      if (c == 'o')
                      {
                          currentlyParsing = "minutes";
                      }
                      else
                      {
                          degText += c;
                      }
                  }
                  else if (currentlyParsing == "minutes")
                  {
                      if (c == '\'')
                      {
                          currentlyParsing = "seconds";
                      }
                      else
                      {
                          minText += c;
                      }
                  }
                  else if (currentlyParsing == "seconds")
                  {
                      if (c == '\'')
                      {
                          currentlyParsing = "secondsSymbol";
                      }
                      else
                      {
                          secText += c;
                      }
                  }
                  else if (currentlyParsing == "secondsSymbol")
                  {
                      if (c == '\'')
                      {
                          currentlyParsing = "done";
                      }
                      else
                      {
                          throw std::domain_error("malformed seconds symbol in data field: " + latText);
                      }
                  }
                  else if (currentlyParsing == "done")
                  {
                      throw std::domain_error("extra characters after seconds symbol in data field: " + latText);
                  }
              }
              if (currentlyParsing != "done")
              {
                  throw std::domain_error("missing symbols in DMS data field: " + latText);
              }
              int degs = stoi(degText);
              int mins = stoi(minText);
              int secs = stoi(secText);
              lat = fromDMS(degs,mins,secs);
              if (lat < 0)
              {
                  throw std::invalid_argument("latitude values in DMS format must be positive.  Positive/negative direction is denoted by a separate bearing indicator.");
              }

              currentlyParsing = "degrees";
              degText = "";
              minText = "";
              secText = "";
              for (unsigned int i = 0; i < lonText.size(); ++i)
              {
                  char c = lonText[i];
                  if (currentlyParsing == "degrees")
                  {
                      if (c == 'o')
                      {
                          currentlyParsing = "minutes";
                      }
                      else
                      {
                          degText += c;
                      }
                  }
                  else if (currentlyParsing == "minutes")
                  {
                      if (c == '\'')
                      {
                          currentlyParsing = "seconds";
                      }
                      else
                      {
                          minText += c;
                      }
                  }
                  else if (currentlyParsing == "seconds")
                  {
                      if (c == '\'')
                      {
                          currentlyParsing = "secondsSymbol";
                      }
                      else
                      {
                          secText += c;
                      }
                  }
                  else if (currentlyParsing == "secondsSymbol")
                  {
                      if (c == '\'')
                      {
                          currentlyParsing = "done";
                      }
                      else
                      {
                          throw std::domain_error("malformed seconds symbol in data field: " + lonText);
                      }
                  }
                  else if (currentlyParsing == "done")
                  {
                      throw std::domain_error("extra characters after seconds symbol in data field: " + lonText);
                  }
              }
              if (currentlyParsing != "done")
              {
                  throw std::domain_error("missing symbols in DMS data field: " + lonText);
              }
              degs = stoi(degText);
              mins = stoi(minText);
              secs = stoi(secText);
              lon = fromDMS(degs,mins,secs);
              if (lon < 0)
              {
                  throw std::invalid_argument("longitude values in DMS format must be positive.  Positive/negative direction is denoted by a separate bearing indicator.");
              }

              alt = stod(altText);
          }
          catch (const invalid_argument& e)
          {
              throw domain_error(string("Ill-formed ISMA data field: ") + e.what());
          }

          if (latBearingText != "N" && latBearingText != "S")
          {
              throw domain_error("Ill-formed ISMA data field: " + latBearingText + " is an invalid bearing indicator.");
          }
          else
          {
              if (latBearingText == "S") lat = -lat;
          }
          if (lonBearingText != "E" && lonBearingText != "W")
          {
              throw domain_error("Ill-formed ISMA data field: " + lonBearingText + " is an invalid bearing indicator.");
          }
          else
          {
              if (lonBearingText == "W") lon = -lon;
          }

          try
          {
                return Waypoint(lat,lon,alt);
          }
          catch (const invalid_argument& e)
          {
                throw domain_error(string("Ill-formed ISMA data field: ") + e.what());
          }
      }
  }

  // Function Stub - to be implemented
  std::vector<Waypoint> extractWaypointsFromLog(std::istream& is)
  {
      return {};
  }

}

}
