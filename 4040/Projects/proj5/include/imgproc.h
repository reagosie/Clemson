

#ifndef IMGPROC_H
#define IMGPROC_H

#include <string>
#include <vector>
#include <complex>
#include "fftw3.h"

namespace img
{

class ColorLUT{
  public:
    ColorLUT(double gamma = 1.0);
    ~ColorLUT(){}
    void operator()(const double& value, std::vector<float>& C) const;
  private:
    double gamma;
    std::vector<float> black;
    std::vector< std::vector<float> > bands;
};

class ImgProc
{

  public:

    //! Construct with no content
    ImgProc();
   ~ImgProc();

    //! delete existing content and leave in a blank state
    void clear();
    //! delete existing content and re-initialize to the input dimensions with value 0.0
    void clear(int nX, int nY, int nC);

    //! Load an image from a file.  Deletes exising content.
    bool load( const std::string& filename );

    //! Retrieve the width
    int nx() const { return Nx; }
    //! Retrieve the height
    int ny() const { return Ny; }
    //! Retrieve the number of channels
    int depth() const { return Nc; }

    //! Retrieve the (multichannel) value at a pixel.  Copies the value into parameter 'pixel'.
    void value( int i, int j, std::vector<float>& pixel) const;
    //! Set the (multichannel) value at a pixel.
    void set_value( int i, int j, const std::vector<float>& pixel);

    //! Copy constructor. Clears existing content.
    ImgProc(const ImgProc& v);
    //! Copy assignment. Clears existing content.
    ImgProc& operator=(const ImgProc& v);

    friend void swap(ImgProc& u, ImgProc& v);

    //! multiplies all pixels and channels by a value
    void operator*=(float v);
    //! divides all pixels and channels by a value
    void operator/=(float v);
    //! adds a value to all pixels and channels
    void operator+=(float v);
    //! subtracts a value from all pixels and channels
    void operator-=(float v);

    //BEGIN ADDED FUNCTIONS/////////////////////////////////////////////////////////////////////////////////////////////////////
    //! function to prepare for the execution of coherentEstimation
    void doCoherentEstimation();
    //! function to implement coherent estimation
    void coherentEstimation(const ImgProc& input, int channel0, int channel1, float weight0, float weight1, ImgProc& output);
    //! Point struct for fractal flames and warping
    struct Point;
    //! function to prepare for execution of FractalFlameIFS
    void doIFS(ImgProc& image);
    //! function to actually execute fractal flames algorithm
    void FractalFlameIFS(const size_t iterations, const ColorLUT& lut, ImgProc& out);
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
    //END ADDED FUNCTIONS/////////////////////////////////////////////////////////////////////////////////////////////////////

    //! converts image to its compliment in-place
    void compliment();
     //! indexing to a particular pixel and channel
    long index(int i, int j, int c) const;
    //! indexing to a particular pixel
    long index(int i, int j) const;

    //! returns raw pointer to data (dangerous)
    float* raw(){ return img_data; }

  private:

    int Nx, Ny, Nc;
    long Nsize;
    float * img_data;
};

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

    double alpha;
    int frame_count;

    double dispersion(double kx, double ky) const;

  protected:

    FFTImgProc A;
    FFTImgProc B;

};

void extract_image( const LinearWaveEstimate& l, int frame, ImgProc& img );


//! swaps content of two images
void swap(ImgProc& u, ImgProc& v);


}
#endif