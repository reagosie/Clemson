#include "fft3Danalysis.h"
#include <iostream>
#include <fstream>
#include <assert.h>
using namespace img;




void img::load_3d_fft(const std::vector<ImgProc>& input, FFT3DImgProc& fftoutput)
{
   fftoutput.clear(input[0].nx(), input[0].ny(), (int)input.size());
   for(size_t k = 0; k < input.size(); k++){
     const ImgProc& image = input[k];
     for(int j = 0; j < input.ny(); j++){
        for(int i = 0; i < input.nx(); i++){
           std::vector<float> ci;
  	       image.value(i, j, ci);
           float luminance = ci[0]*0.2126 + ci[1]*0.7152 + ci[2]*0.0722;
  	       std::complex<double> v(luminance, 0.0);
  	       fftoutput.set_value(i, j, k, v);
        }
     }
   }
}

void img::center_origin(FFT3DImgProc& data)
{
  FFT3DImgProc original = data;
#pragma omp parallel for
  for(int k = 0; k < data.nz(); k++){
    for(int j = 0; j < data.ny(); j++){
      for(int i = 0; i < data.nx(); i++){
        int ic = data.nx()/2+i;
        if(ic >= data.nx()){ic = ic - data.nx();}

        std::complex<double> v0 = original.value(i, j, k);
        data.set_value(ic, j, k, v0);
      }
    }
  }

#pragma omp parallel for
  for(int k = 0; k < data.nz(); k++){
    for(int j = 0; j < data.ny(); j++){
      int jc = data.ny()/2+j;
      if(jc >= data.ny()){jc = jc - data.ny();}
      for(int i = 0; i < data.nx(); i++){
        std::complex<double> v0 = original.value(i, j, k);
        original.set_value(i, jc, k, v0);
      }
    }
  }

#pragma omp parallel for
  for(int k = 0; k < data.nz(); k++){
    int kc = data.nz()/2+k;
    if(kc >= data.nz()){kc = kc - data.nz();}
    for(int j = 0; j < data.ny(); j++){
      for(int i = 0; i < data.nx(); i++){
        std::complex<double> v0 = original.value(i, j, k);
        data.set_value(i, j, kc, v0);
      }
    }
  }
}

void img::psd3d(const FFT3DImgProc& fftinput, int smoothing, FFT3DImgProc& psd)
{
  psd.clear(fftinput.nx(), fftinput.ny(), fftinput.nz());
  for(int k = 0; k < psd.nz(); k++){
    std::cout << "Smoothing k=" << k << std::endl;
    for(int j = 0; j < psd.ny(); j++){
#pragma omp parallel for
      for(int i = 0; i < psd.nx(); i++){
        std::complex<double> psdsum(0.0, 0.0);
        int count = 0;
        for(int kk = k - smoothing; kk <= k + smoothing; kk++){
          int kkk = kk;
          if(kkk < 0){kkk += fftinput.nz();}
          if(kkk >= fftinput.nz()){kkk -= fftinput.nz();}
          for(int jj = j - smoothing; jj <= j + smoothing; jj++){
            int jjj = jj;
            if(jjj < 0){jjj += fftinput.ny();}
            if(jjj >= fftinput.ny()){jjj -= fftinput.ny();}
            for(int ii = i - smoothing; ii <= i + smoothing; ii++){
              int iii = ii;
              if(iii < 0){iii += fftinput.nx();}
              if(iii >= fftinput.nx()){iii -= fftinput.nx();}
              std::complex<double> v0 = fftinput.value(iii, jjj, kkk);
              psdsum += v0*conj(v0);
              count += 1;
            }
          }
        }
        if(count > 0){psdsum /= count;}
        psd.set_value(i, j, k, psdsum);
      }
    }
  }
}

void img::psd_slice.xy(const FFT3DImgProc& psd, ImgProc& slice)
{
  slice.clear(psd.nx(), psd.ny(), 3);
  int k = 0;
  for(int j = 0; j < psd.ny(); j++){
#pragma omp parallel for
    for(int i = 0; i < psd.nx(); i++){
      std::complex<double> v0 = psd.value(i, j, k);
      std::vector<float> value(3, 0.0);
      value[0] = v0.real();
      value[1] = v0.real();
      value[2] = v0.real();
      slice.set_value(i, j, value);
    }
  }
}

void img::psd_slice.xz(const FFT3DImgProc& psd, ImgProc& slice)
{
  {
    slice.clear(psd.nx(), psd.nz(), 3);
    int j = 0;
    for(int k = 0; k < psd.nz(); k++){
  #pragma omp parallel for
      for(int i = 0; i < psd.nx(); i++){
        std::complex<double> v0 = psd.value(i, j, k);
        std::vector<float> value(3, 0.0);
        value[0] = v0.real();
        value[1] = v0.real();
        value[2] = v0.real();
        slice.set_value(i, k, value);
      }
    }
  }
}

void img::psd_slice.yz(const FFT3DImgProc& psd, ImgProc& slice)
{
  slice.clear(psd.ny(), psd.nz(), 3);
  int i = 0;
  for(int k = 0; k < psd.nz(); k++){
#pragma omp parallel for
    for(int j = 0; j < psd.ny(); j++){
      std::complex<double> v0 = psd.value(i, j, k);
      std::vector<float> value(3, 0.0);
      value[0] = v0.real();
      value[1] = v0.real();
      value[2] = v0.real();
      slice.set_value(j, k, value);
    }
  }
}
