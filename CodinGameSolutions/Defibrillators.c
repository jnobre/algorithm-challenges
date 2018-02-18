#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>

// represents a defibrillator
struct defib { char* name; double lon; double lat; };

/* Returns the distance between a point A and a point B 
   given their lat(itude) and lon(gitude)*/
double distance(double longA, double latA, double longB, double latB){
    double x = (longB-longA)*cos((latA+latB)/2);
    double y = latB - latA;
    return (sqrt(x*x+y*y) * 6371.0);
}

// Parses a double in string 'str' using atof, and replacing , by . if needed
double toDouble(char *str) {
    for(char *iter = str; *iter != '\0'; iter++)
        if (*iter == ',')
            *iter = '.';
    return atof(str);
}

// Returns a pointer on next char 'c' in 'str'
char *goToNext(char c, char *str) {
    for(; *str != c; str++);
    return str;
}

/* Reads the line 'str', and stores relevant fields in 'defib'
   pretty awful implementation, but necessary for speed*/
void getDefib(char *str, struct defib *res) {
    str = goToNext(';', str);
    res->name = ++str;
    for (int k = 0; k < 3; k++) {
        str = goToNext(';', str);
        *str = '\0';
        str++;
    }
    char *lon = str;
    str = goToNext(',', str);
    *str = '.';
    str = goToNext(';', str);
    char *lat = ++str;
    str = goToNext(',', str);
    *str = '.';
    res->lon = atof(lon);
    res->lat = atof(lat);
}

int main(int argc, char** argv)
{
    char strLon[50];
    char strLat[50];
    int N;
    scanf("%s\n%s\n%d\n", strLon, strLat, &N);
    double lon = toDouble(strLon);
    double lat = toDouble(strLat);
    
    // simple linear search amongst defibrillators
    double min = DBL_MAX;
    char output[100];
    double d;
    char line[256];
    struct defib res;

    for (int i = 0; i < N; i++) {
        getDefib(fgets(line, 256, stdin), &res);
        d = distance(lon, lat, res.lon, res.lat);
        if (d < min) {
            min = d;
            strcpy(output, res.name);
        }
    }
    printf("%s", output);
    return EXIT_SUCCESS;
}
