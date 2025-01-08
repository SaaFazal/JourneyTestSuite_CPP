#include <boost/test/unit_test.hpp>

#include <string>

#include "niva/niva-parser.h"

using namespace GPS;
using namespace NIVA;

BOOST_AUTO_TEST_SUITE( IsKnownFormat )

BOOST_AUTO_TEST_CASE( NEIL_IsKnownFormat )
{
    BOOST_CHECK( isKnownFormat("NEIL") );
}

BOOST_AUTO_TEST_CASE( ISMA_IsKnownFormat )
{
    BOOST_CHECK( isKnownFormat("ISMA") );
}

BOOST_AUTO_TEST_CASE( NUNO_IsKnownFormat )
{
    BOOST_CHECK( isKnownFormat("NUNO") );
}

BOOST_AUTO_TEST_CASE( UnrecognisedFormats )
{
    BOOST_CHECK( ! isKnownFormat("THOM") );
    BOOST_CHECK( ! isKnownFormat("TAHA") );
    BOOST_CHECK( ! isKnownFormat("AZI") );
    BOOST_CHECK( ! isKnownFormat("PETER") );
    BOOST_CHECK( ! isKnownFormat("BRADLEY") );
}

BOOST_AUTO_TEST_CASE( UncapitalisedFormats )
{
    BOOST_CHECK( isKnownFormat("Neil") );
    BOOST_CHECK( isKnownFormat("Isma") );
}

BOOST_AUTO_TEST_CASE( FormatCodesVeryShort )
{
    BOOST_CHECK( ! isKnownFormat("A") );
}

BOOST_AUTO_TEST_CASE( FormatCodesVeryLong )
{
    const std::string veryLongString(10000,'X');
    BOOST_CHECK( ! isKnownFormat(veryLongString) );
}

BOOST_AUTO_TEST_CASE( ExtraSuffixesNotAccepted )
{
    BOOST_CHECK( ! isKnownFormat("ISMAHANE") );
}

BOOST_AUTO_TEST_CASE( ExtraPrefixesNotAccepted )
{
    BOOST_CHECK( ! isKnownFormat("DRNEIL") );
}

BOOST_AUTO_TEST_CASE( FullDataReadingTextNotAccepted )
{
    BOOST_CHECK( ! isKnownFormat("#NEIL|+45.67,-23.24,231.56,19:44:21|059;") );
}

BOOST_AUTO_TEST_SUITE_END()
