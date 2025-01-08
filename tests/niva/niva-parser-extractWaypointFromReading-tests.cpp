#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>
#include <stdexcept>

#include "waypoint.h"
#include "niva/niva-data_reading.h"
#include "niva/niva-parser.h"

using namespace GPS;
using namespace NIVA;

BOOST_AUTO_TEST_SUITE( ExtractWaypointFromReading )

const double percentageAccuracy = 0.0001;
const double absoluteAccuracy = 0.0001;

BOOST_AUTO_TEST_CASE( NEIL )
{
    const DataReading dataEntry = { "NEIL", {"+45.67","-23.24","231.56","19:44:21"} };
    const degrees expectedLatitude = 45.67;
    const degrees expectedLongitude = -23.24;
    const degrees expectedAltitude = 231.56;

    Waypoint actualWaypoint = extractWaypointFromReading(dataEntry);

    BOOST_CHECK_CLOSE( actualWaypoint.latitude() , expectedLatitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.longitude() , expectedLongitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.altitude() , expectedAltitude , percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( ISMA_NW )
{
    const DataReading dataEntry = { "ISMAHANE", {"78o36'45''","N","23o33'56''","W","23.62"} };
    const degrees expectedLatitude = 78.6125;
    const degrees expectedLongitude = -23.565556;
    const degrees expectedAltitude = 23.62;

    Waypoint actualWaypoint = extractWaypointFromReading(dataEntry);

    BOOST_CHECK_CLOSE( actualWaypoint.latitude() , expectedLatitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.longitude() , expectedLongitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.altitude() , expectedAltitude , percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( ISMA_SE )
{
    const DataReading dataEntry = { "ISMAHANE", {"22o47'03''","S","38o14'43''","E","467.21"} };
    const degrees expectedLatitude = -22.784167;
    const degrees expectedLongitude = 38.245278;
    const degrees expectedAltitude = 467.21;

    Waypoint actualWaypoint = extractWaypointFromReading(dataEntry);

    BOOST_CHECK_CLOSE( actualWaypoint.latitude() , expectedLatitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.longitude() , expectedLongitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.altitude() , expectedAltitude , percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( NegativeAltitude )
{
    const DataReading dataEntry = { "NEIL", {"+45.67","-23.24","-456.8","19:44:21"} };
    const degrees expectedLatitude = 45.67;
    const degrees expectedLongitude = -23.24;
    const degrees expectedAltitude = -456.8;

    Waypoint actualWaypoint = extractWaypointFromReading(dataEntry);

    BOOST_CHECK_CLOSE( actualWaypoint.latitude() , expectedLatitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.longitude() , expectedLongitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.altitude() , expectedAltitude , percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( Longitude_above_90 )
{
    const DataReading dataEntry = { "ISMAHANE", {"78o36'45''","N","123o33'56''","W","23.62"} };
    const degrees expectedLatitude = 78.6125;
    const degrees expectedLongitude = -123.565556;
    const degrees expectedAltitude = 23.62;

    Waypoint actualWaypoint = extractWaypointFromReading(dataEntry);

    BOOST_CHECK_CLOSE( actualWaypoint.latitude() , expectedLatitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.longitude() , expectedLongitude , percentageAccuracy );
    BOOST_CHECK_CLOSE( actualWaypoint.altitude() , expectedAltitude , percentageAccuracy );
}

BOOST_AUTO_TEST_CASE( ZeroValues )
{
    const DataReading dataEntry = { "NEIL", {"+0.0","-0.00","0.00","19:44:21"} };

    Waypoint actualWaypoint = extractWaypointFromReading(dataEntry);

    BOOST_CHECK_SMALL( actualWaypoint.latitude() , absoluteAccuracy );
    BOOST_CHECK_SMALL( actualWaypoint.longitude() , absoluteAccuracy );
    BOOST_CHECK_SMALL( actualWaypoint.altitude() , absoluteAccuracy );
}

BOOST_AUTO_TEST_CASE( InvalidAltitudeText )
{
    const DataReading dataEntry = { "NEIL", {"86.24","-23.24","in orbit","19:44:21"} };

    BOOST_CHECK_THROW( extractWaypointFromReading(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidLatitudeText )
{
    const DataReading dataEntry = { "NEIL", {"far north","-23.24","231.56","19:44:21"} };

    BOOST_CHECK_THROW( extractWaypointFromReading(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidLongitudeText )
{
    const DataReading dataEntry = { "NEIL", {"24.78","far east","231.56","19:44:21"} };

    BOOST_CHECK_THROW( extractWaypointFromReading(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_negative_lon )
{
    const DataReading dataEntry = { "ISMAHANE", {"22o47'03''","S","-38o14'26","E","467.21"} };

    BOOST_CHECK_THROW( extractWaypointFromReading(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_missingDegreesSymbol_lon )
{
    const DataReading dataEntry = { "ISMAHANE", {"22o47'03''","S","3814'26''","E","467.21"} };

    BOOST_CHECK_THROW( extractWaypointFromReading(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_missingMinutesSymbol_lon )
{
    const DataReading dataEntry = { "ISMAHANE", {"22o47'03''","S","38o1426''","E","467.21"} };

    BOOST_CHECK_THROW( extractWaypointFromReading(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_missingSecondsSymbol_lon )
{
    const DataReading dataEntry = { "ISMAHANE", {"22o47'03''","S","38o14'26","E","467.21"} };

    BOOST_CHECK_THROW( extractWaypointFromReading(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_not_numeric_lon )
{
    const DataReading dataEntry = { "ISMAHANE", {"22o47'03''","S","over there","E","467.21"} };

    BOOST_CHECK_THROW( extractWaypointFromReading(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidLonBearingSymbol_lon )
{
    const DataReading dataEntry = { "ISMAHANE", {"22o47'03''","N","38o14'43''","Y","467.21"} };

    BOOST_CHECK_THROW( extractWaypointFromReading(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_negative_lat )
{
    const DataReading dataEntry = { "ISMAHANE", {"-22o47'03''","S","38o14'26","E","467.21"} };

    BOOST_CHECK_THROW( extractWaypointFromReading(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_missingDegreesSymbol_lat )
{
    const DataReading dataEntry = { "ISMAHANE", {"2247'03''","S","38o14'26''","E","467.21"} };

    BOOST_CHECK_THROW( extractWaypointFromReading(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_missingMinutesSymbol_lat )
{
    const DataReading dataEntry = { "ISMAHANE", {"22o4703''","S","38o14'26''","E","467.21"} };

    BOOST_CHECK_THROW( extractWaypointFromReading(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_missingSecondsSymbol_lat )
{
    const DataReading dataEntry = { "ISMAHANE", {"22o47'03","S","38o14'26''","E","467.21"} };

    BOOST_CHECK_THROW( extractWaypointFromReading(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidDMSText_not_numeric_lat )
{
    const DataReading dataEntry = { "ISMAHANE", {"up here","S","38o14'26''","E","467.21"} };

    BOOST_CHECK_THROW( extractWaypointFromReading(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidLonBearingSymbol_lat )
{
    const DataReading dataEntry = { "ISMAHANE", {"22o47'03''","X","38o14'43''","E","467.21"} };

    BOOST_CHECK_THROW( extractWaypointFromReading(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidBearingCase )
{
    const DataReading dataEntry = { "ISMAHANE", {"22o47'03''","n","38o14'43''","e","467.21"} };

    BOOST_CHECK_THROW( extractWaypointFromReading(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_CASE( InvalidBearingWithValidPrefix )
{
    const DataReading dataEntry = { "ISMAHANE", {"22o47'03''","Nowhere","38o14'43''","Wherever","467.21"} };

    BOOST_CHECK_THROW( extractWaypointFromReading(dataEntry) , std::domain_error );
}

BOOST_AUTO_TEST_SUITE_END()
