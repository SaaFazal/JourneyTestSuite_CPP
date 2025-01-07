#include <cmath>
#include <cassert>

#include "geometry.h"
#include "waypoint.h"
#include "earth.h"

namespace GPS
{
  namespace Earth
  {
      const metres meanRadius = 6371008.8;
      const metres equatorialCircumference = 40075160;
      const metres polarCircumference = 40008000;

      metres circumferenceAtLatitude(degrees lat)
      {
          assert (isValidLatitude(lat));

          return equatorialCircumference * std::cos(degreesToRadians(lat));
      }

      degrees latitudeSubtendedBy(metres northSouthDistance)
      {
          assert (! (northSouthDistance > polarCircumference));

          return (northSouthDistance / polarCircumference) * fullRotation;
      }

      degrees longitudeSubtendedBy(metres eastWestDistance, degrees lat)
      {
          assert (isValidLatitude(lat));

          metres circumferenceAtThisLatitude = circumferenceAtLatitude(lat);

          assert (eastWestDistance <= circumferenceAtThisLatitude);

          if (circumferenceAtThisLatitude == 0)
          {
              return 0; // No longitude at poles.
          }
          else
          {
              return (eastWestDistance / circumferenceAtThisLatitude) * fullRotation;
          }
      }

      bool isValidAltitude(metres altitude)
      {
          return altitude >= -meanRadius;
      }

  }
}
