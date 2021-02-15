#ifndef LWE_H
#define LWE_H

#include <string>
#include <fftimgproc.h>
#include <imgproc.h>

namespace img
{
class LinearWaveEstimate
{
  public:

    LinearWaveEstimate( const ImgProc& init, const double dispersion_factor );
    ~LinearWaveEstimate(){}

    void ingest( const ImgProc& I );

    const FFTImgProc& getA() const { return A; }
    const FFTImgProc& getB() const { return B; }

    void value( int i, int j, int n, std::vector< std::complex<double> >& amplitude) const;

  protected:

    FFTImgProc A;
    FFTImgProc B;

  private:

    double alpha;
    int frame_count;

    double dispersion(double kx, double ky) const;
};

void extract_image( const LinearWaveEstimate& l, int frame, ImgProc& img );

}
#endif
