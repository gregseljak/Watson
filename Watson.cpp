# include <vector>
# include <cctype>
# include <string>
# include <cmath>
# include <fftw3.h>
# include <random>
# include <chrono>
# include <fstream>
# include <iostream>
# include "csv_manip.h"
# include "SpecEng.h"
# include "wav.h"


int main()
{
    wav reboot("middle c.wav");
    std::cout << "\n reboot.wav_l location :" << reboot.wav_l <<std::endl;

    SpecEng again;
   
    again.plan();
    
    again.set_timeseries(reboot.wav_l, true);

}