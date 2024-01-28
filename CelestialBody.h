#ifndef CELESTIALBODY_H
#define CELESTIALBODY_H

#include <array>

using std::array;

template <typename T, std::size_t N>
std::array<T,N> operator/(const std::array<T,N>& arr, const double& s) {
    std::array<T,N> res;
    for(int i = 0; i < arr.size(); ++i)
        res[i] = arr[i] / s;
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

template <typename T, std::size_t N>
bool operator==(const std::array<T,N>& arr1, const std::array<T,N>& arr2) {
    if(arr1.size() != arr2.size()) return false;
    for(int i=0;i<arr1.size();++i)
        if (arr1[i] != arr2[i]) return false;
    return true;
}

class CelestialBody{
private:
    array<double,3> _position = {0.0,0.0,0.0};
    array<double,3> _accel = {0.0,0.0,0.0};
    array<double,3> _velo = {0.0,0.0,0.0};
    array<double,3> _force = {0.0,0.0,0.0};
    int _mass;

public:
    CelestialBody(int, array<double,3>, array<double,3>, array<double,3>, array<double,3>);

    void setMass(int);

    int getMass();

    void setForce(array<double,3>);

    array<double,3> getForce();

    void setPosition(array<double,3>);

    array<double,3> getPosition();

    void setVelo(array<double,3>);

    array <double,3> getVelo();

    void setAccel(array<double,3>);

    array<double,3> getAccel();

    double CalcR(CelestialBody& const);

    double CalcForce(CelestialBody& const);
    
    array<double,3> CalcCompF(CelestialBody& const);

    array<double,3> CalcCompA(CelestialBody& const);
};

#endif