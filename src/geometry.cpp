#include <cmath>

#include "geometry.h"

namespace GPS
{
  const unsigned int minutesPerDegree = 60;
  const unsigned int secondsPerMinute = 60;
  const unsigned int degreesInACircle = 360;
  const double pi = 3.141592653589793;
  const degrees fullRotation = degreesInACircle;
  const degrees halfRotation = fullRotation/2;
  const degrees quarterRotation = fullRotation/4;
  const degrees poleLatitude = quarterRotation;
  const degrees antiMeridianLongitude = halfRotation;

  radians degreesToRadians(degrees d)
  {
      return d * pi / halfRotation;
  }

  degrees radiansToDegrees(radians r)
  {
      return r * halfRotation / pi;
  }

  double sinSqr(radians x)
  {
      const double sx = std::sin(x);
      return sx * sx;
  }

  bool isValidLatitude(degrees lat)
  {
      return std::abs(lat) <= poleLatitude;
  }

  bool isValidLongitude(degrees lon)
  {
      return std::abs(lon) <= antiMeridianLongitude;
  }

  degrees normaliseDegrees(degrees d)
  {
      d = fmod(d,fullRotation); // results in range (-360,360)
      if (d <= -halfRotation) d += fullRotation; // results in range (-180,360)
      if (d > halfRotation) d -= fullRotation; // results in range (-180,180]
      return d;
  }

  degrees fromDMS(int wholeDegrees, int wholeMinutes, int wholeSeconds)
  {
      degrees minuteFractionsOfADegree = static_cast<double>(wholeMinutes) / static_cast<double>(minutesPerDegree);
      degrees secondFractionsOfADegree = static_cast<double>(wholeSeconds) / static_cast<double>(secondsPerMinute) / static_cast<double>(minutesPerDegree);

      return wholeDegrees + minuteFractionsOfADegree + secondFractionsOfADegree;
  }
}
