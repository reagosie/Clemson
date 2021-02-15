
#ifndef FFT3DANALYSIS_H
#define FFT3DANALYSIS_H

#include <string>
#include "3Dfftimgproc.h"
#include "imgproc.h"

namespace img
{

void load_fft(const std::vector<ImgProc>& input, FFT3DImgProc& fftoutput);

void center_origin(FFT3DImgProc& data);


void psd3d(const FFT3DImgProc& fftinput, int smoothing, FFT3DImgProc& psd);

void psd_slice_xy(const FFT3DImgProc& psd, ImgProc& slice);
void psd_slice_xz(const FFT3DImgProc& psd, ImgProc& slice);
void psd_slice_yz(const FFT3DImgProc& psd, ImgProc& slice);
void psd_slice_rotated(const FFT3DImgProc& psd, const float angle, ImgProc& slice);

}
#endif
