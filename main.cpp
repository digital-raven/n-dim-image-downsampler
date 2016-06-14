/*******************************************************************************
 * Downsampling
 *
 * Description:
 *  This program will read an array of integers represeinting a N-dimensional
 *  image. It will then downsample the image by a given level. The user is
 *  responsible for writing a data file containing the array and other
 *  information. This datafile should first contain a number representing the
 *  number of dimensions of the image n, followed by n integers representing the
 *  lengths of each side of the image. This array will be called L. The whole
 *  image will be
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
#include <fstream>
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
    ifstream infile;
    string filename;

    // Read filename and open file for reading
    cout << "Enter filename -> ";
    cin >> filename;
    infile.open(filename);
    if (!infile.is_open())
    {
        printf("File %s not found", filename.c_str());
        return 1;
    }

    // Read number of dimensions and each dimension
    infile >> num_dims;
    dim_expos = new unsigned int[num_dims];
    for (unsigned int i = 0; i < num_dims; i++)
    {
        infile >> dim_expos[i];
    }

    // Read level of downsampling to be performed
    infile >> level;

    // Calculate image size based on dimensions and read that many integers
    image_size = calc_size(dim_expos, num_dims);
    arr = new unsigned int[image_size];
    for (unsigned int i = 0; i < image_size; i++)
    {
        infile >> arr[i];
    }
    infile.close();

    // Create and downsample image
    NDimImage image(arr, dim_expos, num_dims);
    image.getDownSampled(level).printImage();

    delete[] arr;
    delete[] dim_expos;

    return 0;
}
