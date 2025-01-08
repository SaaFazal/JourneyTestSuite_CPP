#include <boost/test/unit_test.hpp>

#include <string>

#include "niva/niva-parser.h"

using namespace GPS;
using namespace NIVA;

BOOST_AUTO_TEST_SUITE( HasMatchingChecksum )

BOOST_AUTO_TEST_CASE( SingleCharacterData )
{
    BOOST_CHECK( hasMatchingChecksum("#XXXX|A|065;") );
}

BOOST_AUTO_TEST_CASE( TwoCharacterData )
{
    BOOST_CHECK( hasMatchingChecksum("#XXXX|AB|003;") );
}

BOOST_AUTO_TEST_CASE( EmptyData )
{
    BOOST_CHECK( hasMatchingChecksum("#XXXX||000;") );
}

BOOST_AUTO_TEST_CASE( TypicalData )
{
    BOOST_CHECK( hasMatchingChecksum("#NEIL|+45.67,-23.24,231.56,19:44:21|059;") );
}

BOOST_AUTO_TEST_CASE( SingleCharacterDataIncorrectChecksum )
{
    BOOST_CHECK( ! hasMatchingChecksum("#XXXX|A|112;") );
}

BOOST_AUTO_TEST_CASE( TypicalDataIncorrectChecksum )
{
    BOOST_CHECK( ! hasMatchingChecksum("#NEIL|+45.67,-23.24,231.56,19:44:21|018;") );
}

BOOST_AUTO_TEST_SUITE_END()
