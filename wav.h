#pragma once
# include <iostream>
# include <vector>
# include <string>
# include <fstream>
# include <cmath>
# pragma warning(disable:4996) // Allows use of fopen instead of fopen_s, which I didn't understand

// ule_intcast interprets 4-byte little endian data (wav file metadata)
long unsigned int ule_intcast(char* index, size_t len = 4);
// sle_intcast interprets 2-byte little endian data (wav sample amplitudes)
short int sle_intcast(char* index);

class wav
{
private:
    // Import location declaration
    char* wavfile{ NULL };


    // File metadata, with default values
    std::string wavfilename{ "middle c l only.wav" };
    short int ch_pop{ 0 };           // Number of channels interleaved in data
    unsigned int byterate{ 0 };      // Stream bytes/s
    unsigned int samplefreq{ 0 };    // For 44100 Hz, T = 0.00002267573 s
    unsigned short int samplesize_bits{ 0 };
    unsigned long datachunksize{ 0 }; // number of bytes in raw data subchunk
    short int sample_val(int index, char side = *"L") {
        if (side == *"L" || side == *"l") {
            return sle_intcast(wavfile + (4 * index + 44));
        }
        else if (side == *"R" || side == *"r") {
            return sle_intcast(wavfile + (4 * index + 46));
        }
        else {
            std::cout << "invalid call for .sample_val()";
            return 1;
        }
    } // obsolete, but kept for future testing purposes 
    char* wav_import();

public:
    // Constructors
    wav() {};
    wav(std::string fname){
        setup_wav(fname);
    };


    unsigned channel_len{ 0 };
    short int* wav_l{ NULL }; // L.S. Channel
    short int* wav_r{ NULL }; // R.S. Channel
    std::string get_filename() {
        return wavfilename;
    }
    bool setup_wav(std::string filename);

    void print_header(int len = 43);

    void print_data(int len = 50);

};

