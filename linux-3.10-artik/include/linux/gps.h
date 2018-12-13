#ifndef __LINUX_GPS_H
#define __LINUX_GPS_H

struct gps_location {
  int lat_integer;
  int lat_fractional;
  int lng_integer;
  int lng_fractional;
  int accuracy;
};

void kget_gps_location(struct gps_location *rloc);

#endif