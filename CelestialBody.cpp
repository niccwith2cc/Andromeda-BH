#include <array>
#include <cmath>
#include <algorithm>
#include "CelestialBody.h"

using std::array;
// const double  G = 6.6740105e-11;
// increasing the gravitational constant so observable change is done over a smaller time period
const double  G = 90000000;

CelestialBody::CelestialBody(int mass, array<double,3> position, array<double,3> accel, array<double,3> velo, array<double,3> force)
    : _mass(mass), _position(position), _accel(accel), _velo(velo), _force(force) {}

void CelestialBody::setMass(int mass){
    _mass = mass;
}

int CelestialBody::getMass(){
    return _mass;
}   

void CelestialBody::setForce(array<double, 3> force){
    _force = force;
}

array<double, 3> CelestialBody::getForce(){
    return _force;
}

void CelestialBody::setPosition(array<double, 3> position){
    _position = position;
}

array<double, 3> CelestialBody::getPosition(){
    return _position;
}

void CelestialBody::setAccel(array<double, 3> accel){
    _accel = accel;
}

array<double, 3> CelestialBody::getAccel(){
    return _accel;
}

double CelestialBody::CalcR(CelestialBody body2){
    array<double, 3> position2 = body2.getPosition();
    return sqrt(pow(position2[0]-position[0],2)+pow(position2[1]-position[1],2)+pow(position2[2]-position[2],2));
}

//note that whenever the planets are in the same plane as each other, 2 components will be 0 and cause the force to go to inf

// an alternative solution would be to take the absolute norm between the two planets and calculate the total force between them
// once the total force is calculated, the components can be extracted for use to calculate the summation force for all the bodies acting
// on the that body.

// Calculating the force between two bodies F = -G M1 M2 / (r^2) with r being the second norm of a 3D position array between M1 and M2 
double CelestialBody::CalcForce(CelestialBody body2){
    //array<double> position2 = body2.getPosition();
    double instantForce = -G*mass*body2.getMass()/(pow(CalcR(body2),2));
    return instantForce;
}

// Calculating the components of the force given by CalcForce, by taking the array dot product. F12*r21/|r21|.
array<double, 3> CelestialBody::CalcCompF(CelestialBody body2){
    array<double, 3> position2 = body2.getPosition();
    double r = CalcR(body2);
    double tol = 1e-4;
    if (r < tol) return array<double>(3);
    double FR = CalcForce(body2)/r;
    return {(position[0]-position2[0])*FR,(position[1]-position2[1])*FR,(position[2]-position2[2])*FR};
}

// Calculating the components of the acceleration given by CalcCompF, by dividing by the mass of each body.
array<double, 3> CelestialBody::CalcCompA(CelestialBody const body2){
    array<double, 3> instantAcc = CalcCompF(body2)/mass; //DivideVectorByScalar(CalcCompF(body2),mass);
    return instantAcc;
}
