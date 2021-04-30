# pragma once
# include <iostream>
# include <vector>
# include <fftw3.h>
# include <cmath>
# include "wav.h"


const std::string note_letters[12]{ "C ", "C#", "D ", "D#", "E ", "F ", "F#", "G ", "G#", "A ", "A#", "B ", };
const double pi = 3.14159265359;

class SpecEng
{
public:
	fftw_complex* timeseries{ NULL }, * spectrum{ NULL };
	fftw_plan clip_plan{};
	double* freq_range{ NULL };				  // from f_clip to f_sample in N steps
	
	const int nrange{ 57 };					  // Number of notes to analyze, starting from C0
	double df{ 1 };							  // Frequency-step
	int ambient_lvl{ 0 };
	int st_pop{ 44100 };					  //number of samples in the transform window

	std::vector <int> noise_profile{};	  // DFT of noise
	std::vector <double> fnote{};			  // The frequencies of the notes desired
	std::vector <int> fnote_index{};		  // The indices at which fnotes appear in freq space
	std::vector <int> rls{};				  // Holds the relative power spectrum (relative to noise)
	std::vector <int> note_lvl{};			  // Holds the note power levels

public:
	/*
	SpecEng(short int* adata) {
		set_timeseries(adata);
		plan();
	}
	SpecEng() {};*/

	bool set_noise_profile();
	bool set_note_freqs(double a4 = 440);
	bool plan();

	
	void set_timeseries(short int* adata, bool print = false);

	void get_spectrum() {
		fftw_execute(clip_plan);
	}
	void relative_spectrum();

	void resize_dft_vecs();

	/*
	int* note_id() {
		//std::cout << "\n----- try to identify the note! -------\n";
		std::vector <int> level (st_pop);
		int max_level{ 0 };
		int max_level_index{ 0 };
		int avg_level{ 0 };
		for (int i{ 0 }; i < st_pop - 1; i++) {
			level[i] = 0.00 * (rls[fnote_index [i] - 2]) + 0.0 * *(rls + *(fnote_index + i) - 1) + 1.00 * *(rls + *(fnote_index + i)) +
				0.0 * *(rls + *(fnote_index + i) + 1) + 0.00 * *(rls + *(fnote_index + i) + 2);
			avg_level += level[i];
			if (max_level < level[i]) { max_level = level[i]; max_level_index = i; }

			if (i > 21) { std::cout << " DFT Index " << *(fnote_index + i) << " level " << *(fnote + i) << " : " << level[i]; }
			if (level[i] > *(note_lvl + i) * 2 && level[i] > 40) { std::cout << "*** up from " << *(note_lvl + i); }
			if (i > 21) {
				std::cout << "\n";
			}
			*(note_lvl + i) = level[i];
		}
		avg_level = avg_level / nrange; std::cout << " \n avg_level*3 : " << avg_level * 3 << "\n";
		if (max_level > avg_level * 3) {
			std::cout << "\n most powerful note is " << note_letters[max_level_index % 12] << static_cast<int>(floor(max_level_index / 12) + 1)
				<< " with index " << max_level_index << " at " << *(fnote + max_level_index) << " Hz with a level of " << max_level << "\n";
		}
		else { std::cout << "No prominent note \n"; }
		return NULL;
	}*/
};
