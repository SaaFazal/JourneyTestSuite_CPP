#include <boost/test/unit_test.hpp>

#include <string>
#include <vector>
#include <ios>
#include <ostream>
#include <sstream>

#include "niva/niva-data_reading.h"
#include "niva/niva-parser.h"

using namespace GPS;
using namespace NIVA;

BOOST_AUTO_TEST_SUITE( ParseDataReading )

// Utility function for comparing DataReadings for equality.
void checkDataReadingEqual(const NIVA::DataReading & actual, const NIVA::DataReading & expected);


BOOST_AUTO_TEST_CASE( NEIL )
{
    const std::string nivaReadingText = "#NEIL|+45.67,-23.24,231.56,19:44:21|059;";
    const DataReading expectedData = { "NEIL", {"+45.67","-23.24","231.56","19:44:21"} };

    DataReading actualData = parseDataReading(nivaReadingText);

    checkDataReadingEqual(actualData , expectedData);
}

BOOST_AUTO_TEST_CASE( ISMA )
{
    const std::string nivaReadingText = "#ISMA|78o36'45'',N,23o42'56'',E,23.62|047;";
    const DataReading expectedData = { "ISMA", {"78o36'45''","N","23o42'56''","E","23.62"} };

    DataReading actualData = parseDataReading(nivaReadingText);

    checkDataReadingEqual(actualData , expectedData);
}

BOOST_AUTO_TEST_CASE( UnrecognisedFormat )
{
    const std::string nivaReadingText = "#DAVE|NNW,40mph,14:21:58|045;";
    const DataReading expectedData = { "DAVE", {"NNW","40mph","14:21:58"} };

    DataReading actualData = parseDataReading(nivaReadingText);

    checkDataReadingEqual(actualData , expectedData);
}

BOOST_AUTO_TEST_CASE( JustOneField )
{
    const std::string nivaReadingText = "#XXXX|abc|096;";
    const DataReading expectedData = { "XXXX", {"abc"} };

    DataReading actualData = parseDataReading(nivaReadingText);

    checkDataReadingEqual(actualData , expectedData);
}

BOOST_AUTO_TEST_CASE( SomeEmptyFields )
{
    const std::string nivaReadingText = "#XXXX|abc,,20,|078;";
    const DataReading expectedData = { "XXXX", { "abc", "", "20", "" } };

    DataReading actualData = parseDataReading(nivaReadingText);

    checkDataReadingEqual(actualData , expectedData);
}

BOOST_AUTO_TEST_CASE( NonCapitalisedFormatCode )
{
    const std::string nivaReadingText = "#neil|+45.67,-23.24,231.56,19:44:21|059;";
    const DataReading expectedData = { "NEIL", {"+45.67","-23.24","231.56","19:44:21"} };

    DataReading actualData = parseDataReading(nivaReadingText);

    checkDataReadingEqual(actualData , expectedData);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Displaying vectors directly using the built-in BOOST macros is tricky.
 * For reasons I don't fully understand, just overloading operator<< here doesn't work.
 * See: https://stackoverflow.com/questions/17572583/boost-check-fails-to-compile-operator-for-custom-types
 *
 * So instead we introduce an auxilliary function for formatting the error message for mismatched vectors.
 */

std::ostream& operator<<(std::ostream& outputStream, const std::vector<std::string> & vec)
{
    outputStream << '{';
    for (auto it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) outputStream << ',';
        outputStream << '"' << *it << '"';
    }
    outputStream << '}';
    return outputStream;
}

std::string formatMismatchedFormatCode(std::string actualCode, std::string expectedCode)
{
    std::stringstream outputMessage;
    outputMessage << "parseDataReading() has failed, mismatched format code [ \"" << actualCode << "\" != \"" << expectedCode << "\" ]";
    return outputMessage.str();
}

std::string formatMismatchedFieldData(const std::vector<std::string> & actualFields,
                                      const std::vector<std::string> & expectedFields)
{
    std::stringstream outputMessage;
    outputMessage << "parseDataReading() has failed, mismatched fields [ " << actualFields << " != " << expectedFields << " ]";
    return outputMessage.str();
}

void checkDataReadingEqual(const NIVA::DataReading & actual, const NIVA::DataReading & expected)
{
    BOOST_CHECK_MESSAGE( actual.format == expected.format,
                         formatMismatchedFormatCode(actual.format, expected.format) );

    BOOST_CHECK_MESSAGE( actual.dataFields == expected.dataFields,
                         formatMismatchedFieldData(actual.dataFields,expected.dataFields) );
}

BOOST_AUTO_TEST_SUITE_END()
