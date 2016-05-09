// nimg.hpp - 2016.05.09 - Atlee Brink
// structs and methods for 2D RGBA image plotting
// build with lodepng.cpp


#include "lodepng.h"

#include <iostream>
#include <vector>


namespace nimg {
    using namespace std;

    struct srgba {
        union {
            struct {
                unsigned char r, g, b, a;
            };
            unsigned char c[4];
        };
        srgba() {}
        srgba( short r, short g, short b, short a ) : r(r), g(g), b(b), a(a) {}
        srgba( short c[4] ) : r(c[0]), g(c[1]), b(c[2]), a(c[3]) {}
    };

    struct srgba_light {
        union {
            struct {
                short r, g, b, a;
            };
            short s[4];
        };
        srgba_light() {}
        srgba_light( short r, short g, short b, short a ) : r(r), g(g), b(b), a(a) {}
        srgba_light( short s[4] ) : r(s[0]), g(s[1]), b(s[2]), a(s[3]) {}
        operator const short*() const {
            return s;
        }
    };

    struct simg {
        const unsigned w, h;
        vector<srgba> p;
        simg( unsigned w, unsigned h, srgba pel = {0, 0, 0, 0} )
            : w(w), h(h), p( vector<srgba>( w*h, pel ) ) {}
        operator const unsigned char*() const {
            return reinterpret_cast<const unsigned char*>(p.data());
        }
        void add( unsigned x, unsigned y, const short pel[4] ) {
            srgba &rp = p[ y*w + x ];
            for( int c = 0; c < 4; c++ ) {
                short v = pel[c] + rp.c[c];
                rp.c[c] = (unsigned char)( v < 0 ? 0 : v > 255 ? 255 : v );
            }
        }
        void plot( unsigned x, unsigned y, srgba pel ) { p[ y*w + x ] = pel; }
        void save( const string &filename ) const {
            cout << "saving to " << filename << "..." << flush;
            unsigned error = lodepng::encode( filename, *this, w, h );
            if( error ) {
                cerr << "encoder error " << error << ": "
                    << lodepng_error_text(error) << endl;
            }
            cout << " done." << endl;
        }
    };
}
