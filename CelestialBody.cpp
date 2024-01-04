#include "CelestialBody.h"
#include <vector>
#include <cmath>
#include <algorithm>

using std::vector;

// const double  G = 6.6740105e-11;
// increasing the gravitational constant so observable change is done over a smaller time period
const double  G = 90000000;

template <typename T>
std::vector<T> operator/(const std::vector<T>& vec, const double& s) {
    std::vector<T> res(vec.size());
    for(int i=0;i<vec.size();++i)
        res[i] = vec[i] / s;
    return res;
}

template <typename T, std::size_t N>
std::array<T,N> operator+(const std::array<T,N>& arr1, const std::array<T,N>& arr2) {
    std::array<T,N> res;
    for(int i = 0; i < N; ++i){
        res[i] = arr1[i] + arr2[i];
    }
    return res;
}

template <typename T>
bool operator==(const std::vector<T>& vec1, const std::vector<T>& vec2) {
    if(vec1.size() != vec2.size()) return false;
    for(int i=0;i<vec1.size();++i)
        if (vec1[i] != vec2[i]) return false;
    return true;
}

CelestialBody::CelestialBody(int mass, array<double,3> position, array<double,3> accel, array<double,3> velo, array<double,3> force)
    : _mass(mass), _position(position), _accel(accel), _velo(velo), _force(force) {}

void CelestialBody::setMass(int mass) {
    _mass = mass;
}

int CelestialBody::getMass() {
    return _mass;
}

void CelestialBody::setForce(array<double,3> force) {
    _force = force;
}

array<double,3> CelestialBody::getForce() {
    return _force;
}

void CelestialBody::setPosition(array<double,3> position) {
    _position = position;
}

array<double,3> CelestialBody::getPosition() {
    return _position;
}

void CelestialBody::setAccel(array<double,3> accel) {
    _accel = accel;
}

array<double,3> CelestialBody::getAccel() {
    return _accel;
}

