#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

/* Mounted Test GPS File */
#define TEST_GPS_FILE "/root/proj4/a.txt"

struct gps_location {
    int lat_integer;
    int lat_fractional;
    int lng_integer;
    int lng_fractional;
    int accuracy;
};

void print_gps(struct gps_location loc)
{
    double latitude, longitude;
    latitude = loc.lat_integer + loc.lat_fractional*pow(10, -6);
    longitude = loc.lng_integer + loc.lng_fractional*pow(10, -6);
    printf("Latitude: %f\tLongitude: %f\tAccuracy: %d\n", latitude,
		longitude, loc.accuracy);
}

int main(int argc, char **argv) {
    struct gps_location loc = {
      .lat_integer = 38,
      .lat_fractional = 423232,
      .lng_integer = 128,
      .lng_fractional = 123123,
      .accuracy = 2,
    };
    struct gps_location tmp = {
      .lat_integer = 1,
      .lat_fractional = 1,
      .lng_integer = 1,
      .lng_fractional = 1,
      .accuracy = 1,
    };
    
    int ret = syscall(380, &loc);
    if (ret < 0) {
        perror("Setting location was failed.\n");
    }
    print_gps(loc);
    printf("Setting location complete\n");
    int res = syscall(381, TEST_GPS_FILE, &tmp);
    if (res < 0) {
        printf("error, %d", res);
    }
    print_gps(tmp);
    return 0;
}