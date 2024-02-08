#pragma once

#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>
#include "Settings.hpp"
using namespace std;

// Timing code adapted from: https://www.learncpp.com/cpp-tutorial/timing-your-code/

namespace potato {
    using Clock = std::chrono::steady_clock;
    using Second = std::chrono::duration<double, std::ratio<1> >;

    class Timekeeper {
        private:       
            double sumElapsed = 0;
            int sumCnt = 0;    
            chrono::time_point<Clock> startAve = Clock::now();   
            chrono::time_point<Clock> startTime = Clock::now();
            const double targetTime = 1.0/TARGET_FPS; 

        public:
            Timekeeper() = default;

            void resetStats() {
                sumElapsed = 0;
                sumCnt = 0;    
                startAve = Clock::now();  
            };

            void startFrame() {
                // Start time
                startTime = Clock::now();
            };

            double endFrame() {
                // Get elapsed time
                double elapsed = chrono::duration_cast<Second>(Clock::now() - startTime).count();

                // Calculate how much time to wait
                double waitTime = targetTime - elapsed;

                // Overtime?
                if(waitTime < 0) {            
                    waitTime = 0;
                }

                // Add to average stats        
                sumElapsed += elapsed;
                sumCnt++;
        
                // How much time has elapsed for our average calculations?
                double elapsedAve = chrono::duration_cast<Second>(Clock::now() - startAve).count();        
                if(PRINT_AVE_STATS && elapsedAve >= AVE_STATS_TIME_WINDOW) {           
                    // Calculate average across time window            
                    double aveElapsed = sumElapsed / sumCnt;            
                    int avePossibleFPS = (int)(round(1.0/aveElapsed));

                    // Print time elapsed
                    cout << "(AVE WINDOW: " << elapsedAve << "): POSSIBLE FPS: " << avePossibleFPS << "; TIME (SECONDS): " << aveElapsed << endl;

                    // Reset
                    resetStats();    
                }

                return waitTime;
            };
    };
};