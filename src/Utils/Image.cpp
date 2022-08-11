#include "STDInclude.hpp"

namespace Utils
{
	namespace Image
	{
		std::vector<int> BoxesForGauss(int sigma, int n)  // standard deviation, number of boxes
		{
			float wIdeal = sqrt((12 * sigma * sigma / n) + 2);  // Ideal averaging filter w 
			int wl = floor(wIdeal);
			if (wl % 2 == 0) wl--;
			int wu = wl + 2;

			float mIdeal = (12 * sigma * sigma - n * wl * wl - 4 * n * wl - 3 * n) / (-4 * wl - 4);
			int m = round(mIdeal);
			// var sigmaActual = Math.sqrt( (m*wl*wl + (n-m)*wu*wu - n)/12 );

			std::vector<int> sizes;
			for (int i = 0; i < n; i++)
				sizes.push_back(i < m ? wl : wu);
			return sizes;
		}
		// standard gaussian


		// algorithm4
		void BoxBlurH_4(unsigned char* scl, unsigned char* tcl, int w, int h, int channel, int radius)
		{
			float iarr = 1.0f / (radius + radius + 1.0f);
			for (int i = 0; i < h; i++) {
				int ti = i * w * channel;// middle index
				int li = ti;// left index
				int ri = ti + radius * channel;// right index
				vec3 fv = vec3(scl[ti], scl[ti + 1], scl[ti + 2]);// first value
				vec3 lv = vec3(scl[ti + (w - 1) * channel], scl[ti + (w - 1) * channel + 1], scl[ti + (w - 1) * channel + 2]);// last value
				vec3 val = vec3(fv.r * (radius + 1), fv.g * (radius + 1), fv.b * (radius + 1));// (r+1)/(2r+1)
				for (int j = 0; j < radius * channel; j += channel)
				{
					val.r += scl[ti + j];
					val.g += scl[ti + j + 1];
					val.b += scl[ti + j + 2];
				}
				for (int j = 0; j <= radius * channel; j += channel)
				{
					val.r += scl[ri] - fv.r;
					val.g += scl[ri + 1] - fv.g;
					val.b += scl[ri + 2] - fv.b;

					tcl[ti] = val.r * iarr;
					tcl[ti + 1] = val.g * iarr;
					tcl[ti + 2] = val.b * iarr;

					ri += channel;
					ti += channel;
				}
				for (int j = (radius + 1) * channel; j < (w - radius) * channel; j += channel)
				{
					val.r += scl[ri] - scl[li];
					val.g += scl[ri + 1] - scl[li + 1];
					val.b += scl[ri + 2] - scl[li + 2];

					tcl[ti] = val.r * iarr;
					tcl[ti + 1] = val.g * iarr;
					tcl[ti + 2] = val.b * iarr;

					ri += channel;
					li += channel;
					ti += channel;
				}
				for (int j = (w - radius) * channel; j < w * channel; j += channel)
				{
					val.r += lv.r - scl[li];
					val.g += lv.g - scl[li + 1];
					val.b += lv.b - scl[li + 2];

					tcl[ti] = val.r * iarr;
					tcl[ti + 1] = val.g * iarr;
					tcl[ti + 2] = val.b * iarr;

					li += channel;
					ti += channel;
				}
			}
		}

		void BoxBlurT_4(unsigned char* scl, unsigned char* tcl, int w, int h, int ch, int r)
		{
			float iarr = 1.0f / (r + r + 1.0f);
			for (int i = 0; i < w * ch; i += ch) {
				int ti = i;
				int li = ti;
				int ri = ti + r * w * ch;
				vec3 fv = vec3(scl[ti], scl[ti + 1], scl[ti + 2]);
				vec3 lv = vec3(scl[ti + w * (h - 1) * ch], scl[ti + w * (h - 1) * ch + 1], scl[ti + w * (h - 1) * ch + 2]);
				vec3 val = vec3((r + 1) * fv.r, (r + 1) * fv.g, (r + 1) * fv.b);
				for (int j = 0; j < r; j++)
				{
					val.r += scl[ti + j * w * ch];
					val.g += scl[ti + j * w * ch + 1];
					val.b += scl[ti + j * w * ch + 2];
				}
				for (int j = 0; j <= r; j++)
				{
					val.r += scl[ri] - fv.r;
					val.g += scl[ri + 1] - fv.g;
					val.b += scl[ri + 2] - fv.b;

					tcl[ti] = val.r * iarr;
					tcl[ti + 1] = val.g * iarr;
					tcl[ti + 2] = val.b * iarr;

					ri += w * ch;
					ti += w * ch;
				}
				for (int j = r + 1; j < h - r; j++)
				{
					val.r += scl[ri] - scl[li];
					val.g += scl[ri + 1] - scl[li + 1];
					val.b += scl[ri + 2] - scl[li + 2];

					tcl[ti] = val.r * iarr;
					tcl[ti + 1] = val.g * iarr;
					tcl[ti + 2] = val.b * iarr;

					li += w * ch;
					ri += w * ch;
					ti += w * ch;
				}
				for (int j = h - r; j < h; j++)
				{
					val.r += lv.r - scl[li];
					val.g += lv.g - scl[li + 1];
					val.b += lv.b - scl[li + 2];

					tcl[ti] = val.r * iarr;
					tcl[ti + 1] = val.g * iarr;
					tcl[ti + 2] = val.b * iarr;

					li += w * ch;
					ti += w * ch;
				}
			}
		}

		void BoxBlur_4(unsigned char* scl, unsigned char* tcl, int w, int h, int ch, int r)
		{
			memcpy(tcl, scl, w * h * ch);
			BoxBlurH_4(tcl, scl, w, h, ch, r);
			BoxBlurT_4(scl, tcl, w, h, ch, r);
		}

		void GaussianBlur4(unsigned char* data, unsigned char* newData, int width, int height, int channelCount, int radius)
		{
			std::vector<int> bxs = BoxesForGauss(radius, 3);
			BoxBlur_4(data, newData, width, height, channelCount, (bxs[0] - 1) / 2);
			BoxBlur_4(newData, data, width, height, channelCount, (bxs[1] - 1) / 2);
			BoxBlur_4(data, newData, width, height, channelCount, (bxs[2] - 1) / 2);
		}
	}
}
