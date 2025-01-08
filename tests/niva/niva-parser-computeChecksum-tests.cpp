#include <boost/test/unit_test.hpp>

#include "niva/niva-parser.h"

using namespace GPS;
using namespace NIVA;

BOOST_AUTO_TEST_SUITE( ComputeChecksum )

BOOST_AUTO_TEST_CASE( SingleCharacterData )
{
    BOOST_CHECK_EQUAL( computeChecksum("A") , 65 );
}

BOOST_AUTO_TEST_CASE( TwoCharacterData )
{
    BOOST_CHECK_EQUAL( computeChecksum("AB") , 3 );
}

BOOST_AUTO_TEST_CASE( EmptyData )
{
    BOOST_CHECK_EQUAL( computeChecksum("") , 0 );
}

BOOST_AUTO_TEST_CASE( TypicalData )
{
    BOOST_CHECK_EQUAL( computeChecksum("+45.67,-23.24,231.56") , 28 );
}

BOOST_AUTO_TEST_SUITE_END()
