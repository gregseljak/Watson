#include "wav.h"
# include <iostream>
# include <vector>
# include <string>
# include <fstream>
# include <cmath>



char* wav::wav_import() {
    // Largely taken from cppreference.com
    FILE* pFile;
    long lSize{};
    char* buffer{ NULL };
    size_t result;

    pFile = fopen(wavfilename.c_str(), "rb");
    if (pFile == NULL) { fputs("File error", stderr); exit(1); }

    // obtain file size:
    fseek(pFile, 0, SEEK_END);
    lSize = ftell(pFile);
    rewind(pFile);
    std::cout << lSize << std::endl;

    // allocate memory to contain the whole file:
    buffer = new char[(sizeof(char) +1) * lSize];
    if (buffer == NULL) { fputs("Memory error", stderr); exit(2); }

    // copy the file into the buffer:
    std::cout << sizeof(char) * lSize;
    result = fread(buffer, 1, lSize, pFile);
    if (result != lSize) { fputs("Reading error", stderr); std::cout << result << " " << lSize; exit(3); }

    fclose(pFile);
    wavfile = buffer;
    return buffer;
}


bool wav::setup_wav(std::string filename) {
    wavfilename = filename;

    // Directly import the whole file and read the header for info
    wav_import();
    ch_pop = sle_intcast(wavfile + 22);
    samplefreq = ule_intcast(wavfile + 24);
    byterate = sle_intcast(wavfile + 28);
    samplesize_bits = sle_intcast(wavfile + 34);
    datachunksize = ule_intcast(wavfile + 40);
    channel_len = datachunksize / ch_pop / samplesize_bits * 8;
    //channel_len = 10000; // For testing purposes

    // Allocate memory for the channels
    wav_l = new short int[sizeof(short int) * channel_len];
    std::cout << "wav_l: " << wav_l <<std::endl;
    if (ch_pop > 1) {
        wav_r = new short int[sizeof(short int) * channel_len];
        //std::cout << " -------- generated L.C. and R.C arrays ------------- " << std::endl;
    };


    // This is a dumb way that only actually supports two channels. But it's working for now.
    int prtct{ 0 };
    prtct = 80;
    for (unsigned int i{ 0 }; i < channel_len; i++) {
        *(wav_l + i) = sle_intcast(wavfile + (44 + 4 * i));
        *(wav_r + i) = sle_intcast(wavfile + (46 + 4 * i));
        if (prtct < 1000) {
            std::cout << "index " << i << " || wav l : " << *(wav_l + i) << " || wav r : " << *(wav_r + i) << std::endl;
            std::cout << "wav l written to : " << &(*(wav_l + i)) << std::endl;
            prtct++;
        }

    }

    // Done with the raw import
    delete wavfile;
    wavfile = NULL;
    std::cout << "ch_pop = " << ch_pop << " samplefreq = " << samplefreq << " byterate = " << byterate << std::endl;
    std::cout << "channel len = " << channel_len << "samples" << std::endl;
    return 0;
}

void wav::print_header(int len) {
    int chunk{ 0 };
    std::cout << "First four positions: RIFF" << std::endl;

    for (int i{ 0 }; i < len; i++) {
        if ((i % 4 == 0 || i == 22 || i == 34) && (i < 44)) {
            std::cout << "+------- lilchunk #" << chunk << " -----------+" << std::endl;;
            chunk++;
            if (i == 20 || i == 22 || i == 32 || i == 34) {
                std::cout << "short int [" << i << " : " << i + 1 << "] :" << ule_intcast(wavfile + i, 2) << std::endl;
            }
            else if (i == 0 || i == 8 || i == 12 || i == 36) {
                std::cout << "Message block: " << std::endl;
            }
            else {
                std::cout << "long int [" << i << " : " << i + 3 << "] : " << ule_intcast(wavfile + i) << std::endl;
            }
        }


        if (chunk - 1 == 0 || chunk - 1 == 2 || chunk - 1 == 3 || chunk - 1 == 11) {
            std::cout << "char " << i << " : " << *(wavfile + i) << std::endl;
        }
        else {
            std::cout << "int " << i << " : " << static_cast<short int>(*(wavfile + i)) << std::endl;
        }
    }

}

void wav::print_data(int len) {
    for (int i{ 44 }; i < (len + 44); i++) {
        if (i % 2 == 0) {
            std::cout << "sample at index " << i << " :" << sle_intcast(wavfile + i) << std::endl;

        }
        std::cout << "Byte " << i << " : " << static_cast<short int>(*(wavfile + i)) << std::endl;
    }
}

// ule_intcast interprets 4-byte little endian data (wav file metadata)
long unsigned int ule_intcast(char* index, size_t len) { // Come back to redefine for cases above/below 4 bytes if necessary
    unsigned long summand{ 0 };
    if (len == 4) {
        summand = (static_cast<unsigned char>(*(index + 3)) << 24) | (static_cast<unsigned char>(*(index + 2)) << 16) |
            (static_cast<unsigned char>(*(index + 1)) << 8) | (static_cast<unsigned char>(*(index)));
        return summand;
    }
    else if (len == 2) {
        summand = (static_cast<unsigned char>(*(index + 1)) << 8) | (static_cast<unsigned char>(*(index)));
    }
    return summand;
}

// sle_intcast interprets 2-byte little endian data (wav sample amplitudes)
short int sle_intcast(char* index) {
    short int summand{ 0 };
    summand = (static_cast<unsigned char>(*(index + 1)) << 8) | (static_cast<unsigned char>(*(index)));
    return summand;
}
