#ifndef PPM_IMAGE_H
#define PPM_IMAGE_H

class PPMImage {
public:
	int width;
	int height;
	std::vector<std::vector<std::vector<unsigned char>>> colorMat;

	PPMImage (int width = 0, int height = 0) {
		allocImage(width, height);
	}

	PPMImage (std::string name) {
		loadImage(name);
	}

	void loadImage (std::string name) {
		FILE *fin = fopen(name.c_str(), "rb");

		char chr;
		if (!fscanf(fin, "P6\n%d %d\n255%c", &height, &width, &chr)) {
			perror("Could not read image!");
		}

		allocImage(width, height);

		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				int a = fread(&colorMat[i][j][0], sizeof(char), sizeof(char) * 3, fin);
				(void)a;
			}
		}

		fclose(fin);
	}

	void saveImage (std::string name) {
		FILE *fout = fopen(name.c_str(), "wb");
		fprintf(fout, "P6\n%d %d\n255\n", height, width);

		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				fwrite(&colorMat[i][j][0], sizeof(char), sizeof(char) * 3, fout);
			}
		}

		fclose(fout);
	}

	void allocImage (int width, int height) {
		this->width = width;
		this->height = height;
		colorMat = std::vector<std::vector<std::vector<unsigned char>>>(width,
					std::vector<std::vector<unsigned char>>(height, 
					std::vector<unsigned char> (3)));
	}

	bool isInside (int x, int y) {
		return x >= 0 && y >= 0 && x < width && y < height;
	}
};

#endif