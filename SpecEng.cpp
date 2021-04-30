# include <vector>
# include <fftw3.h>
# include "SpecEng.h"
# include "numpy.h"


bool SpecEng::set_note_freqs(double a4) {
	fnote.clear();
	fnote_index.clear();
	int octave{ 0 };
	double twon{ 1.05946309436 }; // = 2^(1/12)

	fnote.push_back(a4 / pow(twon, nrange));

	for (int i{ 0 }; i < nrange; i++) {
		fnote.push_back((fnote[i]) * twon);
		fnote_index.push_back(round(fnote[i] / df));
		//std::cout << " " << note_letters[i % 12] << octave << " " << (fnote[i]) << " at index " << (fnote_index[i]) << "\n"; ;
		if (i % 12 == 11) { octave++; };
	}
	return true;
}


bool SpecEng::plan() {
	resize_dft_vecs();
	if (timeseries == NULL) {
		timeseries = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * st_pop);
		spectrum = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * st_pop);
	};
	df = (44100 - st_pop / 44100) / st_pop;
	clip_plan = fftw_plan_dft_1d(st_pop, timeseries, spectrum, FFTW_FORWARD, FFTW_ESTIMATE);
	//freq_range = linspace((4 / 44100), 44100 / 4, st_pop);
	set_noise_profile();
	set_note_freqs();

	return true;
};



bool SpecEng::set_noise_profile() {
	wav noise_wav("dataset/mic noise/ambient noise.wav");
	std::cout << "Importing ambient noise \n";


	set_timeseries(noise_wav.wav_l);
	get_spectrum();
	

	int summand{ 0 };
	int enroute{ 0 };
	std::cout << "\n ----- extracting noise spectrum ----- \n" << std::endl;

	for (int i{ 0 }; i < st_pop; i++) {
		noise_profile[i] = ((spectrum[i][0] * spectrum[i][0] + spectrum[i][1] * spectrum[i][1]));
		summand += (noise_profile[i]);
		if (i % 2 == 0 && i < -400) {
			std::cout << " Noise profile modulus " << i << " : " << noise_profile[i] << "\n";
			std::cout << " Summand: " << summand << "\n";

		}
		if (i == 0) {
			enroute = noise_profile[0];
		}
		else if (i == 1) {
			enroute = 0.5 * (noise_profile[0] + noise_profile[1]);
		}
		else {
			enroute = (enroute * (i - 1) + noise_profile[i]) / i;
		}
	}
	
	ambient_lvl = floor(summand / st_pop);
	std::cout << "enroute = " << enroute << " Ambient level = " << ambient_lvl << "\n ------------------------------ \n";
	return true;
};

void SpecEng::set_timeseries(short int* adata, bool print) {
	std::cout << "\n --- setting timeseries ---- \n";

	int prnct{ 0 };
	prnct = 95;
	for (int i{ 0 }; i < st_pop; i++) {

		timeseries[i][0] = (*(adata + i));

		if (prnct < 100 && print) {
			std::cout << "pulling from " << (adata + i) << std::endl;
			std::cout << "adata index " << i << " : " << *(adata + i) << std::endl;
			prnct++;
		}

	}
	if (print) { std::cout << "\n --------------------------- \n"; }
};


void SpecEng::relative_spectrum() {
	
	int prtct{ 0 };
	std::cout << "\n ----- relative spectrum ----\n";
	for (int i{ 0 }; i < st_pop / 2; i++) {
		rls[i] = ((spectrum[i][0] * spectrum[i][0] + spectrum[i][1] * spectrum[i][1])) / (noise_profile[i]);
		if (i % 2 == 0 && i < 320) {
			//std::cout << " Relative spectrum strength at index " << i << " : " << *(rls + i) << "\n";
			prtct++;
		}
	}
}

void SpecEng::resize_dft_vecs() {
	noise_profile.resize(st_pop);	  
	fnote.resize(st_pop);			  
	fnote_index.resize(st_pop);		  
	rls.resize(st_pop);				  
	note_lvl.resize(st_pop);
}