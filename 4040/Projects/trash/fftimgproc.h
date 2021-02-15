
#ifndef FFTIMGPROC_H
#define FFTIMGPROC_H

#include <vector>
#include <complex>
#include "imgproc.h"
//#include "LWE.h"
#include "fftw3.h"

namespace img
{

class FFTImgProc
{
  public:

    //! Construct with no content
    FFTImgProc();
   ~FFTImgProc();

    //! delete existing content and leave in a blank state
    void clear();
    //! delete existing content and re-initialize to the input dimensions with value 0.0
    void clear(int nX, int nY, int nC);

    //! Retrieve the width
    int nx() const { return Nx; }
    //! Retrieve the height
    int ny() const { return Ny; }
    //! Retrieve the number of channels
    int depth() const { return Nc; }

    //! Retrieve the (multichannel) complex value at a pixel
    void value( int i, int j, std::vector< std::complex<double> >& pixel) const;
    //! Set the (multichannel) complex value at a pixel.
    void set_value( int i, int j, const std::vector<std::complex<double> >& pixel);

    double kx(int i) const;
    double ky(int j) const;

    //! Copy constructor. Clears existing content.
    FFTImgProc(const FFTImgProc& v);
    //! Copy assignment. Clears existing content.
    FFTImgProc& operator=(const FFTImgProc& v);

    //BEGIN ADDED FUNCTIONS/////////////////////////////////////////////////////////////////////////////////////////////////////
    //! function to execute Linear Wave Estimate
    //void doLinearWaveEstimate();
    /*
    //! function to prepare for the execution of coherentEstimation
    void doCoherentEstimation();
    //! function to implement coherent estimation
    //void coherentEstimation(const ImgProc& input, int channel0, int channel1, float weight0, float weight1, ImgProc& output);
    //! Point struct for fractal flames and warping
    struct Point;
    //! function to prepare for execution of FractalFlameIFS
    //void doIFS(ImgProc& image);
    //! function to actually execute fractal flames algorithm
    //void FractalFlameIFS(const size_t iterations, const ColorLUT& lut, ImgProc& out);
    //! helper function for Polar variation of fractal flames
    float fractalPolar(Point& P);
    //! helper function for Handkerchief variation of fractal flames
    float fractalHandkerchief(Point& P);
    //! helper function for Heart variation of fractal flames
    float fractalHeart(Point& P);
    //! helper function for Disc variation of fractal flames
    float fractalDisc(Point& P);
    //! increases bias in image
    void biasUp();
    //! decreases bias in image
    void biasDown();
    //! performs rms contrast on image
    void rmsContrast();
    //! increases gamma in image
    void gammaUp();
    //! decreases gamma in image
    void gammaDown();
    //! histogram equalization applied independently to each channel
    void histogramEQ();
    //! writes image to OpenEXR file
    void writeOpenEXR();
    //! quanitzes image
    void quantize();
    //! increases brightness in image
    void brightnessUp();
    //! decreases brightness in image
    void brightnessDown();
    //! converts image to grayscale
    void grayscale();
    //! Compute and print minimum, maximum, average, and standard deviation of the values in each channel
    void pixelAnalysis();
    //! Helper functions to quickly calculate min, max, mean, and stdDev values for each channel
    float Iminimum(int x);
    float Imaximum(int x);
    float Iaverage(int x);
    float IstdDev(int x);
    */
    //END ADDED FUNCTIONS/////////////////////////////////////////////////////////////////////////////////////////////////////

    //! converts image to its compliment in-place
    void compliment();
    //! indexing to a particular pixel and channel
    long index(int i, int j, int c) const;
    //! indexing to a particular pixel
    long index(int i, int j) const;

    //! Load an image from a file.  Deletes exising content.
    bool load(const std::string& filename);

    fftw_complex* raw(){ return img_data; }

    void fft_forward();
    void fft_backward();


  private:

    int Nx, Ny, Nc;
    long Nsize;
    fftw_complex * img_data;
    std::vector<fftw_plan> forward;
    std::vector<fftw_plan> backward;
};

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
