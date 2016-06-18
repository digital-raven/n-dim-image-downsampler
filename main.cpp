/*******************************************************************************
 * Downsampling
 *
 * Description:
 *  This program will read an array of integers represeinting a N-dimensional
 *  image. It will then downsample the image by a given level. For convenience,
 *  it is recommended a user write a data file containing the array and other
 *  information. This datafile should first contain a number n representing the
 *  number of dimensions of the image, followed by n integers representing the
 *  lengths of each side of the image. This array will be called L. The whole
 *  image will be
 *      2^L1 * 2^L2 * 2^L3... * 2^Ln
 *  large, and this size will be called s. The next line should contain the
 *  level of downsampling to be performed on the image, which should be no
 *  larger than the minimum value in L. There should then be s integers
 *  representing the image itself.
 *  Negative values should not be used.
 *
 * Usage: ./downsample < datafile.dat
*******************************************************************************/
#include "NDimImage.h"
#include <time.h>
#include <iostream>
#include <string>
using namespace std;

// Resulting image size is 2^L1 * 2^L2 * 2^L3... * 2^Ln where n = num_expos
unsigned int calc_size(const unsigned int *expos, const unsigned int num_expos)
{
    unsigned int size = 1;
    for (unsigned int i = 0; i < num_expos; i++)
    {
        size *= 1 << expos[i];
    }
    return size;
}

int main(int argc, char *argv[])
{
    unsigned int num_dims, image_size, *dim_expos, *arr, level;

    cin >> num_dims;
    dim_expos = new unsigned int[num_dims];
    for (unsigned int i = 0; i < num_dims; i++)
    {
        cin >> dim_expos[i];
    }
    cin >> level;

    image_size = calc_size(dim_expos, num_dims);
    arr = new unsigned int[image_size];
    for (unsigned int i = 0; i < image_size; i++)
    {
        cin >> arr[i];
    }

    NDimImage image(arr, dim_expos, num_dims);
    auto start_ticks = clock();
    auto start_secs = time(0);
    image.getDownSampled(level).printImage();
    int ticks_since_start = clock() - start_ticks;
    double secs_since_start = time(0) - start_secs;
    printf("Ticks: %d ticks\n", ticks_since_start);
    printf("Seconds: %lf seconds\n", secs_since_start);
    

    delete[] arr;
    delete[] dim_expos;

    return 0;
}
