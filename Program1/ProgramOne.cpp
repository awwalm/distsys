//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/**
* @author       : Awwal Mohammed
* @subject      : BCE617 Distributed Systems
* @organization : International University of Malaya-Wales (IUMW)
* @title        : Program One - One-Way Data Transfer
* @date         : 27-11-19
*/

/*! \brief Program 1 (ProgramOne.cpp)
 *         Copies bytes of data from one buffer to another, all within the same shared memory space.
 *
 * This one-way buffer to buffer data transfer program performs a benchmarking test.
 * The results are intitially displayed in the console application or command line.
 * 
 */


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#include <windows.h>    /**< Windows preprocessor needed for CMD arguments and console/Shell/BASH management. */
#include <iostream>     /**< Standard input-output stream to expose the std namespace. */
#include <fstream>
#include <sys/stat.h>   /**< System header necessary for performing the file check. */
#include <unistd.h>     /**< System preprocessor directory necessary for initializing unsinged integers. */
#include <string.h>
#include <sys/time.h>   /**< System header for time. */
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>      /**< Specifying this to adjust with any invocation of legacy C code, seeing as this is C++ source. */
#include <math.h>       /**< To meet up with any calculations involved with time. */

using namespace std;    /**< Optional at this stage, it connects to the iostream preprocessor. */



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/*! \def BMAX 128*1024*1024
 *   \brief Specifying default Buffer allocation preset of (approximately) 134 MB. 
 *   
 *  Note that 1 Byte = 8 bits.
 *  128 * 1024 * 1024 Bits = 134,217,728 Bytes ≈ 134.2 Megabytes.
 *  As soon as the function \fn malloc(bmax)
 *  runs and does its work, memcpy copies bytes of data from the input buffer to the output buffer
 *  a number times corresponding to iamx. (NOTE: imax is essentially the same as IMAX.)
 *  The time interval is then subsequently associated with every single file successfully copied. 
 */

/*! \def IMAX 1000
 *  \brief Specifying that no more than 1000 iterations.
 *
 *    Maximum number of iteration the loop cycles throguh upon completing the benchmark test. 
 *    Subsequently, the 1000 acts as a constant universal scale for the time and latency.
 */


#define IMAX    1000 /**< Maximum number of iterations. */

// #define IMAX  10000
// #define BMAX  32*1024*1024

// #define BMAX    256*1024*1024 /**< Approximately 268 Megabytes of buffer allocation preset. */
// #define BMAX    512*1024*1024 /**< Approximately 537 Megabytes of buffer allocation preset. ENSURE YOU HAVE THE HARDWARE CAPACITY TO RUN THIS PROGRAM. */
#define BMAX    128*1024*1024 /**< Approximately 134 Megabytes of buffer allocation preset. ENSURE YOU HAVE THE HARDWARE CAPACITY TO RUN THIS PROGRAM. */


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*! \brief Global variables being used to declare Python source names.
 */

const char *bytesGraph = "BytesGraph.py";             /**< Pointer character constant for naming the Bytes Python source. */
const char *iterationsGraph = "IterationsGraph.py";   /**< Pointer character constant for naming the Iterations Python source. */
const char *timeGraph = "TimeGraph.py";               /**< Pointer character constant for naming the Time Python source. */
const char *latencyGraph = "LatencyGraph.py";         /**< Pointer character constant for naming the Latency Python source. */
const char *bandwidthGraph = "BandwidthGraph.py";     /**< Pointer character constant for naming the Bandwidth Python source. */
const char *combinedGraph = "CombinedGraph.py";       /**< Pointer character constant for naming the Combined Chart Python source. */

const char *bytesVsBandwidth = "BytesVsBandwidth.py"; /**< Pointer character constant for naming the Bytes plotted against Bandwidth Python source. */
const char *bytesVsLatency = "BytesVsLatency.py";     /**< Pointer character constant for naming the Bytes plotted against Latency Python source. */



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*! \brief Preliminaries for parsing Bytes Python code.
 *
 */
string bytesYAxis = "bytesYAxis = [";   /**< The part of the source that creates parses the first character of the bytes on the Y-axis. */

//----------------------------------------------------

/*! \brief Preliminaries for parsing Iterations Python code.
 *
 */
string iterationsYAxis = "iterationsYAxis = [";
//string iterationsYAxis; 

//----------------------------------------------------

/*! \brief Preliminaries for parsing Time Python code.
 *
 */
string timeYAxis = "timeYAxis = [";
//string timeYAxis; 

//----------------------------------------------------

/*! \brief Preliminaries for parsing Latency Python code.
 *
 */
string latencyYAxis = "latencyYAxis = [";
//string latencyYAxis; 

//----------------------------------------------------

/*! \brief Preliminaries for parsing Bandwidth Python code.
 *
 */
string bandwidthYAxis = "bandwidthYAxis = [";
//string bandwidthYAxis; 

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/*! \brief \fn bool exists_test1 (const std::string& name)
*          Checks whether a file exists or not.
* In our case, we need to be sure a certain Python file being parsed into
* exists prior to being modified.
* @param name  represents string memory addres variable that the function uses to evaluate the string.
* The string is then used to detect actual file existence.
*/
inline bool exists_test1 (const std::string& name) 
{
    if (FILE *file = fopen(name.c_str(), "r")) 
    {
        fclose(file);
        return true;
    } 
    else 
    {
        return false;
    }   
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/** 
* \fn static double log_ClockTime(void)
* \brief Function that retrieves system time in milliseconds.
*
* @param &tv is a dereferenced value to an invisible reference pointer that represents the current time value.
* By initiating it with a struct, the compiler instantly assigns a number to it dynamically.
*/
static double log_ClockTime(void)
{
    struct timeval tv;

    gettimeofday(&tv,NULL);

    return tv.tv_sec + 0.000001*tv.tv_usec;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/** 
* \fn int main(int argc,char **argv)
* \brief Main function that performs the desired buffer to buffer functionality.
*
* @param argc represents the argument count (to be assessed in the command line).
* @param **argv represents a dynamically generated pointer that points to yet another pointer.
* of variables organizes the input data variable to be worked on in a dynamically generated temporary array.
*/
int main(int argc,char **argv) /**< Begin Main Function. */
{
    
  //--------------------------------------------------------

  /*! \brief This if-else block ensures the necessary files exists otherwise, it displays some important warnings.
  */
  if ( (exists_test1(bytesGraph) == true) && (exists_test1(iterationsGraph) == true) && (exists_test1(timeGraph) == true) && (exists_test1(latencyGraph) == true) && (exists_test1(bandwidthGraph) == true) && (exists_test1(combinedGraph) == true) && (exists_test1(bytesVsBandwidth) == true) && (exists_test1(bytesVsLatency) == true) ) 
  {
        std::cout << "ALL GRAPHING FILES (BytesGraph.py, IterationsGraph.py, TimeGraph.py, LatencyGraph.py, BandwidthGraph.py, CombinedGraph.py, BytesVsBandwidth.py, BytesVsLatency.py) DISCOVERED." << endl;
        std::cout << "MODIFYING EXISITING FILES..." << endl;
  }
  else
  {
        if ( (remove( bytesGraph ) != 0) || (remove( iterationsGraph ) != 0) || (remove( timeGraph ) != 0) || (remove( latencyGraph ) != 0) || (remove( bandwidthGraph ) != 0) || (remove( combinedGraph ) != 0) || (remove( bytesVsBandwidth ) != 0) || (remove( bytesVsLatency ) != 0) ) 
        {
          perror( "WARNING: Some Python files may be missing (Possible first time execution.)\n" );
          std::cout << "File deletion skipped.\n";
        }
        else {
          puts( "Existing files found...\n" );
          std::cout << "Existing files deleted.\n";
        }

  }


  //-------------------------------------------------------



  std::cout << "GENERATING PYTHON FILE NAMES (BytesGraph.py, IterationsGraph.py, TimeGraph.py, LatencyGraph.py, BandwidthGraph.py, CombinedGraph.py, BytesVsBandwidth.py, BytesVsLatency.py)... \n";
  std::cout << "PLEASE ENSURE YOU'VE MET THE FOLLOWING REQUIREMENTS:\n";
  Sleep(750);
  std::cout << "\t 1. Python 3.6.x and above is installed.\n";
  std::cout << "\t 2. The matplotlib module has been properly configured.\n";
  std::cout << "FILE GENERATION DONE.\nWAIT FOR BENCHMARK RESULTS AND GRAPHS.\n\n\n";
  Sleep(750);
  //std::getchar();
  //cin >> fileName;


  //---------------------------------------------------------


    int iterations,imax = IMAX,	bytes,bmax = BMAX;
    double time;
    int *inputBuffer,*outputBuffer;


    inputBuffer = (int *)malloc(bmax);
    outputBuffer = (int *)malloc(bmax);

    /*! \brief This For-loop uses Bitwise Left-shift to adjust the values of the bytes contgously.
     */
    for (bytes = 0; bytes <= bmax; bytes = (bytes) ? bytes << 1 : 1) /**< Start critical For-loop. */
     {
        time = log_ClockTime(); /**< Get the current time value. */

        /*! \brief This For-loop will attempt to copy the contents of the input buffer to the output buffer.
         *         This operation is done almost  1000 times, but there's a catch.
         *         This is an internal For-loop therefore, the loop terminates prematurely due to the condition...
         *         of the superior For-loop.
         */
        for (iterations = 0; iterations < imax; iterations++) /**< Start secondary For-loop */
        {
            /**
            * \fn memcpy(void* outputBuffer, const void* inputBuffer, std::size_t count)
            * \brief Copies contents in bytes from the source @param inputBuffer to the destination @param outputBuffer.
            *
            * This function returns an array, in this case, explcitly casted as int* rather than char*, of characters.
            */
            memcpy(outputBuffer,inputBuffer,bytes);
        }

        time = log_ClockTime() - time; /**< Subtract the time value from its previous definition to get duration of memcpy invocation. */



        //-------------------------------------------------------------------------------------------------------
        /** Bureaucratic strings for parsing in Array values for the Bytes.
         *
         *  Concatenate the previous string onto itself and parse in a comma between the values.
         *  This generates the array for the X-Axes for Bytes, Iteration, Latency, and Bandwidth data.
         */
        //-----------------------------------------------------------
        std::string convertBytes = std::to_string(bytes); /**< Bytes is in integer data type hence, is converted to strings. */
        bytesYAxis += convertBytes + ","; /**< Concatenate previous strings, add a comma as array separator, and update the global definition of the string. */
        //-----------------------------------------------------------
        std::string convertIterations = std::to_string(iterations); /**< Iterations is in integer data type hence, is converted to strings. */
        iterationsYAxis += convertIterations + ","; /**< Concatenate previous strings, add a comma as array separator, and update the global definition of the string. */
        //-----------------------------------------------------------
        std::string convertTime = std::to_string(time); /**< Time is in Double Precision data type hence, is converted to strings. */
        timeYAxis += convertTime + ","; /**< Concatenate previous strings, add a comma as array separator, and update the global definition of the string. */
        //-----------------------------------------------------------
        double parsedLatency = (time/iterations)*10000000000; /**< Upscaling by 10 billion to avoid loss of precision. */
        std::string convertLatency = std::to_string(parsedLatency); /**< Latency is in Double Precision data type hence, is converted to strings. */
        latencyYAxis += convertLatency + ","; /**< Concatenate previous strings, add a comma as array separator, and update the global definition of the string. */
        //-----------------------------------------------------------
        double parsedBandwidth = (bytes/time*iterations);
        std::string convertBandwidth = std::to_string(parsedBandwidth); /**< Bandwidth is in Double Precision data type hence, is converted to strings. */
        bandwidthYAxis += convertBandwidth + ","; /**< Concatenate previous strings, add a comma as array separator, and update the global definition of the string. */
        //-------------------------------------------------------------

        printf("bytes = %-8d\titers = %-8d\ttime = %-12.6g\t""lat = %-12.6g\tbw = %-12.6g\n",
                bytes,iterations,time,time/iterations,bytes/time*iterations); 

    } /**< End Critical For-loop. */



    //-------------------------------------------------------------------------------------------------------
    


    /** Bureaucratic strings for enclosing the Y-Axes arrays of our Python graphs.
     *
     *  Concatenate the previous string onto itself and parse in an enclosing squared bracket.
     *  This encloses the array for the Y-Axis for Bytes, Iteration, Latency, and Bandwidth data.
     */
    bytesYAxis      += "]\n";
    iterationsYAxis += "]\n";
    timeYAxis       += "]\n";
    latencyYAxis    += "]\n";
    bandwidthYAxis  += "]\n";



    //-------------------------------------------------------------

    
    /** Final parsing and generation of BytesGraph.py.
     *
     *  This opens the file and modifies the content.
     *  Explicitly declared strings for plotting the logarithmic graphs are specified herwith.
     */
    
    ofstream txtOut;

    txtOut.open (bytesGraph);
    txtOut << "# Title: BytesGraph.py - Generates the logarithmic graph based on the C++ benchmark code.\n\n";
    txtOut << "# Author: Awwal Mohammed\n\n";
    txtOut << "# PLEASE RUN THIS COMMAND TO INSTALL THE REQUIRED MODULES: \n";
    txtOut << "# \t pip install matplotlib \n\n";
    txtOut << "# Parsing in Logarithmic Graph for Bytes...\n\n\n";
    txtOut << "import math\n";
    txtOut << "import matplotlib.pyplot as plt\n\n\n";
    txtOut << bytesYAxis; /**< Actually the Y-Axis Now. */
    txtOut << "del bytesYAxis[-1]\n"; /**< Delete the outlying comma. */
    txtOut << "del bytesYAxis[0]\n"; /**< Delete the zero since Log(0) is invalid. */
    txtOut << "print ('Pure Byte Values:')\n";
    txtOut << "bytesXAxisLog = []\n"; /**< Logarithmic X-Axis. */
    txtOut << "for i in bytesYAxis:\n";
    txtOut << "\ti = math.log10(i)\n";
    txtOut << "\tbytesXAxisLog.append(i)\n";
    txtOut << "print (bytesYAxis)\n\n";
    txtOut << "print ('Logarithmic Byte Values:')\n";
    txtOut << "print (bytesXAxisLog)\n\n";
    txtOut << "#plotting the Logarithmic Graph\n";
    txtOut << "plt.plot(bytesXAxisLog, bytesYAxis, label = 'Logarithmic Values of Bytes')\n";
    txtOut << "plt.ylabel('Bytes')\n";
    txtOut << "plt.xlabel('Logarithmic Values')\n";
    txtOut << "plt.title('Byte Readings for Program 1')\n";
    txtOut << "plt.legend()\n";
    txtOut << "plt.show()\n";

    txtOut.close(); /**< Exit editing BytesGraph.py. */

    ShellExecute(NULL, "open", "BytesGraph.py", NULL, NULL, SW_SHOWDEFAULT); /**< Immediately execute the Python file right away. */

    /** DONE GENERATING BYTE PYTHON GRAPH SOURCE */


    //-------------------------------------------------------------------------------------------------------



    //-------------------------------------------------------------------------------------------------------

    
    /** Final parsing and generation of IterationsGraph.py.
     *
     *  This opens the file and modifies the content.
     *  Explicitly declared strings for plotting the logarithmic graphs are specified herwith.
     */
    
    ofstream txtOut2;

    txtOut2.open (iterationsGraph);
    txtOut2 << "# Title: IterationsGraph.py - Generates the logarithmic graph based on the C++ benchmark code.\n\n";
    txtOut2 << "# Technically, the graph displays blank because the iteration stays constant at 1000.\n\n";
    txtOut2 << "# Author: Awwal Mohammed\n\n";
    txtOut2 << "# PLEASE RUN THIS COMMAND TO INSTALL THE REQUIRED MODULES: \n";
    txtOut2 << "# \t pip install matplotlib \n\n";
    txtOut2 << "# Parsing in Logarithmic Graph for Iterations...\n\n\n";
    txtOut2 << "import math\n";
    txtOut2 << "import matplotlib.pyplot as plt\n\n\n";
    //txtOut << bytesSource1;
    txtOut2 << iterationsYAxis; /**< Actually the Y-Axis. */
    txtOut2 << "del iterationsYAxis[-1]\n"; /**< Delete the outlying comma. */
    txtOut2 << "print ('Pure Iteration Values:')\n";
    txtOut2 << "iterationsXAxisLog = []\n"; /**< Logarithmic X_Axis. */
    txtOut2 << "for i in iterationsYAxis:\n";
    txtOut2 << "\ti = math.log10(i)\n";
    txtOut2 << "\titerationsXAxisLog.append(i)\n";
    txtOut2 << "print (iterationsYAxis)\n";
    txtOut2 << "print ('Logarithmic Iterations Values:')\n";
    txtOut2 << "print (iterationsXAxisLog)\n";
    txtOut2 << "#plotting the Logarithmic Graph\n";
    txtOut2 << "plt.plot(iterationsXAxisLog, iterationsYAxis, label = 'Logarithmic Values of Iterations')\n";
    txtOut2 << "plt.ylabel('Iterations')\n";
    txtOut2 << "plt.xlabel('Logarithmic Values')\n";
    txtOut2 << "plt.title('Iterations Readings for Program 1')\n";
    txtOut2 << "plt.legend()\n";
    txtOut2 << "plt.show()\n";

    txtOut2.close(); /**< Exit editing IterationsGraph.py. */

   // ShellExecute(NULL, "open", "IterationsGraph.py", NULL, NULL, SW_SHOWDEFAULT); /**< No need to immediately execute the Python file right away. */

    /** DONE GENERATING ITERATION PYTHON GRAPH SOURCE */


    //-------------------------------------------------------------------------------------------------------


    /** Final parsing and generation of TimeGraph.py.
     *
     *  This opens the file and modifies the content.
     *  Explicitly declared strings for plotting the logarithmic graphs are specified herwith.
     */
    
    ofstream txtOut3;

    txtOut3.open (timeGraph);
    txtOut3 << "# Title: TimeGraph.py - Generates the logarithmic graph based on the C++ benchmark code.\n\n";
    txtOut3 << "# Author: Awwal Mohammed\n\n";
    txtOut3 << "# PLEASE RUN THIS COMMAND TO INSTALL THE REQUIRED MODULES: \n";
    txtOut3 << "# \t pip install matplotlib \n\n";
    txtOut3 << "# Parsing in Logarithmic Graph for Iterations...\n\n\n";
    txtOut3 << "import math\n";
    txtOut3 << "import matplotlib.pyplot as plt\n\n\n";
    txtOut3 << timeYAxis; /**< Actually the Y-Axis. */
    txtOut3 << "del timeYAxis[-1]\n"; /**< Delete the outlying comma. */
    txtOut3 << "print ('Pure Time Values:')\n";
    txtOut3 << "timeXAxisLog = []\n";
    txtOut3 << "for i in timeYAxis:\n";
    txtOut3 << "\ti = math.log10(i)\n";
    txtOut3 << "\ttimeXAxisLog.append(i)\n";
    txtOut3 << "print (timeYAxis)\n";
    txtOut3 << "print ('Logarithmic Time Values:')\n";
    txtOut3 << "print (timeXAxisLog)\n";
    txtOut3 << "#plotting the Logarithmic Graph\n";
    txtOut3 << "plt.plot(timeXAxisLog, timeYAxis, label = 'Logarithmic Values of Time')\n";
    txtOut3 << "plt.ylabel('Time')\n";
    txtOut3 << "plt.xlabel('Logarithmic Values')\n";
    txtOut3 << "plt.title('Time Readings for Program 1')\n";
    txtOut3 << "plt.legend()\n";
    txtOut3 << "plt.show()\n";

    txtOut3.close(); /**< Exit editing TimeGraph.py. */

    ShellExecute(NULL, "open", "TimeGraph.py", NULL, NULL, SW_SHOWDEFAULT); /**< Immediately execute the Python file right away. */

    /** DONE GENERATING TIME PYTHON GRAPH SOURCE */


    //-------------------------------------------------------------------------------------------------------



    //-------------------------------------------------------------------------------------------------------


    /** Final parsing and generation of LatencyGraph.py.
     *
     *  This opens the file and modifies the content.
     *  Explicitly declared strings for plotting the logarithmic graphs are specified herwith.
     */
    
    ofstream txtOut4;

    txtOut4.open (latencyGraph);
    txtOut4 << "# Title: LatencyGraph.py - Generates the logarithmic graph based on the C++ benchmark code.\n\n";
    txtOut4 << "# Author: Awwal Mohammed\n\n";
    txtOut4 << "# PLEASE RUN THIS COMMAND TO INSTALL THE REQUIRED MODULES: \n";
    txtOut4 << "# \t pip install matplotlib \n\n";
    txtOut4 << "# Parsing in Logarithmic Graph for Latency...\n\n\n";
    txtOut4 << "import math\n";
    txtOut4 << "import matplotlib.pyplot as plt\n\n\n";
    txtOut4 << latencyYAxis; /**< Actually the initial Y-Axis. */
    txtOut4 << "latencyYAxisNew = []\n"; /**< The updated Y-Axis. */
    txtOut4 << "del latencyYAxis[-1]\n"; /**< Delete the outlying comma. */
    txtOut4 << "print ('Pure Latency Values:')\n";
    txtOut4 << "latencyXAxisLog = []\n"; /**< The X-Axis. */
    txtOut4 << "for i in latencyYAxis:\n";
    txtOut4 << "\ti = i/10000000000\n"; /**< Descaling by 10 billion to revert back to normal. */
    txtOut4 << "\tlatencyYAxisNew.append(i)\n";
    txtOut4 << "\ti = math.log10(i)\n";
    txtOut4 << "\tlatencyXAxisLog.append(i)\n";
    txtOut4 << "print (latencyYAxisNew)\n";
    txtOut4 << "print ('Logarithmic Latency Values:')\n";
    txtOut4 << "print (latencyXAxisLog)\n";
    txtOut4 << "#plotting the Logarithmic Graph\n";
    txtOut4 << "plt.plot(latencyXAxisLog, latencyYAxisNew, label = 'Logarithmic Values of Latency')\n";
    txtOut4 << "plt.ylabel('Latency')\n";
    txtOut4 << "plt.xlabel('Logarithmic Values')\n";
    txtOut4 << "plt.title('Latency Readings for Program 1')\n";
    txtOut4 << "plt.legend()\n";
    txtOut4 << "plt.show()\n";

    txtOut4.close(); /**< Exit editing LatencyGraph.py. */

    ShellExecute(NULL, "open", "LatencyGraph.py", NULL, NULL, SW_SHOWDEFAULT); /**< Immediately execute the Python file right away. */

    /** DONE GENERATING LATENCY PYTHON GRAPH SOURCE */




 //-------------------------------------------------------------------------------------------------------


 //-------------------------------------------------------------------------------------------------------



    /** Final parsing and generation of BandwidthGraph.py.
     *
     *  This opens the file and modifies the content.
     *  Explicitly declared strings for plotting the logarithmic graphs are specified herwith.
     */
    
    ofstream txtOut5;

    txtOut5.open (bandwidthGraph);
    txtOut5 << "# Title: BandwidthGraph.py - Generates the logarithmic graph based on the C++ benchmark code.\n\n";
    txtOut5 << "# Author: Awwal Mohammed\n\n";
    txtOut5 << "# PLEASE RUN THIS COMMAND TO INSTALL THE REQUIRED MODULES: \n";
    txtOut5 << "# \t pip install matplotlib \n\n";
    txtOut5 << "# Parsing in Logarithmic Graph for Bandwidth...\n\n\n";
    txtOut5 << "import math\n";
    txtOut5 << "import matplotlib.pyplot as plt\n\n\n";
    txtOut5 << bandwidthYAxis; /**< Actually the Y-Axis. */
    txtOut5 << "del bandwidthYAxis[0]\n"; /**< Delete the first zero to avoid arithmetic errors. */
    txtOut5 << "del bandwidthYAxis[-1]\n"; /**< Delete the outlying comma. */
    txtOut5 << "bandwidthXAxisLog = []\n"; /**< Logarithmic X-Axis. */
    txtOut5 << "for i in bandwidthYAxis:\n";
    txtOut5 << "\ti = math.log10(i)\n";
    txtOut5 << "\tbandwidthXAxisLog.append(i)\n";
    txtOut5 << "print ('Pure Bandwidth Values:')\n";
    txtOut5 << "print (bandwidthYAxis)\n\n";
    txtOut5 << "print ('Logarithmic Bandwidth Values:')\n";
    txtOut5 << "print (bandwidthXAxisLog)\n\n";
    txtOut5 << "#plotting the Logarithmic Graph\n";
    txtOut5 << "plt.plot(bandwidthXAxisLog, bandwidthYAxis, label = 'Logarithmic Values of Bandwidth')\n";
    txtOut5 << "plt.ylabel('Bandwidth')\n";
    txtOut5 << "plt.xlabel('Logarithmic Values')\n";
    txtOut5 << "plt.title('Bandwidth Readings for Program 1')\n";
    txtOut5 << "plt.legend()\n";
    txtOut5 << "plt.show()\n";

    txtOut5.close(); /**< Exit editing BandwidthGraph.py. */

    ShellExecute(NULL, "open", "BandwidthGraph.py", NULL, NULL, SW_SHOWDEFAULT); /**< Immediately execute the Python file right away. */

    /** DONE GENERATING BANDWIDTH PYTHON GRAPH SOURCE */




     //-------------------------------------------------------------------------------------------------------


 //-------------------------------------------------------------------------------------------------------



    /** Final parsing and generation of BandwidthGraph.py.
     *
     *  This opens the file and modifies the content.
     *  Explicitly declared strings for plotting the logarithmic graphs are specified herwith.
     */
    
    ofstream txtOutC;

    txtOutC.open (combinedGraph);
    txtOutC << "# Title: CombinedGraph.py - Generates the amalgam logarithmic graph based on the C++ benchmark code.\n\n";
    txtOutC << "# Author: Awwal Mohammed\n\n";
    txtOutC << "# PLEASE RUN THIS COMMAND TO INSTALL THE REQUIRED MODULES: \n";
    txtOutC << "# \t pip install matplotlib \n\n";
    txtOutC << "# Parsing in Logarithmic Graph for all data (Bytes, Time, Latency, Bandwidth)...\n\n\n";
    txtOutC << "import math\n";
    txtOutC << "import matplotlib.pyplot as plt\n\n\n";
    
    //------------------------------------------------------------------------------------------
    /** All Y-Axes data specified. */
    txtOutC << bytesYAxis << endl;              /**< Y-Axis for Bytes. */
    txtOutC << timeYAxis << endl;               /**< Y-Axis for Time. */
    txtOutC << latencyYAxis << endl;            /**< Not quite the X-Axis for Latency, this will be corrected soon. */
    txtOutC << bandwidthYAxis << endl;          /**< Y-Axis for Bandwidth. */

    //------------------------------------------------------------------------------------------
    /** Bytes: With the X-Axis sorted, every single array is precariously dealt with at first. */
    txtOutC << "del bytesYAxis[-1]\n";          /**< Delete the outlying comma. */
    txtOutC << "del bytesYAxis[0]\n";           /**< Delete the zero since Log(0) is invalid. */

    txtOutC << "bytesXAxisLog = []\n";          /**< X-Axis for Bytes. */

    txtOutC << "for i in bytesYAxis:\n";
    txtOutC << "\ti = math.log10(i)\n";
    txtOutC << "\tbytesXAxisLog.append(i)\n\n";

    //------------------------------------------------------------------------------------------

    /** Time: With the Y-Axis sorted, every subsequent array is precariously dealt with at first. */
    txtOutC << "del timeYAxis[-1]\n";           /**< Delete the outlying comma. */

    txtOutC << "timeXAxisLog = []\n";           /**< X-Axis for Time. */

    txtOutC << "for i in timeYAxis:\n";
    txtOutC << "\ti = math.log10(i)\n";
    txtOutC << "\ttimeXAxisLog.append(i)\n\n";    




    //------------------------------------------------------------------------------------------

    /** Latency: With the Y-Axis sorted, every subsequent array is precariously dealt with at first. */
    txtOutC << "del latencyYAxis[-1]\n";        /**< Delete the outlying comma. */
    //=========================================================================================
    txtOutC << "latencyYAxisNew = []\n";        /**< ACTUAL Y-AXIS FOR LATENCY: Placeholder array to manage the upscaling. */   
    //=========================================================================================

    txtOutC << "latencyXAxisLog = []\n";        /**< X-Axis for Latency. */

    txtOutC << "for i in latencyYAxis:\n";
    txtOutC << "\ti = i/10000000000\n";         /**< Descaling by 10 billion to revert back to normal. */
    txtOutC << "\tlatencyYAxisNew.append(i)\n";
    txtOutC << "\ti = math.log10(i)\n";
    txtOutC << "\tlatencyXAxisLog.append(i)\n\n";    



    //------------------------------------------------------------------------------------------

    /** Bandwidth: With the X-Axis sorted, every subsequent array is precariously dealt with at first. */
    txtOutC << "del bandwidthYAxis[0]\n";       /**< Delete the first zero to avoid arithmetic errors. */
    txtOutC << "del bandwidthYAxis[-1]\n";      /**< Delete the outlying comma. */

    txtOutC << "bandwidthXAxisLog = []\n";      /**< Y-Axis for Bandwidth. */

    txtOutC << "for i in bandwidthYAxis:\n";
    txtOutC << "\ti = math.log10(i)\n";
    txtOutC << "\tbandwidthXAxisLog.append(i)\n\n"; 

    //------------------------------------------------------------------------------------------


    txtOutC << "#plotting the Logarithmic Graph\n";

    /** Plotting commands for Bandwidth. */
    txtOutC << "plt.plot(bandwidthXAxisLog, bandwidthYAxis, label = 'Logarithmic Values of Bandwidth')\n";

    /** Plotting commands for Latency. */
    txtOutC << "plt.plot(latencyXAxisLog, latencyYAxisNew, label = 'Logarithmic Values of Latency')\n";

    /** Plotting commands for Time. */
    txtOutC << "plt.plot(timeXAxisLog, timeYAxis, label = 'Logarithmic Values of Time')\n";

    /** Plotting commands for Bytes. */
    txtOutC << "plt.plot(bytesXAxisLog, bytesYAxis, label = 'Logarithmic Values of Bytes')\n";

    txtOutC << "plt.ylabel('Aggregate Pure Values')\n";
    txtOutC << "plt.xlabel('Logarithmic Values')\n";
    txtOutC << "plt.title('Combined Comparison Chart For Program 1')\n";
    txtOutC << "plt.legend()\n";
    txtOutC << "plt.show()\n";


    //------------------------------------------------------------------------------------------


    txtOutC.close(); /**< Exit editing CombinedGraph.py. */

    ShellExecute(NULL, "open", "CombinedGraph.py", NULL, NULL, SW_SHOWDEFAULT); /**< Immediately execute the Python file right away. */

    /** DONE GENERATING BANDWIDTH PYTHON GRAPH SOURCE */


    //------------------------------------------------------------------------------------------





    //------------------------------------------------------------------------------------------------------


   //-------------------------------------------------------------------------------------------------------


    /** Final parsing and generation of BytesVsBandwidth.py.
     *
     *  This opens the file and modifies the content.
     *  Explicitly declared strings for plotting the logarithmic graphs are specified herwith.
     */

    ofstream txtOut7;

    txtOut7.open (bytesVsBandwidth);
    txtOut7 << "# Title: BytesVsBandwidth.py - Generates the graph of Bytes against Bandwidth based on the C++ benchmark code.\n\n";
    txtOut7 << "# Author: Awwal Mohammed\n\n";
    txtOut7 << "# PLEASE RUN THIS COMMAND TO INSTALL THE REQUIRED MODULES: \n";
    txtOut7 << "# \t pip install matplotlib \n\n";
    txtOut7 << "# Parsing in Axial values Graph for Bytes and Bandwidth...\n\n\n";
    txtOut7 << "import math\n";
    txtOut7 << "import matplotlib.pyplot as plt\n\n\n";

    txtOut7 << bytesYAxis; /**< The Y-Axis Now. */
    txtOut7 << "del bytesYAxis[-1]\n"; /**< Delete the outlying comma. */
    txtOut7 << "del bytesYAxis[0]\n"; /**< Delete the zero since Log(0) is invalid. */
    txtOut7 << "print ('Pure Byte Values:')\n";
    txtOut7 << "print (bytesYAxis)\n\n";

    txtOut7 << bandwidthYAxis; /**< The X-Axis but just for now. */
    txtOut7 << "del bandwidthYAxis[0]\n"; /**< Delete the first zero to avoid arithmetic errors. */
    txtOut7 << "del bandwidthYAxis[-1]\n"; /**< Delete the outlying comma. */   
    txtOut7 << "print ('Pure Bandwidth Values:')\n";
    txtOut7 << "print (bandwidthYAxis)\n\n";

    txtOut7 << "#Plotting Bytes against Bandwidth\n";
    txtOut7 << "plt.plot(bandwidthYAxis, bytesYAxis, label = 'Bytes Against Bandwidth')\n";
    txtOut7 << "plt.ylabel('Bytes')\n";
    txtOut7 << "plt.xlabel('Bandwidth')\n";
    txtOut7 << "plt.title('Bytes vs Bandwidth for Program 1')\n";
    txtOut7 << "plt.legend()\n";
    txtOut7 << "plt.show()\n";

    txtOut7.close(); /**< Exit editing BytesVsBandwidth.py. */

    ShellExecute(NULL, "open", "BytesVsBandwidth.py", NULL, NULL, SW_SHOWDEFAULT); /**< Immediately execute the Python file right away. */

    /** DONE GENERATING BYTES VS BANDWIDTH PYTHON GRAPH SOURCE */


    //------------------------------------------------------------------------------------------------------


   //-------------------------------------------------------------------------------------------------------


    /** Final parsing and generation of BytesVsLatency.py.
     *
     *  This opens the file and modifies the content.
     *  Explicitly declared strings for plotting the logarithmic graphs are specified herwith.
     */

    ofstream txtOut8;

    txtOut8.open (bytesVsLatency);
    txtOut8 << "# Title: BytesVsLatency.py - Generates the graph of Bytes against Latency based on the C++ benchmark code.\n\n";
    txtOut8 << "# Author: Awwal Mohammed\n\n";
    txtOut8 << "# PLEASE RUN THIS COMMAND TO INSTALL THE REQUIRED MODULES: \n";
    txtOut8 << "# \t pip install matplotlib \n\n";
    txtOut8 << "# Parsing in Axial values Graph for Bytes and Latency...\n\n\n";
    txtOut8 << "import math\n";
    txtOut8 << "import matplotlib.pyplot as plt\n\n\n";

    txtOut8 << bytesYAxis; /**< The Y-Axis Now. */
    txtOut8 << "del bytesYAxis[-1]\n"; /**< Delete the outlying comma. */
    txtOut8 << "del bytesYAxis[0]\n"; /**< Delete the zero since Log(0) is invalid. */
    txtOut8 << "print ('Pure Byte Values:')\n";
    txtOut8 << "print (bytesYAxis)\n\n";

    txtOut8 << latencyYAxis; /**< Actually the initial X-Axis. */
    txtOut8 << "latencyYAxisNew = []\n"; /**< The updated X-Axis. */
    txtOut8 << "del latencyYAxis[-1]\n"; /**< Delete the outlying comma. */
    txtOut8 << "print ('Pure Latency Values:')\n\n";
    txtOut8 << "for i in latencyYAxis:\n";
    txtOut8 << "\ti = i/10000000000\n"; /**< Descaling by 10 billion to revert back to normal. */
    txtOut8 << "\tlatencyYAxisNew.append(i)\n\n";
    txtOut8 << "del latencyYAxisNew[0]\n"; /**< Deleting the first value to coincied with Bytes. */
    txtOut8 << "print (latencyYAxisNew)\n\n";
    


    txtOut8 << "#Plotting Bytes against Bandwidth\n";
    txtOut8 << "plt.plot(latencyYAxisNew, bytesYAxis, label = 'Bytes Against Latency')\n";
    txtOut8 << "plt.ylabel('Bytes')\n";
    txtOut8 << "plt.xlabel('Latency')\n";
    txtOut8 << "plt.title('Bytes vs Latency for Program 1')\n";
    txtOut8 << "plt.legend()\n";
    txtOut8 << "plt.show()\n";

    txtOut8.close(); /**< Exit editing BytesVsLatency.py. */

    ShellExecute(NULL, "open", "BytesVsLatency.py", NULL, NULL, SW_SHOWDEFAULT); /**< Immediately execute the Python file right away. */

    /** DONE GENERATING BYTES VS LATENCY PYTHON GRAPH SOURCE */


    //------------------------------------------------------------------------------------------------------


   //-------------------------------------------------------------------------------------------------------





    return 0;

} /**< End Main Function. */



///////////////////////////////////////////////////////////////////////////////