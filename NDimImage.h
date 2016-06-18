#pragma once
/*******************************************************************************
 * The NDimImage class represents an N dimensional image where N may be decided
 * at runtime. The underlying storage is simply a massive one dimensional array
 * treated as a multi dimensional image depending on the number of requested
 * dimensions and the lengths of each of the image's sides.
 *
 * There are methods to grab sub blocks from the image, to downsample it, and
 * basic getters and setters.
*******************************************************************************/
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
 *  I: image - Array of integers which will be copied to this.
 *  I: dim_expos - Each side of the image will be 2^dim_expos[i] large, where i
 *      is an index in dim_expos.
 *  I: num_dims - lengh of dim_expos.
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
 *  I: level - Desired level of downsampling. Should only be as large as the
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
 *  I: start - array of integers indicating location of starting corner. Should
 *      be the same size as num_dims
 *  I: l - Desired length of each side of the cube. Cube size will be
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

        /* Simple Getters */
        unsigned int getMode() const { return mode; }
        unsigned int getNumDims() const { return num_dims; }
        unsigned int getImageSize() const { return image_size; }


/*******************************************************************************
 * getDims
 *
 * This function returns a memory copy of the array holding the lengths of each
 * of the image's sides. New memory is created with this method, so the caller
 * is responsible for cleanup.
*******************************************************************************/
        unsigned int *getDims() const;

    private:

        // Recursive helper methods for printing, downsampling, and grabbing
        // a sub image
        void printImage(unsigned int *coords, unsigned int i,
            unsigned int image_index, unsigned int area_covered) const;
        void getDownSampled(unsigned int *modes, unsigned int *coords,
            unsigned int &block_num, unsigned int i,
            const unsigned int level) const;
        void getSubImage(unsigned int *sub_image,
            unsigned int &sub_image_index, const unsigned int *start,
            unsigned int *coords, unsigned int i, unsigned int image_index,
            unsigned int area_covered, const unsigned int side_length) const;
        
        // Data
        // Raw image data, 
        unsigned int *image;
        unsigned int *dim_expos; // dims[n] == 2^dim_expos[n]. 'expos' stands
        unsigned int *dims;      // for exponents

        // Total number of pixels in image and number of dimensions
        unsigned int image_size;
        unsigned int num_dims;

        // most commonly occuring value in image
        unsigned int mode;
};

