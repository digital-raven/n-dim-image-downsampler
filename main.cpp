/*******************************************************************************
 * Downsampling
 *
 * Description:
 *  This program will read an array of integers represeinting a N-dimensional
 *  image. It will then downsample the image by a given level. The user should
 *  write a data file containing the array and other information. This massively
 *  helps with the enevitably large input sizes. This datafile should first
 *  contain a number representing the number of dimensions of the image n,
 *  followed by n integers representing the lengths of each side of the image.
 *  This array will be called L. The whole image will be
 *      2^L1 * 2^L2 * 2^L3... * 2^Ln
 *  large, and this size will be called s. The next line should contain the
 *  level of downsampling to be performed on the image, which should be no
 *  larger than the minimum value in L. There should then be s integers
 *  representing the image itself.
 *
 * Usage: ./downsample
*******************************************************************************/
#include "NDimImage.h"
#include <iostream>
#include <string>
using namespace std;

/*******************************************************************************
 *  calc_size
 *
 *  Given an array of exponents, returns 2^arr[0] + 2^arr[1]... + 2^arr[n] 
 *  where n is the size of the array.
 *
 *  Parameters:
 *      expos (i): Array of exponents. Each length is 2^expos[i]
 *      num_expos(i): Number of elements in expos
 *
 *  Returns:
 *      Number of pixels present in image of described dimensions
*******************************************************************************/
unsigned int calc_size(const unsigned int *expos, const unsigned int num_expos)
{
    unsigned int size = 1;
    for (unsigned int i = 0; i < num_expos; i++)
    {
        size *= 1 << expos[i];
    }
    return size;
}

int main(int argc, const char **argv)
{
    unsigned int num_dims, image_size, *dim_expos, *arr, level;

    // Read number of dimensions and each dimension
    cin >> num_dims;
    dim_expos = new unsigned int[num_dims];
    for (unsigned int i = 0; i < num_dims; i++)
    {
        cin >> dim_expos[i];
    }

    // Read level of downsampling to be performed
    cin >> level;

    // Calculate image size based on dimensions and read that many integers
    image_size = calc_size(dim_expos, num_dims);
    arr = new unsigned int[image_size];
    for (unsigned int i = 0; i < image_size; i++)
    {
        cin >> arr[i];
    }

    // Create and downsample image
    NDimImage image(arr, dim_expos, num_dims);
    image.getDownSampled(level).printImage();

    delete[] arr;
    delete[] dim_expos;

    return 0;
}
