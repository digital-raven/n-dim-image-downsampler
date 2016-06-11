#pragma once
#include <functional>

class NDimImage
{
    public:

/*******************************************************************************
 * NDimImage
 *
 * Empty image, no dimensions, mode of 0
*******************************************************************************/
        NDimImage();

/*******************************************************************************
 * NDimImage overloaded
 *
 * Creates a new NDimImage instance from a one dimensional array of integers,
 * an array of dimensional measurements, and the number of dimensions in the
 * image.
 *
 * Parameters:
 *  image (I): Array of integers which will be copied to this.
 *  dim_expos I: Array of integers indicating lengths of sides of image. Should
 *      be same size as num_dims. image should be
 *      2^L1 * 2^L2 * 2^L3... * 2^Ln large, where n is the length of dim_expos.
 *  num_dims (I): lengh of dim_expos.
*******************************************************************************/
        NDimImage(const unsigned int *image, const unsigned int *dim_expos,
            const unsigned int num_dims);

/*******************************************************************************
 * NDimImage copy
 *
 * Deep copies all data from other to create a new NDimImage class
*******************************************************************************/
        NDimImage(const NDimImage &other);

/*******************************************************************************
 * ~NDimImage
 *
 * Destroys this
*******************************************************************************/
        ~NDimImage();

/*******************************************************************************
 * getDownSampled
 *
 * Returns a new NDimImage instance created from a downsampled version of this
 *
 * Parameters:
 *  level (I): Desired level of downsampling. Should only be as large as the
 *      smallest value in dim_expos.
*******************************************************************************/
        NDimImage getDownSampled(unsigned int level) const;

/*******************************************************************************
 * getSubImage
 *
 * Returns a new NDimImage instance created from a sub-cube of the original
 * image.
 *
 * Parameters:
 *  start (I): array of integers indicating location of starting corner. Should
 *      be the same size as num_dims
 *  l (I):  Desired length of each side of the cube. Cube size will be
 *      (2^l)^num_dims
*******************************************************************************/
        NDimImage getSubImage(const unsigned int *start,
            const unsigned int l) const;

/*******************************************************************************
 * printImage
 *
 * Prints the image with each 'face' separated by new lines. More newlines = 
 * more dimensinal separation.
*******************************************************************************/
        void printImage() const;

		// Simple getters
		unsigned int getMode() const { return mode; }
		unsigned int getNumDims() const { return num_dims; }
		unsigned int getImageSize() const { return image_size; }
		unsigned int *getDims() const {
			unsigned int *temp_dims = new unsigned int[num_dims];
			memcpy(temp_dims, dims, num_dims * sizeof(unsigned int));
			return temp_dims;
		}

    private:

        // Methods
		void printImage(const unsigned int *start, const unsigned int *end,
			unsigned int *coords, unsigned int i, unsigned int image_index,
			unsigned int area_covered) const;
		void getDownSampled(unsigned int *modes, unsigned int *coords,
			unsigned int &block_num, unsigned int i, const unsigned int level) const;
		void getSubImage(unsigned int *sub_image,
			unsigned int &sub_image_index, const unsigned int *start,
			unsigned int *coords, unsigned int i, unsigned int image_index,
			unsigned int area_covered, const unsigned int side_length) const;

        // Data
        // Raw image data, and dims[n] == 2^dim_expos[n]. 'expos' stands for
        // exponents
        unsigned int *image;
        unsigned int *dim_expos;
        unsigned int *dims;

        // Total number of pixels in image and number of dimensions
        unsigned int image_size;
        unsigned int num_dims;

        // Most commonly occuring value in image
        unsigned int mode;
};