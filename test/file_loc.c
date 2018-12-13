#include <sys/syscall.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>

struct gps_location {
	int lat_integer;
	int lat_fractional;
    int lng_integer;
    int lng_fractional;
	int accuracy; 
};

static inline int get_gps_location(const char *pathname,
				   struct gps_location *loc)
{
	return syscall(381, pathname, loc);
}

int main(int argc, char **argv)
{
	struct gps_location loc;
	int ret;
    double latitude;
    double longitude;

	if (argc < 2) {
		printf("Usage: %s <path to file>\n", argv[0]);
		exit(1);
	}

	ret = get_gps_location(argv[1], &loc);

	if (ret < 0) {
		printf("Error: %s\n", strerror(errno));
		printf("You probably entered a path in a different FS\n");
		exit(1);
	}

    latitude = loc.lat_integer + loc.lat_fractional*pow(10, -6);
    longitude = loc.lng_integer + loc.lng_fractional*pow(10, -6);

	printf("Latitude: %f\tLongitude: %f\tAccuracy: %d\n", latitude,
		longitude, loc.accuracy);

	printf("URL: https://www.google.com/maps?&z=10&q=%f+%f&ll=%f+%f\n",
		latitude, longitude, latitude, longitude);

	return 0;
}