#include <boost/test/unit_test.hpp>

#include <string>

#include "niva/niva-parser.h"

using namespace GPS;
using namespace NIVA;

BOOST_AUTO_TEST_SUITE( IsWellformedDataReading )

BOOST_AUTO_TEST_CASE( WellFormedTypicalDataReadings )
{
    BOOST_CHECK( isWellformedDataReading("#NEIL[+45.67,-23.24,231.56,19:44:21]3B;") );
    BOOST_CHECK( isWellformedDataReading("#ISMAHANE[78o36'45'',N,23o42'56'',W,23.62]43;") );
    BOOST_CHECK( isWellformedDataReading("#VISHAL[09:33:21,78o36'45'',N,23o13'56'',E,56.89]65;") );
}

BOOST_AUTO_TEST_CASE( WellFormedMixedCaseFormatCodes )
{
    BOOST_CHECK( isWellformedDataReading("#neil[+45.67,-23.24,231.56,19:44:21]3B;") );
    BOOST_CHECK( isWellformedDataReading("#IsmaHane[78o36'45'',N,23o42'56'',W,23.62]43;") );
    BOOST_CHECK( isWellformedDataReading("#vishAL[09:33:21,78o36'45'',N,23o13'56'',E,56.89]65;") );
}

BOOST_AUTO_TEST_CASE( WellFormedDifferentFormatCodeLengths )
{
    BOOST_CHECK( isWellformedDataReading("#X[1]64;") );
    BOOST_CHECK( isWellformedDataReading("#JO[77,88,99]64;") );
    BOOST_CHECK( isWellformedDataReading("#AZI[33,44,55,66]64;") );
    BOOST_CHECK( isWellformedDataReading("#SIMON[1,2,3,4,5]64;") );
    BOOST_CHECK( isWellformedDataReading("#DORATHA[0,0,0]64;") );
}

BOOST_AUTO_TEST_CASE( IncorrectStartSymbol )
{
    BOOST_CHECK( ! isWellformedDataReading("!NEIL[+45.67,-23.24,231.56,19:44:21]3B;") );
}

BOOST_AUTO_TEST_CASE( MissingStartSymbol )
{
    BOOST_CHECK( ! isWellformedDataReading("NEIL[+45.67,-23.24,231.56,19:44:21]3B;") );
}

BOOST_AUTO_TEST_CASE( MissingEndSymbol )
{
    BOOST_CHECK( ! isWellformedDataReading("#NEIL[+45.67,-23.24,231.56,19:44:21]3B") );
}

BOOST_AUTO_TEST_CASE( MissingFormatCode )
{
    BOOST_CHECK( ! isWellformedDataReading("#[+45.67,-23.24,231.56,19:44:21]3B;") );
}

BOOST_AUTO_TEST_CASE( MissingChecksum )
{
    BOOST_CHECK( ! isWellformedDataReading("#NEIL[+45.67,-23.24,231.56,19:44:21];") );
}

BOOST_AUTO_TEST_CASE( InvalidChecksumLength )
{
    BOOST_CHECK( ! isWellformedDataReading("#NEIL[+45.67,-23.24,231.56,19:44:21]1;") );
    BOOST_CHECK( ! isWellformedDataReading("#NEIL[+45.67,-23.24,231.56,19:44:21]123;") );
    BOOST_CHECK( ! isWellformedDataReading("#NEIL[+45.67,-23.24,231.56,19:44:21]1234;") );
}

BOOST_AUTO_TEST_CASE( InvalidChecksumCharacters )
{
    BOOST_CHECK( ! isWellformedDataReading("#NEIL[+45.67,-23.24,231.56,19:44:21]h1;") );
    BOOST_CHECK( ! isWellformedDataReading("#NEIL[+45.67,-23.24,231.56,19:44:21]to;") );
    BOOST_CHECK( ! isWellformedDataReading("#NEIL[+45.67,-23.24,231.56,19:44:21] 3;") );
}

BOOST_AUTO_TEST_CASE( WellFormedUnknownFormats )
{
    BOOST_CHECK( isWellformedDataReading("#PETE[77,88,99]64;") );
    BOOST_CHECK( isWellformedDataReading("#DAVE[77,88,99]64;") );
}

BOOST_AUTO_TEST_CASE( WellFormedOneField )
{
    BOOST_CHECK( isWellformedDataReading("#XXXX[77]64;") );
}

BOOST_AUTO_TEST_CASE( WellFormedTwoFields )
{
    BOOST_CHECK( isWellformedDataReading("#XXXX[77,A]57;") );
}

BOOST_AUTO_TEST_CASE( WellFormedEmptyField )
{
    BOOST_CHECK( isWellformedDataReading("#XXXX[]00;") );
}

BOOST_AUTO_TEST_CASE( WellFormedManyFields )
{
    const std::string commas(1000,','); // 1000 fields
    BOOST_CHECK( isWellformedDataReading("#XXXX[" + commas + "]62;") );
}

BOOST_AUTO_TEST_CASE( WellFormedCodeEdgeLetters )
{
    BOOST_CHECK( isWellformedDataReading("#AAAA[77]64;") );
    BOOST_CHECK( isWellformedDataReading("#ZZZZ[77]64;") );
    BOOST_CHECK( isWellformedDataReading("#aaaa[77]64;") );
    BOOST_CHECK( isWellformedDataReading("#zzzz[77]64;") );
}

BOOST_AUTO_TEST_CASE( WellFormedChecksumEdgeDigits )
{
    BOOST_CHECK( isWellformedDataReading("#XXXX[77]00;") );
    BOOST_CHECK( isWellformedDataReading("#XXXX[77]99;") );
    BOOST_CHECK( isWellformedDataReading("#XXXX[77]AA;") );
    BOOST_CHECK( isWellformedDataReading("#XXXX[77]FF;") );
}

BOOST_AUTO_TEST_CASE( WellFormedChecksumLowerCaseDigits )
{
    BOOST_CHECK( isWellformedDataReading("#XXXX[77]a3;") );
    BOOST_CHECK( isWellformedDataReading("#XXXX[77]4c;") );
}

BOOST_AUTO_TEST_CASE( MissingSuffix )
{
    BOOST_CHECK( ! isWellformedDataReading("") );
    BOOST_CHECK( ! isWellformedDataReading("#") );
    BOOST_CHECK( ! isWellformedDataReading("#XXXX") );
    BOOST_CHECK( ! isWellformedDataReading("#XXXX[") );
    BOOST_CHECK( ! isWellformedDataReading("#XXXX[23") );
    BOOST_CHECK( ! isWellformedDataReading("#XXXX[23,") );
    BOOST_CHECK( ! isWellformedDataReading("#XXXX[23,A") );
}

BOOST_AUTO_TEST_CASE( IllformedFormatCode )
{
    BOOST_CHECK( ! isWellformedDataReading("#1234|77]64;") );
    BOOST_CHECK( ! isWellformedDataReading("#$%&-|77]64;") );
    BOOST_CHECK( ! isWellformedDataReading("#A*C,|77]64;") );
}

BOOST_AUTO_TEST_CASE( InvalidReservedCharInField )
{
    BOOST_CHECK( ! isWellformedDataReading("#XXXX[23,#,A]64;") );
    BOOST_CHECK( ! isWellformedDataReading("#XXXX[23,;,A]64;") );
    BOOST_CHECK( ! isWellformedDataReading("#XXXX[23,[,A]64;") );
    BOOST_CHECK( ! isWellformedDataReading("#XXXX[23,],A]64;") );
}

BOOST_AUTO_TEST_SUITE_END()
