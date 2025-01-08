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

const std::string validNEILdata = "#NEIL[+45.67,-23.24,231.56,19:44:21]3B;";
const std::string validISMAdata = "#Ismahane[78o36'45'',N,23o42'56'',W,23.62]3D;";
const std::string validVISHALdata = "#VISHAL[09:33:21,78o36'45'',S,23o13'56'',E,56.89]17;";
const std::string validALICIAdata = "#ALICIA[78o36.75',S,23o42.9333',W,28.90]2C;";

const GPS::Waypoint waypointNEIL = GPS::Waypoint(45.67, -23.24, 231.56);
const GPS::Waypoint waypointISMA = GPS::Waypoint(78.6125, -23.715556, 23.62);
const GPS::Waypoint waypointVISHAL = GPS::Waypoint(-78.6125, 23.232222, 56.89);
const GPS::Waypoint waypointALICIA = GPS::Waypoint(-78.6125, -23.715556, 28.90);

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

BOOST_AUTO_TEST_CASE( FourValidEntries )
{
    std::stringstream dataLog;
    dataLog << validNEILdata << validISMAdata << validVISHALdata << validALICIAdata;
    const std::vector<Waypoint> expectedWaypoints = { waypointNEIL, waypointISMA, waypointVISHAL, waypointALICIA};

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
    dataLog << "\n\n" << validNEILdata << "   \n \t " << validISMAdata << std::endl;
    const unsigned int expectedSize = 2;

    std::vector<Waypoint> waypoints = extractWaypointsFromLog(dataLog);

    BOOST_CHECK_EQUAL( waypoints.size() , expectedSize );
}

BOOST_AUTO_TEST_CASE( IllFormedNIVAdataEntries )
{
    std::stringstream dataLog;
    dataLog << validNEILdata << "#2345;" << validISMAdata << "#NUNO[23,A];" << std::endl;
    const unsigned int expectedSize = 2;

    std::vector<Waypoint> waypoints = extractWaypointsFromLog(dataLog);

    BOOST_CHECK_EQUAL( waypoints.size() , expectedSize );
}

BOOST_AUTO_TEST_CASE( ValidChecksumsAreRequired )
{
    const std::string invalidChecksumdataNEIL = "#NEIL[+45.67,-23.24,231.56,19:44:21]71;";
    const std::string invalidChecksumdataISMA = "#ISMAHANE[78o36'45'',N,23o42'56'',W,23.62]4A;";
    std::stringstream dataLog;
    dataLog << invalidChecksumdataNEIL << invalidChecksumdataISMA << std::endl;
    const unsigned int expectedSize = 0;

    std::vector<Waypoint> waypoints = extractWaypointsFromLog(dataLog);

    BOOST_CHECK_EQUAL( waypoints.size() , expectedSize );
}

BOOST_AUTO_TEST_CASE( IllformedStructureButValidChecksum )
{
    const std::string illformedNIVAentry = "[A]41;";
    std::stringstream dataLog;
    dataLog << validISMAdata << illformedNIVAentry << validNEILdata << std::endl;
    const unsigned int expectedSize = 2;

    std::vector<Waypoint> waypoints = extractWaypointsFromLog(dataLog);

    BOOST_CHECK_EQUAL( waypoints.size() , expectedSize );
}

BOOST_AUTO_TEST_CASE( UnrecognisedFormat )
{
    const std::string unrecognisedNIVAentry = "#PETE[NNW,40mph,14:21:58]3A;";
    std::stringstream dataLog;
    dataLog << validISMAdata << unrecognisedNIVAentry << validNEILdata << std::endl;
    const unsigned int expectedSize = 2;

    std::vector<Waypoint> waypoints = extractWaypointsFromLog(dataLog);

    BOOST_CHECK_EQUAL( waypoints.size() , expectedSize );
}

BOOST_AUTO_TEST_CASE( MissingFields )
{
    const std::string missingFieldsNEILdata = "#NEIL[+45.67,231.56,19:44:21]13;";
    const std::string missingFieldsISMAdata = "#ISMAHANE[78o36'45'',23o42'56'',23.62]24;";
    const std::string missingFieldsVISHALdata = "#VISHAL[09:33:21,78o36'45'',S,23o13'56'',E]17;";

    std::stringstream dataLog;
    dataLog << validISMAdata << missingFieldsNEILdata << validNEILdata << missingFieldsISMAdata << missingFieldsVISHALdata << std::endl;
    const unsigned int expectedSize = 2;

    std::vector<Waypoint> waypoints = extractWaypointsFromLog(dataLog);

    BOOST_CHECK_EQUAL( waypoints.size() , expectedSize );
}

BOOST_AUTO_TEST_CASE( ExtraFields )
{
    const std::string extraFieldsNEILdata = "#NEIL[+45.67,-23.24,231.56,19:44:21,15/07/87]1B;";
    const std::string extraFieldsISMAdata = "#ISMAHANE[23/11/06,78o36'45'',N,23o42'56'',W,23.62]3D;";
    const std::string extraFieldsVISHALdata = "#VISHAL[09:33:21,30/06/17,78o36'45'',S,23o13'56'',E,56.89]17;";

    std::stringstream dataLog;
    dataLog << validISMAdata << extraFieldsNEILdata << validNEILdata << extraFieldsISMAdata << extraFieldsVISHALdata << std::endl;
    const unsigned int expectedSize = 2;

    std::vector<Waypoint> waypoints = extractWaypointsFromLog(dataLog);

    BOOST_CHECK_EQUAL( waypoints.size() , expectedSize );
}

BOOST_AUTO_TEST_CASE( InvalidFields )
{
    const std::string invalidFieldsNEILdata = "#NEIL[+97.67,-23.24,231.56,19:44:21]34;"; // latitude > 90o
    const std::string invalidFieldsISMAdata = "#ISMAHANE[7836'45'',N,23o42'56'',W,23.62]52;"; // missing o
    const std::string invalidFieldsVISHALdata = "#VISHAL[09:33:21,78o36'45'',S,23o13'56,E,56.89]17;"; // missing ''

    std::stringstream dataLog;
    dataLog << validISMAdata << invalidFieldsNEILdata << validNEILdata << invalidFieldsISMAdata << std::endl;
    const unsigned int expectedSize = 2;

    std::vector<Waypoint> waypoints = extractWaypointsFromLog(dataLog);

    BOOST_CHECK_EQUAL( waypoints.size() , expectedSize );
}

BOOST_AUTO_TEST_SUITE_END()
