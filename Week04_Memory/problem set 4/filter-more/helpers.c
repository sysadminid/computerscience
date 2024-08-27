#include "helpers.h"
#include <math.h>
#include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        int avg = 0;
        for (int j = 0; j < width; j++)
        {
            avg = round(
                ((float) image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3);
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int row = width / 2;
    RGBTRIPLE tmp[1][1];

    for (int i = 0; i < height; i++)
    {
        // swap pixels horizontally
        for (int j = 0; j < row; j++)
        {
            tmp[0][0] = image[i][j];
            image[i][j] = image[i][(width - 1) - j];
            image[i][(width - 1) - j] = tmp[0][0];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // create a temporary image to store the blurred values
    RGBTRIPLE(*temp_px)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (temp_px == NULL)
    {
        return;
    }

    // iterate over each pixel in the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // initialize sums for each color & counter
            float sumBlue = 0, sumGreen = 0, sumRed = 0;
            int counter = 0;

            // iterate over the neighboring pixels
            for (int x = -1; x <= 1; x++)
            {
                for (int y = -1; y <= 1; y++)
                {
                    int ni = i + x;
                    int nj = j + y;

                    // check if neighbor is within image bounds
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        sumBlue += image[ni][nj].rgbtBlue;
                        sumGreen += image[ni][nj].rgbtGreen;
                        sumRed += image[ni][nj].rgbtRed;
                        counter++;
                    }
                }
            }
            // calculate the average color and assign it to `temp_px`
            temp_px[i][j].rgbtBlue = round(sumBlue / counter);
            temp_px[i][j].rgbtGreen = round(sumGreen / counter);
            temp_px[i][j].rgbtRed = round(sumRed / counter);
        }
    }

    // copy the `temp_px` array back to `image` array
    for (int a = 0; a < height; a++)
    {
        for (int b = 0; b < width; b++)
        {
            image[a][b] = temp_px[a][b];
        }
    }
    free(temp_px);
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // create a temporary image to store the blurred values
    RGBTRIPLE(*temp_px)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (temp_px == NULL)
    {
        return;
    }

    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // iterate over each pixel in the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // initialize gx & gy for each color
            int gxBlue = 0, gxGreen = 0, gxRed = 0;
            int gyBlue = 0, gyGreen = 0, gyRed = 0;

            // iterate over the neighboring pixels
            for (int x = -1; x <= 1; x++)
            {
                for (int y = -1; y <= 1; y++)
                {
                    int ni = i + x;
                    int nj = j + y;

                    // check if neighbor is within image bounds
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        // gx gy - convolutional matrix
                        gxBlue += gx[x + 1][y + 1] * image[ni][nj].rgbtBlue;
                        gyBlue += gy[x + 1][y + 1] * image[ni][nj].rgbtBlue;

                        gxGreen += gx[x + 1][y + 1] * image[ni][nj].rgbtGreen;
                        gyGreen += gy[x + 1][y + 1] * image[ni][nj].rgbtGreen;

                        gxRed += gx[x + 1][y + 1] * image[ni][nj].rgbtRed;
                        gyRed += gy[x + 1][y + 1] * image[ni][nj].rgbtRed;
                    }
                }
            }
            // find the gradient magnitude for each color
            int gBlue = round(sqrt(gxBlue * gxBlue + gyBlue * gyBlue));
            int gGreen = round(sqrt(gxGreen * gxGreen + gyGreen * gyGreen));
            int gRed = round(sqrt(gxRed * gxRed + gyRed * gyRed));

            // save it to `temp_px` array
            temp_px[i][j].rgbtBlue = (gBlue > 255) ? 255 : gBlue;
            temp_px[i][j].rgbtGreen = (gGreen > 255) ? 255 : gGreen;
            temp_px[i][j].rgbtRed = (gRed > 255) ? 255 : gRed;
        }
    }

    // copy the `temp_px` array back to `image` array
    for (int a = 0; a < height; a++)
    {
        for (int b = 0; b < width; b++)
        {
            image[a][b] = temp_px[a][b];
        }
    }
    free(temp_px);
    return;
}
