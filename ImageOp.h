#ifndef IMAGE_OP_H
#define IMAGE_OP_H

#define SIG (10.0f)
#define MIU (0.0f)
#define PI (3.0141592653589f)

namespace ImageOp
{
	void pixelate (PPMImage &image, int block_side) {
		for (int block_i = 0; block_i < image.width; block_i += block_side) {
			for (int block_j = 0; block_j < image.height; block_j += block_side) {
				int r = 0;
				int g = 0;
				int b = 0;
				for (int i = block_i; (i < block_i + block_side) && (i < image.width); i++) {
					for (int j = block_j; (j < block_j + block_side) && (j < image.height); j++) {
						r += image.colorMat[i][j][0];
						g += image.colorMat[i][j][1];
						b += image.colorMat[i][j][2];
					}
				}
				r /= block_side * block_side;
				g /= block_side * block_side;
				b /= block_side * block_side;
				for (int i = block_i; (i < block_i + block_side) && (i < image.width); i++) {
					for (int j = block_j; (j < block_j + block_side) && (j < image.height); j++) {
						image.colorMat[i][j][0] = (unsigned char)r;
						image.colorMat[i][j][2] = (unsigned char)b;
						image.colorMat[i][j][1] = (unsigned char)g;
					}
				}
			}
		}
	}

	void grayscale (PPMImage &image) {
		for (int i = 0; i < image.width; i++) {
			for (int j = 0; j < image.height; j++) {
				int avarage = (image.colorMat[i][j][0] + image.colorMat[i][j][1] + image.colorMat[i][j][2]) / 3;
				image.colorMat[i][j][0] = image.colorMat[i][j][1] = image.colorMat[i][j][2] = avarage;
			}
		}
	}

	void blur(PPMImage &image, int distance) {
		PPMImage result(image.width, image.height);

		float ampl = 1.0f / sqrt(2 * PI * SIG * SIG);
		float expAmpl =  1.0f / (2.0f * SIG * SIG);
		
		float weights[distance * 2 + 1][distance * 2 + 1];

		for (int i = 0; i < distance * 2 + 1; i++) {
			for (int j = 0; j < distance * 2 + 1; j++) {
				float rel_i = i - distance - 1;
				float rel_j = j - distance - 1;

				float dist = sqrt((rel_i) * (rel_i) + (rel_j) * (rel_j));
				weights[i][j] = ampl * exp(-(dist - MIU) * (dist - MIU) * expAmpl);
			}
		}

		for (int i = 0; i < image.width; i++) {
			for (int j = 0; j < image.height; j++) {
				int avarage_r = 0;
				int avarage_g = 0;
				int avarage_b = 0;
				float count = 0;
				for (int k = i - distance; k < i + distance; k++) {
					for (int l = j - distance; l < j + distance; l++) {
						if (image.isInside(k, l)) {
							float weight = weights[k - i + distance + 1][l - j + distance + 1];
							
							avarage_r += image.colorMat[k][l][0] * weight;
							avarage_g += image.colorMat[k][l][1] * weight;
							avarage_b += image.colorMat[k][l][2] * weight;
							
							count += weight;
						}
					}
				}
				result.colorMat[i][j][0] = avarage_r / count;
				result.colorMat[i][j][1] = avarage_g / count;
				result.colorMat[i][j][2] = avarage_b / count;
			}
		}
		image = result;
	}

	void brightness (PPMImage &image, float brightness) {
		float add = brightness * 512 / 100 - 256;

		for (int i = 0; i < image.width; i++) {
			for (int j = 0; j < image.height; j++) {
				image.colorMat[i][j][0] = std::max(0.0f, std::min(add + image.colorMat[i][j][0], 255.0f));
				image.colorMat[i][j][1] = std::max(0.0f, std::min(add + image.colorMat[i][j][1], 255.0f));
				image.colorMat[i][j][2] = std::max(0.0f, std::min(add + image.colorMat[i][j][2], 255.0f));
			}
		}
	}
}

#endif