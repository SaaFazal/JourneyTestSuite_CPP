#include <boost/test/unit_test.hpp>

#include <string>

#include "niva/niva-parser.h"

using namespace GPS;
using namespace NIVA;

BOOST_AUTO_TEST_SUITE( IsWellformedDataReading )

BOOST_AUTO_TEST_CASE( WellFormedTypicalDataReadings )
{
    BOOST_CHECK( isWellformedDataReading("#NEIL|+45.67,-23.24,231.56,19:44:21|059;") );
    BOOST_CHECK( isWellformedDataReading("#ISMA|78o36'45'',N,23o42'56'',W,23.62|067;") );
    BOOST_CHECK( isWellformedDataReading("#NUNO|2000-01-11T01:10:05Z,56.89,-17.5,+51.4|013;") );
}

BOOST_AUTO_TEST_CASE( WellFormedMixedCaseFormatCodes )
{
    BOOST_CHECK( isWellformedDataReading("#neil|+45.67,-23.24,231.56,19:44:21|059;") );
    BOOST_CHECK( isWellformedDataReading("#IsmA|78o36'45'',N,23o42'56'',W,23.62|067;") );
    BOOST_CHECK( isWellformedDataReading("#NuNo|2000-01-11T01:10:05Z,56.89,-17.5,+51.4|013;") );
}

BOOST_AUTO_TEST_CASE( WellFormedDifferentFormatCodeLengths )
{
    BOOST_CHECK( isWellformedDataReading("#X|1|100;") );
    BOOST_CHECK( isWellformedDataReading("#JO|77,88,99|100;") );
    BOOST_CHECK( isWellformedDataReading("#AZI|33,44,55,66|100;") );
    BOOST_CHECK( isWellformedDataReading("#SIMON|1,2,3,4,5|100;") );
    BOOST_CHECK( isWellformedDataReading("#DORATHA|0,0,0|100;") );
}

BOOST_AUTO_TEST_CASE( IncorrectStartSymbol )
{
    BOOST_CHECK( ! isWellformedDataReading("!NEIL|+45.67,-23.24,231.56,19:44:21|059;") );
}

BOOST_AUTO_TEST_CASE( MissingStartSymbol )
{
    BOOST_CHECK( ! isWellformedDataReading("NEIL|+45.67,-23.24,231.56,19:44:21|059;") );
}

BOOST_AUTO_TEST_CASE( MissingEndSymbol )
{
    BOOST_CHECK( ! isWellformedDataReading("#NEIL|+45.67,-23.24,231.56,19:44:21|059") );
}

BOOST_AUTO_TEST_CASE( MissingFormatCode )
{
    BOOST_CHECK( ! isWellformedDataReading("#|+45.67,-23.24,231.56,19:44:21|059;") );
}

BOOST_AUTO_TEST_CASE( MissingChecksum )
{
    BOOST_CHECK( ! isWellformedDataReading("#NEIL|+45.67,-23.24,231.56,19:44:21|;") );
}

BOOST_AUTO_TEST_CASE( InvalidChecksumLength )
{
    BOOST_CHECK( ! isWellformedDataReading("#NEIL|+45.67,-23.24,231.56,19:44:21|1;") );
    BOOST_CHECK( ! isWellformedDataReading("#NEIL|+45.67,-23.24,231.56,19:44:21|12;") );
    BOOST_CHECK( ! isWellformedDataReading("#NEIL|+45.67,-23.24,231.56,19:44:21|1234;") );
}

BOOST_AUTO_TEST_CASE( InvalidChecksumCharacters )
{
    BOOST_CHECK( ! isWellformedDataReading("#NEIL|+45.67,-23.24,231.56,19:44:21|1A1;") );
    BOOST_CHECK( ! isWellformedDataReading("#NEIL|+45.67,-23.24,231.56,19:44:21|two;") );
    BOOST_CHECK( ! isWellformedDataReading("#NEIL|+45.67,-23.24,231.56,19:44:21|0 3;") );
}

BOOST_AUTO_TEST_CASE( WellFormedUnknownFormats )
{
    BOOST_CHECK( isWellformedDataReading("#PETE|77,88,99|100;") );
    BOOST_CHECK( isWellformedDataReading("#DAVE|77,88,99|100;") );
}

BOOST_AUTO_TEST_CASE( WellFormedOneField )
{
    BOOST_CHECK( isWellformedDataReading("#XXXX|77|100;") );
}

BOOST_AUTO_TEST_CASE( WellFormedTwoFields )
{
    BOOST_CHECK( isWellformedDataReading("#XXXX|77,A|087;") );
}

BOOST_AUTO_TEST_CASE( WellFormedEmptyField )
{
    BOOST_CHECK( isWellformedDataReading("#XXXX||000;") );
}

BOOST_AUTO_TEST_CASE( WellFormedManyFields )
{
    const std::string commas(1000,','); // 1000 fields
    BOOST_CHECK( isWellformedDataReading("#XXXX|" + commas + "|098;") );
}

BOOST_AUTO_TEST_CASE( WellFormedCodeEdgeLetters )
{
    BOOST_CHECK( isWellformedDataReading("#AAAA|77|100;") );
    BOOST_CHECK( isWellformedDataReading("#ZZZZ|77|100;") );
    BOOST_CHECK( isWellformedDataReading("#aaaa|77|100;") );
    BOOST_CHECK( isWellformedDataReading("#zzzz|77|100;") );
}

BOOST_AUTO_TEST_CASE( WellFormedChecksumEdgeDigits )
{
    BOOST_CHECK( isWellformedDataReading("#XXXX|77|000;") );
    BOOST_CHECK( isWellformedDataReading("#XXXX|77|999;") );
}

BOOST_AUTO_TEST_CASE( MissingSuffix )
{
    BOOST_CHECK( ! isWellformedDataReading("") );
    BOOST_CHECK( ! isWellformedDataReading("#") );
    BOOST_CHECK( ! isWellformedDataReading("#XXXX") );
    BOOST_CHECK( ! isWellformedDataReading("#XXXX|") );
    BOOST_CHECK( ! isWellformedDataReading("#XXXX|23") );
    BOOST_CHECK( ! isWellformedDataReading("#XXXX|23,") );
    BOOST_CHECK( ! isWellformedDataReading("#XXXX|23,A") );
}

BOOST_AUTO_TEST_CASE( IllformedFormatCode )
{
    BOOST_CHECK( ! isWellformedDataReading("#1234|77|100;") );
    BOOST_CHECK( ! isWellformedDataReading("#$%&-|77|100;") );
    BOOST_CHECK( ! isWellformedDataReading("#A*C,|77|100;") );
}

BOOST_AUTO_TEST_CASE( InvalidReservedCharInField )
{
    BOOST_CHECK( ! isWellformedDataReading("#XXXX|23,#,A|100;") );
    BOOST_CHECK( ! isWellformedDataReading("#XXXX|23,;,A|100;") );
    BOOST_CHECK( ! isWellformedDataReading("#XXXX|23,|,A|100;") );
}

BOOST_AUTO_TEST_SUITE_END()
