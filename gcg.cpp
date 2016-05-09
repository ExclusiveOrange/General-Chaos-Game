// gcg.cpp - 2016.05.09 - Atlee Brink
// General Chaos Game plotter

#include "nimg.hpp"

#include <cmath>
#include <cstdio>
#include <iostream>
#include <random>
#include <vector>

namespace {
    using namespace nimg;
    using namespace std;

    random_device rand_dev;
    mt19937 rand_gen( rand_dev() );
    uniform_real_distribution<double> dist_real01(0., 1.);

    double rand01() { return dist_real01( rand_gen ); }
}

int main() {

    const unsigned oversample = 2;
    const unsigned w = 2048*oversample, h = 2048*oversample;
    cout << "allocating memory for a " << w << " x " << h << " image buffer..." << flush;
    simg img( w, h, {0, 0, 0, 255} );
    cout << " done." << endl;

    // list of points
    const int numpoints = 5;
    uniform_int_distribution<> dist_int( 0, numpoints-1 );
    vector< double[2] > points( numpoints );

    // radius: 0.5 will just touch the edges of the image
    const double rad = 0.99 * 0.5;
    // initial rotation: not important but makes each image unique
    const double rot = rand01();

    // distribute points evenly around a circle (not necessary, but makes it easy to compare)
    for( int p = 0; p < numpoints; p++ ) {
        double angle = (rot + ((double)p / numpoints)) * 2. * 3.14159265358979323846;
        points[p][0] = 0.5 + rad*cos(angle);
        points[p][1] = 0.5 + rad*sin(angle);
    }

    // additive RGBA (may be negative, usually want to leave alpha = 0)
    const srgba_light light = {10, 10, 10, 0};

    // a touchy constant: too big and the render appears fuzzy
    const double beta = 1. / 2.;


    // note: may need to try many different starting points to get a good image
    // * note: so far has worked with a single starting point
    double point[2] = { rand01(), rand01() };

    // how many times to plot the point: too big = slow
    const unsigned long numplots = 100000000;

    // do a render
    cout << "rendering " << numplots << " points..." << flush;
    for( unsigned long i = 0; i < numplots; i++ ) {
        
        // pick a point to move toward
        int p = dist_int( rand_gen );

        // compute new coordinates
        point[0] = points[p][0] + beta * ( point[0] - points[p][0] );
        point[1] = points[p][1] + beta * ( point[1] - points[p][1] );

        // plot point
        if( point[0] >= 0. && point[0] < 1. && point[1] >= 0. && point[1] < 1. ) {
            unsigned x = floor(point[0] * w);
            unsigned y = floor(point[1] * h);
            img.add( x, y, light );
        }
    }
    cout << " done." << endl;

    char filename[1024];
    sprintf( filename, "%d-points_%ux%u_%lu-samples.png", numpoints, w, h, numplots );

    img.save( string(filename) );

    return 0;
}
