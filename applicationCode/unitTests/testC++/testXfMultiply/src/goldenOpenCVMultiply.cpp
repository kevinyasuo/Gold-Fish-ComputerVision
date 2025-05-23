/******************************************************************************
 *  Copyright (c) 2018, Xilinx, Inc.
 *  All rights reserved.
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions are met:
 *
 *  1.  Redistributions of source code must retain the above copyright notice, 
 *     this list of conditions and the following disclaimer.
 *
 *  2.  Redistributions in binary form must reproduce the above copyright 
 *      notice, this list of conditions and the following disclaimer in the 
 *      documentation and/or other materials provided with the distribution.
 *
 *  3.  Neither the name of the copyright holder nor the names of its 
 *      contributors may be used to endorse or promote products derived from 
 *      this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
 *  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
 *  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 *  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 *  OR BUSINESS INTERRUPTION). HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 *  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
 *  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 *  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/

/*****************************************************************************
*
#     Author: Kristof Denolf <kristof@xilinx.com>
#     Date:   2018/03/13
*
*****************************************************************************/

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/core/core.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <stdexcept>

#include "OpenCVUtils.h"
#include "HRTimer.h"

using namespace cv;

/** @function main */
int main ( int argc, char** argv )
{
  //command line parsing
  const String keys =
  "{help h usage ? |                 | print this message                   }"
  "{@image	       |                 | input image #1                       }"  
  "{@image	       |                 | input image #2                       }"
  "{display d      |                 | diplay result with imshow            }"
  "{interations n  | 1               | number of iterations to measure time }"
  ;

  CommandLineParser parser(argc, argv, keys);
  if (parser.has("help") || argc < 2)
  {
    parser.printMessage();
    std::cout << "\nhit enter to quit...";
    std::cin.get();
    return 0;
  }

  String fileName1 = parser.get<String>(0);   
  String fileName2 = parser.get<String>(1); 
  
  unsigned int numberOfIterations = 0;
  if (parser.has("interations"))
  numberOfIterations = parser.get<unsigned int>("interations");

  bool imShowOn = parser.has("display");

  /// Declare variables
  Mat src1, src2;
  Mat gray1, gray2;   
  Mat dstSW;
  double scale =1;
  // Initialize
  initializeSingleImageTest(fileName1, src1);
  initializeSingleImageTest(fileName2, src2);
  //convert to grayscale
  cvtColor(src1, gray1, COLOR_BGR2GRAY, 1);
  cvtColor(src2, gray2, COLOR_BGR2GRAY, 1);
  
  if (numberOfIterations > 0) {
    HRTimer timer;

	std::cout << "starting time measurement: " << numberOfIterations << std::endl;
    timer.StartTimer();
	
    for (unsigned int i = 0; i < numberOfIterations; i++) {
		multiply(gray1, gray2, dstSW, scale);
	}

    timer.StopTimer();

    std::cout << "Elapsed time over " << numberOfIterations << " calls: " << timer.GetElapsedUs() << " us or " << (float)timer.GetElapsedUs() / (float)numberOfIterations << "us per frame" << std::endl;
    std::cout << "Framerate: " << (float)numberOfIterations / ((float)timer.GetElapsedUs() / 1e6) << std::endl;
  }

  if (imShowOn) { 
	imshow("input src1",src1);	
	imshow("input src2",src2);
	imshow("Results",dstSW);
	waitKey(0);
  }

  return 0;
}
