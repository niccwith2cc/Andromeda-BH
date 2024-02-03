#include <random>
#include <array>
#include <vector>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include "CelestialBody.h"

using std::vector;
using std::array;


template < class T > inline std::ostream& operator << (std::ostream& os, const std::array<T,3>& a) {
    for (auto &elem: a){
        os << elem << ", ";
    }
    return os;
}

double generateRandomDouble(double min, double max, std::mt19937& gen);

int generateRandomInt(int min, int max, std::mt19937& gen);

array<double,3> generateRandomPosition(double& boundary, std::mt19937& gen);

void calculateForce(vector<CelestialBody>& bodies);

void calculateAcceleration(vector<CelestialBody>& bodies);

vector<CelestialBody> generateBodies(int bodynumber, double boundary, int massMin, int massMax);

inline std::string getCurrentDateTime( std::string s ){
    time_t now = time(0);
    struct tm  tstruct;
    char  buf[260];
    tstruct = *localtime(&now);
    if(s=="now")
        strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    else if(s=="date")
        strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
    return  std::string(buf);
};

inline void createDirectory(const std::string& path) { 
    if (mkdir(path.c_str(), 0777) == 0) { 
        std::cout << "Output directory created.\n"; 
    } 
} 