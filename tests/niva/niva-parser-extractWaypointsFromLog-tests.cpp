#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>
#include <sstream>

#include "waypoint.h"
#include "niva/niva-data_reading.h"
#include "niva/niva-parser.h"

using namespace GPS;
using namespace NIVA;

BOOST_AUTO_TEST_SUITE( ExtractWaypointsFromLog )

const double percentageAccuracy = 0.0001;

const std::string validNEILdata = "#NEIL|+45.67,-23.24,231.56,19:44:21|059;";
const std::string validISMAdata = "#Isma|78o36'45'',N,23o42'56'',W,23.62|061;";
const std::string validNUNOdata = "#nuno|2000-01-11T01:10:05Z,56.89,-17.5,+51.4|013;";

const GPS::Waypoint waypointNEIL = GPS::Waypoint(45.67,-23.24,231.56);
const GPS::Waypoint waypointISMA = GPS::Waypoint(78.6125,-23.715556,23.62);
const GPS::Waypoint waypointNUNO = GPS::Waypoint(51.4,-17.5,56.89);

BOOST_AUTO_TEST_CASE( EmptyLog )
{
    std::stringstream dataLog("");
    const unsigned int expectedSize = 0;

    std::vector<Waypoint> waypoints = extractWaypointsFromLog(dataLog);

    BOOST_CHECK_EQUAL( waypoints.size() , expectedSize );
}

BOOST_AUTO_TEST_CASE( OneValidEntry )
{
    std::stringstream dataLog;
    dataLog << validNEILdata;
    const unsigned int expectedSize = 1;
    const Waypoint expectedWaypoint = waypointNEIL;

    std::vector<Waypoint> waypoints = extractWaypointsFromLog(dataLog);

    BOOST_REQUIRE_EQUAL( waypoints.size() , expectedSize );
    const Waypoint actualWaypoint = waypoints.front();
    BOOST_CHECK_CLOSE( actualWaypoint.latitude(), expectedWaypoint.latitude(), percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.longitude(), expectedWaypoint.longitude(), percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.altitude(), expectedWaypoint.altitude(), percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( ThreeValidEntries )
{
    std::stringstream dataLog;
    dataLog << validNEILdata << validISMAdata << validNUNOdata;
    const std::vector<Waypoint> expectedWaypoints = { waypointNEIL, waypointISMA, waypointNUNO};

    std::vector<Waypoint> actualWaypoints = extractWaypointsFromLog(dataLog);

    BOOST_REQUIRE_EQUAL( actualWaypoints.size() , expectedWaypoints.size() );
    for (unsigned int i = 0; i < expectedWaypoints.size(); ++i)
    {
        BOOST_CHECK_CLOSE( actualWaypoints[i].latitude(), expectedWaypoints[i].latitude(), percentageAccuracy );
        BOOST_CHECK_CLOSE( actualWaypoints[i].longitude(), expectedWaypoints[i].longitude(), percentageAccuracy );
        BOOST_CHECK_CLOSE( actualWaypoints[i].altitude(), expectedWaypoints[i].altitude(), percentageAccuracy );
    }
}

BOOST_AUTO_TEST_CASE( IgnoringWhiteSpace )
{
    std::stringstream dataLog;
    dataLog << "\n\n" << validNEILdata << "    " << validISMAdata << " \n \t" << validNUNOdata << std::endl;
    const unsigned int expectedSize = 3;

    std::vector<Waypoint> waypoints = extractWaypointsFromLog(dataLog);

    BOOST_CHECK_EQUAL( waypoints.size() , expectedSize );
}

BOOST_AUTO_TEST_CASE( IllFormedNIVAdataEntries )
{
    std::stringstream dataLog;
    dataLog << validNEILdata << "#2345;" << validISMAdata << "#NUNO|23,A|;" << std::endl;
    const unsigned int expectedSize = 2;

    std::vector<Waypoint> waypoints = extractWaypointsFromLog(dataLog);

    BOOST_CHECK_EQUAL( waypoints.size() , expectedSize );
}

BOOST_AUTO_TEST_CASE( ValidChecksumsAreNotRequired )
{
    const std::string invalidChecksumdataNEIL = "#NEIL|+45.67,-23.24,231.56,19:44:21|113;";
    const std::string invalidChecksumdataISMA = "#ISMA|78o36'45'',N,23o42'56'',W,23.62|074;";
    std::stringstream dataLog;
    dataLog << invalidChecksumdataNEIL << invalidChecksumdataISMA << std::endl;
    const unsigned int expectedSize = 2;

    std::vector<Waypoint> waypoints = extractWaypointsFromLog(dataLog);

    BOOST_CHECK_EQUAL( waypoints.size() , expectedSize );
}

BOOST_AUTO_TEST_CASE( IllformedStructureButValidChecksum )
{
    const std::string illformedNIVAentry = "|A|065;";
    std::stringstream dataLog;
    dataLog << validISMAdata << illformedNIVAentry << validNEILdata << std::endl;
    const unsigned int expectedSize = 2;

    std::vector<Waypoint> waypoints = extractWaypointsFromLog(dataLog);

    BOOST_CHECK_EQUAL( waypoints.size() , expectedSize );
}

BOOST_AUTO_TEST_CASE( UnrecognisedFormat )
{
    const std::string unrecognisedNIVAentry = "#PETE|NNW,40mph,14:21:58|058;";
    std::stringstream dataLog;
    dataLog << validISMAdata << unrecognisedNIVAentry << validNEILdata << std::endl;
    const unsigned int expectedSize = 2;

    std::vector<Waypoint> waypoints = extractWaypointsFromLog(dataLog);

    BOOST_CHECK_EQUAL( waypoints.size() , expectedSize );
}

BOOST_AUTO_TEST_CASE( MissingFields )
{
    const std::string missingFieldsNEILdata = "#NEIL|+45.67,231.56,19:44:21|019;";
    const std::string missingFieldsISMAdata = "#ISMA|78o36'45'',23o42'56'',23.62|036;";
    const std::string missingFieldsNUNOdata = "#NUNO|56.89,-17.5,+51.4|041;";

    std::stringstream dataLog;
    dataLog << validISMAdata << missingFieldsNEILdata << validNEILdata << missingFieldsISMAdata
            << validNUNOdata << missingFieldsNUNOdata << std::endl;
    const unsigned int expectedSize = 3;

    std::vector<Waypoint> waypoints = extractWaypointsFromLog(dataLog);

    BOOST_CHECK_EQUAL( waypoints.size() , expectedSize );
}

BOOST_AUTO_TEST_CASE( ExtraFields )
{
    const std::string extraFieldsNEILdata = "#NEIL|+45.67,-23.24,231.56,19:44:21,15/07/87|027;";
    const std::string extraFieldsISMAdata = "#ISMA|23/11/06,78o36'45'',N,23o42'56'',W,23.62|061;";
    const std::string extraFieldsNUNOdata = "#NUNO|2000-01-11T01:10:05Z,56.89,-17.5,+51.4,A|013;";

    std::stringstream dataLog;
    dataLog << validISMAdata << extraFieldsNEILdata << validNEILdata << extraFieldsISMAdata
            << validNUNOdata << extraFieldsNUNOdata << std::endl;
    const unsigned int expectedSize = 3;

    std::vector<Waypoint> waypoints = extractWaypointsFromLog(dataLog);

    BOOST_CHECK_EQUAL( waypoints.size() , expectedSize );
}

BOOST_AUTO_TEST_CASE( InvalidFields )
{
    const std::string invalidFieldsNEILdata = "#NEIL|+97.67,-23.24,231.56,19:44:21|052;"; // latitude > 90o
    const std::string invalidFieldsNUNOdata = "#NUNO|2000-01-11T01:10:05Z,126.89,-274.5,+51.4|012;"; // longitude > 180o
    const std::string invalidFieldsISMAdata = "#ISMA|7836'45'',N,23o42'56'',W,23.62|082;"; // missing o

    std::stringstream dataLog;
    dataLog << validISMAdata << invalidFieldsNEILdata << validNEILdata << invalidFieldsISMAdata
            << validNUNOdata << invalidFieldsNUNOdata << std::endl;
    const unsigned int expectedSize = 3;

    std::vector<Waypoint> waypoints = extractWaypointsFromLog(dataLog);

    BOOST_CHECK_EQUAL( waypoints.size() , expectedSize );
}

BOOST_AUTO_TEST_SUITE_END()
