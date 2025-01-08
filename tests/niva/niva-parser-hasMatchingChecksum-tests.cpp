#include <boost/test/unit_test.hpp>

#include <string>

#include "niva/niva-parser.h"

using namespace GPS;
using namespace NIVA;

BOOST_AUTO_TEST_SUITE( HasMatchingChecksum )

BOOST_AUTO_TEST_CASE( SingleCharacterData )
{
    BOOST_CHECK( hasMatchingChecksum("#XXXX[A]41;") );
}

BOOST_AUTO_TEST_CASE( TwoCharacterData )
{
    BOOST_CHECK( hasMatchingChecksum("#XXXX[AB]03;") );
}

BOOST_AUTO_TEST_CASE( EmptyData )
{
    BOOST_CHECK( hasMatchingChecksum("#XXXX[]00;") );
}

BOOST_AUTO_TEST_CASE( TypicalData )
{
    BOOST_CHECK( hasMatchingChecksum("#NEIL[+45.67,-23.24,231.56,19:44:21]3B;") );
}

BOOST_AUTO_TEST_CASE( LowerCaseChecksumDigit )
{
    BOOST_CHECK( hasMatchingChecksum("#NEIL[+45.67,-23.24,231.56,19:44:21]3b;") );
}

BOOST_AUTO_TEST_CASE( SingleCharacterDataIncorrectChecksum )
{
    BOOST_CHECK( ! hasMatchingChecksum("#XXXX[A]70;") );
}

BOOST_AUTO_TEST_CASE( TypicalDataIncorrectChecksum )
{
    BOOST_CHECK( ! hasMatchingChecksum("#NEIL[+45.67,-23.24,231.56,19:44:21]12;") );
}

BOOST_AUTO_TEST_SUITE_END()
