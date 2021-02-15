
#ifndef FFT3DIMGPROC_H
#define FFT3DIMGPROC_H

#include <vector>
#include <complex>
#include "fftw3.h"

namespace img
{

class FFT3DImgProc
{
  public:

    //! Construct with no content
    FFT3DImgProc();
   ~FFT3DImgProc();

    //! delete existing content and leave in a blank state
    void clear();
    //! delete existing content and re-initialize to the input dimensions with value 0.0
    void clear(int nX, int nY, int nZ);

    //! Retrieve the width
    int nx() const {return Nx;}
    //! Retrieve the height
    int ny() const {return Ny;}
    //! Retrieve the number of channels
    int depth() const {return Nz;}

    //! Retrieve the (multichannel) complex value at a pixel
    std::complex<double> value(int i, int j, int k) const;
    //! Set the (multichannel) complex value at a pixel.
    void set_value(int i, int j, int k, const std::complex<double>& v);

    double kx(int i) const;
    double ky(int j) const;
    double kz(int k) const;

    //! Copy constructor. Clears existing content.
    FFT3DImgProc(const FFT3DImgProc& v);
    //! Copy assignment. Clears existing content.
    FFT3DImgProc& operator=(const FFT3DImgProc& v);

    //! function to execute Linear Wave Estimate
    void doLinearWaveEstimate();

    //! indexing to a particular pixel and channel
    long index(int i, int j, int k) const;

    //! Load an image from a file.  Deletes exising content.
    bool load(const std::string& filename);

    fftw_complex* raw(){return img_data;}

    void fft_forward();
    void fft_backward();


  private:

    int Nx, Ny, Nz;
    long Nsize;
    fftw_complex * img_data;
    std::vector<fftw_plan> forward;
    std::vector<fftw_plan> backward;
};


}

#endif
