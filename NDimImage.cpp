#include "NDimImage.h"
#include <math.h>
#include <cstring>
#include <unordered_map>
#include <algorithm>
#include <thread>
#include <vector>
using namespace std;

vector<thread> threads;

// Create empty image with no values and a mode of 0
NDimImage::NDimImage()
{
    image = NULL;
    dim_expos = NULL;
    dims = NULL;
    image_size = 0;
    num_dims = 0;
    mode = 0;
}

// Overloaded constructor accepting new array of integers, dimensional
// exponents, and number of dimensions of image.
NDimImage::NDimImage(const unsigned int *image, const unsigned int *dim_expos,
    const unsigned int num_dims)
{
    this->num_dims = num_dims;
    this->dim_expos = new unsigned int[num_dims];
    this->dims = new unsigned int[num_dims];
    this->image_size = 1;
    unordered_map<int, int> num_counts;

    // Copy dim_expos, calculate length of each side, and calculate total image
    // size
    for (unsigned int i = 0; i < num_dims; i++)
    {
        this->dim_expos[i] = dim_expos[i];
        this->dims[i] = 1 << dim_expos[i];
        this->image_size *= dims[i];
    }

    // Don't use memcpy for copying the image data since we'd have to iterate
    // through to calculate the mode anyway.
    // We calculate the mode by paring each value with the number of occurances
    // in a hash table. This is for quick lookup while comparing number of
    // occurances of our current value to the occurences of the mode.
    this->image = new unsigned int[image_size];
    this->mode = image[0];
    for (unsigned int i = 0; i < image_size; i++)
    {
        this->image[i] = image[i];
        if (num_counts.find(image[i]) == num_counts.end())
        {
            num_counts[image[i]] = 1;
        }
        else
        {
            num_counts.find(image[i])->second += 1;
        }

        if (num_counts.find(image[i])->second
            > num_counts.find(this->mode)->second)
        {
            this->mode = image[i];
        }
    }
}

// Copy all data from other image to this one
NDimImage::NDimImage(const NDimImage &other)
{
    image_size = other.image_size;
    num_dims = other.num_dims;
    mode = other.mode;
    image = new unsigned int[image_size];
    dims = new unsigned int[num_dims];
    dim_expos = new unsigned int[num_dims];
    memcpy(image, other.image, image_size * sizeof(unsigned int));
    memcpy(dims, other.dims, num_dims * sizeof(unsigned int));
    memcpy(dim_expos, other.dim_expos, num_dims * sizeof(unsigned int));
}

// Delete image, dim_expos, and dims
NDimImage::~NDimImage()
{
    if (image)
        delete[] image;
    if (dim_expos)
        delete[] dim_expos;
    if (dims)
        delete[] dims;
}

// Return new NDimImage class from downsampled version of this image. level is
// the desired level of downsampling, and must be <= lowest value in dim_expos
NDimImage NDimImage::getDownSampled(unsigned int level) const
{
    // Just return a copy of this if we're not downsampling anything
    if (level == 0)
    {
        return NDimImage(*this);
    }
    else
    {
        // Calculate number of blocks we will need
        // Also the total size of the down sampled image
        unsigned int num_blocks = image_size;
        unsigned int *down_sampled_expos = new unsigned int[num_dims];
        for (unsigned int i = 0; i < num_dims; i++)
        {
            num_blocks = num_blocks >> level;
            down_sampled_expos[i] = dim_expos[i] - level;
        }

        // Get starting locations for each sub block
        unsigned int sub_block_side = 1 << level;
        unsigned int *start = new unsigned int[num_dims];
        memset(start, 0, num_dims * sizeof(int));

        // Get modes from our sub images
        unsigned int *modes = new unsigned int[num_blocks];
        unsigned int block_num = 0;
        getDownSampled(modes, start, block_num, 0, level);
        for (size_t i = 0; i < threads.size(); i++)
        {
            threads[i].join();
        }
        threads.clear();

        // Combine modes of each sub image to form new image
        auto down_sampled = NDimImage(modes, down_sampled_expos, num_dims);

        // Cleanup and return
        delete[] down_sampled_expos;
        delete[] start;
        delete[] modes;

        return down_sampled;
    }
}

// Return sub image created from cube secton of original image.
// lengh of each 
NDimImage NDimImage::getSubImage(const unsigned int *start,
    const unsigned int l) const
{
    // Calculate ending coordinates
    unsigned int *side_exps = new unsigned int[num_dims];
    for (unsigned int i = 0; i < num_dims; i++)
    {
        side_exps[i] = l;
    }

    unsigned int size = static_cast<int>(pow(1<<l, num_dims));
    unsigned int *sub_image = new unsigned int[size];

    unsigned int sub_image_index = 0;
    unsigned int *coords = new unsigned int[num_dims];
    unsigned int side_length = 1 << l;

    getSubImage(sub_image, sub_image_index, start, coords, 0, 0, image_size,
        side_length);

    NDimImage ret_image = NDimImage(sub_image, side_exps, num_dims);

    delete[] sub_image;
    delete[] side_exps;
    delete[] coords;

    return ret_image;
}

// Print each value in image
void NDimImage::printImage() const
{
    unsigned int *start = new unsigned int[num_dims];
    memset(start, 0, num_dims * sizeof(int));
    unsigned int *coords = new unsigned int[num_dims];

    printImage(start, dims, coords, 0, 0, image_size);

    delete[] start;
    delete[] coords;
}  

unsigned int *NDimImage::getDims() const
{
    unsigned int *temp_dims = new unsigned int[num_dims];
    memcpy(temp_dims, dims, num_dims * sizeof(unsigned int));
    return temp_dims;
}

// Private helpers
// Private print function. Recursively iterates through whole image printing
// each value. Each new dimension is separated by new lines
void NDimImage::printImage(const unsigned int *start, const unsigned int *end,
    unsigned int *coords, unsigned int i, unsigned int image_index,
    unsigned int area_covered) const
{
    if (i == num_dims)
    {
        printf("%d ", image[image_index]);
    }
    else
    {
        area_covered /= dims[i];
        for (unsigned int j = start[i]; j < end[i]; j++)
        {
            coords[i] = j;
            printImage(start, end, coords, i + 1,
                image_index + (area_covered * coords[i]), area_covered);
        }
        printf("\n");
    }
}

void write_mode(const NDimImage *image, unsigned int *modes,
    unsigned int block_num, unsigned int *coords, const unsigned int level)
{
    modes[block_num] = image->getSubImage(coords, level).getMode();

    // We delete coords here instead of in getDownSampled because there's no
    // guarantee the thread will start as we create it.
    delete[] coords;
}

void NDimImage::getDownSampled(unsigned int *modes, unsigned int *coords,
    unsigned int &block_num, unsigned int i, const unsigned int level) const
{
    if (i == num_dims)
    {
        unsigned int *temp_coords = new unsigned int[num_dims];
        memcpy(temp_coords, coords, num_dims * sizeof(unsigned int));
        threads.push_back(thread(write_mode, this, modes, block_num,
            temp_coords, level));
        block_num++;
    }
    else
    {
        for (unsigned int j = 0; j < dims[i]; j += 1 << level)
        {
            coords[i] = j;
            getDownSampled(modes, coords, block_num, i + 1, level);
        }
    }
}

void NDimImage::getSubImage(unsigned int *sub_image,
    unsigned int &sub_image_index, const unsigned int *start,
    unsigned int *coords, unsigned int i, unsigned int image_index,
    unsigned int area_covered, const unsigned int side_length) const
{
    if (i == num_dims)
    {
        sub_image[sub_image_index] = image[image_index];
        sub_image_index++;
    }
    else
    {
        area_covered /= dims[i];
        for (unsigned int j = start[i]; j < start[i] + side_length; j++)
        {
            coords[i] = j;
            getSubImage(sub_image, sub_image_index, start, coords, i + 1,
                image_index + (area_covered * coords[i]), area_covered,
                side_length);
        }
    }
}
