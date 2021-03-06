#ifndef INCLUDED_DEMGRID_H
#define INCLUDED_DEMGRID_H

#include <iostream>

#ifndef INCLUDED_DEMHEADER_H
#include "DEMHeader.h"
#endif

#ifndef INCLUDED_DEMPROFILE_H
#include "DEMProfile.h"
#endif

namespace USGSDEMLib
{

class DEMGrid
{
  // This class uses the DEM library to read a DEM and construct a rectangular
  // grid of floating point values.

 public:

  DEMGrid(float missingDataValue = -32767.0);
  // Missing or void data is replaced with missingDataValue.

  ~DEMGrid();
  
  long read(std::istream& dem, bool incrementalRead = false);
  // Convertes a DEM to a rectangular grid.
  //
  // If incrementalRead is false, the entire DEM will be converted
  // with just one call to read. 0 (zero) is returned if successful, -1
  // if error.
  //
  // If incrementalRead is true, multiple calls to read are
  // required, and the return value is as follows:
  //    >0  --  Number of calls remaining until conversion is complete.
  //   ==0  --  Conversion was successful. No more calls to read required.
  //    -1  --  Error during conversion.
  

  // Accessors
  DEMHeader const& getHeader() const;
  
  long getWidth() const;
  // Returns the number of columns in the grid.

  long getHeight() const;
  // Returns the number of rows in the grid.

  float getElevation(long x, long y) const;
  // Returns the elevation value at (x,y). (0,0) is in the upper left
  // corner, x is positive to the right, y is positive down.
  
  float getMissingDataValue() const;
  // Returns the value used to indicate missing data.

  void getGroundCoords(long x, long y, double& ground_x, double& ground_y);
  // (x,y) are the coordinates of a elevation value in the dem. 
  // (ground_x, ground_y) are the coordinates of that value in the
  // units of the horizontal projection system used by the dem.
  
 private:
  
  float     _missDataVal;
  DEMHeader _header;
  long      _width;
  long      _height;
  float*    _grid;
  bool      _firstTime;   // Whether or not we've started reading the DEM.
  long      _curProfile;
  std::vector<DEMProfile> _profiles;  // Used by fillUTM()
  double _northwest_x, _northwest_y;

  void setElevation(long x, long y, long val);
  long fillGeographic(std::istream& dem, bool incrementalRead);
  long fillUTM(std::istream& dem, bool incrementalRead);
};

inline void DEMGrid::setElevation(long x, long y, long val)
{
  if (_grid == 0)
    return;

  _grid[(_width * y) + x] = static_cast<float>(val);
}

} // namespace USGSDEMLib

#endif  // INCLUDED_DEMGRID_HXX
