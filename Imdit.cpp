#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#include <cmath>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stack>

#include "PPMImage.h"
#include "ImageOp.h"

int main(int argc, char **argv)
{
	int opt;
	enum Opts {PIXELATE_FLAG, GRAYSCALE_FLAG, BLUR_FLAG, BRIGHTNESS_FLAG};
	std::string outFile = "a.ppm";
	
	std::stack <Opts> opts;
	std::stack <std::string> optsInd;
	while ((opt = getopt(argc, argv, "ho:gp:b:r:")) != -1) {
		switch (opt) {
			case 'p': 
				opts.push(Opts::PIXELATE_FLAG);
				optsInd.push(std::string(optarg));
				break;
			case 'r': 
				opts.push(Opts::BRIGHTNESS_FLAG);
				optsInd.push(std::string(optarg));
				break;
			case 'b': 
				opts.push(Opts::BLUR_FLAG);
				optsInd.push(std::string(optarg));
				break;
			case 'g':
				opts.push(Opts::GRAYSCALE_FLAG);
				if (optarg)
					optsInd.push(std::string(optarg));
				else 
					optsInd.push("");
				break;
			case 'o': outFile = std::string(optarg);
				break;
			case 'h':
				std::cout << "Usage: make-image [options] input_file" << std::endl;
				std::cout << "-p [side lenght] pixelate" << std::endl;
				std::cout << "-b [distance] blur" << std::endl;
				std::cout << "-r [percentage] brightness (0..100)" << std::endl;
				std::cout << "-g grayscale" << std::endl;
				std::cout << "-o [file name] output file (defaults to a.ppm)" << std::endl;
				std::cout << "-h help" << std::endl;
				exit(EXIT_SUCCESS);
			default:
				std::cerr << "Usage: make-image [opts] [inputfile]" << std::endl;
				exit(EXIT_FAILURE);
		}
	}

	if (optind >= argc) {
		std::cerr << "Expected argument after option" << std::endl;
		exit(EXIT_FAILURE);
	}

	PPMImage image(argv[optind]);

	while (opts.size() != 0) {
		Opts opt = opts.top();
		std::string optVal = optsInd.top();
		opts.pop();
		optsInd.pop();

		switch (opt) {
			case Opts::PIXELATE_FLAG : 
				ImageOp::pixelate(image, atoi(optVal.c_str()));
				break;
			case Opts::BLUR_FLAG : 
				ImageOp::blur(image, atoi(optVal.c_str()));
				break;
			case Opts::BRIGHTNESS_FLAG : 
				ImageOp::brightness(image, atof(optVal.c_str()));
				break;
			case Opts::GRAYSCALE_FLAG : ImageOp::grayscale(image);
				break;
		}
	}
	
	image.saveImage(outFile);

	return 0;
}
