#include "fftimgproc.h"
#include <cmath>
#include "fftanalysis.h"

using namespace img;

LinearWaveEstimate::LinearWaveEstimate(const ImgProc& init, const double dispersion_factor) :
  alpha(dispersion_factor),
  frame_count(0){
    A.clear(init.nx(), init.ny(), init.depth());
    B.clear(init.nx(), init.ny(), init.depth());
  }

double LinearWaveEstimate::dispersion(double kx, double ky){
  double kmag = std::sqrt( kx*kx + ky*ky );
  double freq = alpha * std::sqrt(kmag);
  return freq;
}

void LinearWaveEstimate::ingest(const ImgProc& I){
  FFTImgProc Itilde;
  img::load_fft(I.Itilde);
  Itilde.fft_forward();

  for(int j = 0; j < Itilde.ny(); j++){
#pragma omp parallel for
    for(int i = 0; i < Itilde.nx(); i++){
      std::vector< std::complex<double> > itilde;
      std::vector< std::complex<double> > a;
      std::vector< std::complex<double> > b;
      Itilde.value(i, j, itilde);
      A.value(i, j, a);
      B.value(i, j, b);
      std::vector< std::complex<double> > aupdate = a;
      std::vector< std::complex<double> > bupdate = b;
      std::complex<double> phase(0.0, frame_count * dispersion(Itilde.kx(i), Itilde.ky(j)));
      phase = std::exp(phase);
      double one_over_N = 1.0/(frame_count + 1);
      for(size_t c = 0; c < itilde.size(); c++){
        aupdate[c] += (itilde[c]/phase - b[c]/(phase*phase) - a[c] )*one_over_N;
        bupdate[c] += (itilde[c]*phase - a[c]*(phase*phase) - b[c] )*one_over_N;
      }
      A.set_value(i, j, aupdate);
      B.set_value(i, j, bupdate);
    }
  }
  frame_count++;
}

void LinearWaveEstimate::value(int i, int j, int n, std::vector< std::complex<double> >& amplitude) const{
  std::complex<double> phase(0.0, n * dispersion(A.kx(i),A.ky(j)));
  phase = std::exp(phase);
  std::vector< std::complex<double> > a;
  std::vector< std::complex<double> > b;
  A.value(i, j, a);
  B.value(i, j, b);
  amplitude.resize( a.size() );
  for(size_t c = 0; c < a.size(); c++){
    amplitude[c] = a[c]*phase + b[c]/phase;
  }
}

void img::extract_image(const LinearWaveEstimate& l, int frame, ImgProc& img){
  img.clear(l.getA().nx(), l.getA().ny(), l.getA().depth());

  FFTImgProc fftimg;
  fftimg.clear(img.nx(), img.ny(), img.depth());
  for(int j = 0; j < img.ny(); j++){
    for(int i = 0; i < img.nx(); i++){
      std::vector<std::complex<double> > v;
      l.value(i, j, frame, v);
      fftimg.set_value(i, j, v);
    }
  }

  fftimg.fft_backward();
  for(int j = 0; j < img.ny(); j++){
    for(int i = 0; i < img.nx(); i++){
      std::vector< std::complex<double> > v;
      fftimg.value(i, j, v);
      std::vector<float> iv(v.size());
      for(size_t c = 0; c < v.size(); c++){
        iv[c] = v[c].real();
      }
      img.set_value(i, j, iv);
    }
  }
}
