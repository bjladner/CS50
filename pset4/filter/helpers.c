#include "helpers.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int average = 0;
    // Traverse each RGB triple of the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculate the average of the red, green, and blue;
            average = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / (float) 3);
            // Store resulting average in each of the red, green, and blue byte
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a temporary RGBTRIPLE to store the triple that is moving
    RGBTRIPLE temp;
    // Traverse only half of the RGB triples of the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            // Swap pixel with the reflected pixel
            temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    int total_red = 0;
    int total_green = 0;
    int total_blue = 0;
    int total_red2 = 0;
    int total_green2 = 0;
    int total_blue2 = 0;

    // Allocate memory for image
    RGBTRIPLE(*blurred)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (blurred == NULL)
    {
        fprintf(stderr, "Not enough memory to store image.\n");
        return;
    }

    // Case for edge row blur function
    for (int j = 1; j < width - 1; j++)
    {
        // Reset counts when we move to the next column
        total_red = 0;
        total_blue = 0;
        total_green = 0;
        total_red2 = 0;
        total_blue2 = 0;
        total_green2 = 0;
        // Traverse the bits adjacent to this pixel on the 1st and last row
        for (int w = -1; w < 2; w++)
        {
            for (int h = 0; h < 2; h++)
            {
                total_red += image[h][j + w].rgbtRed;
                total_blue += image[h][j + w].rgbtBlue;
                total_green += image[h][j + w].rgbtGreen;
                total_red2 += image[height - 1 + h - 1][j + w].rgbtRed;
                total_blue2 += image[height - 1 + h - 1][j + w].rgbtBlue;
                total_green2 += image[height - 1 + h - 1][j + w].rgbtGreen;
            }
        }
        // Calculate average for blur function
        blurred[0][j].rgbtRed = round(total_red / (float) 6);
        blurred[0][j].rgbtBlue = round(total_blue / (float) 6);
        blurred[0][j].rgbtGreen = round(total_green / (float) 6);
        blurred[height - 1][j].rgbtRed = round(total_red2 / (float) 6);
        blurred[height - 1][j].rgbtBlue = round(total_blue2 / (float) 6);
        blurred[height - 1][j].rgbtGreen = round(total_green2 / (float) 6);
    }

    // Case for edge column blur function
    for (int i = 1; i < height - 1; i++)
    {
        // Reset counts when we move to the next row
        total_red = 0;
        total_blue = 0;
        total_green = 0;
        total_red2 = 0;
        total_blue2 = 0;
        total_green2 = 0;
        // Traverse the bits adjacent to this pixel on the 1st and last column
        for (int h = -1; h < 2; h++)
        {
            for (int w = 0; w < 2; w++)
            {
                total_red += image[i + h][w].rgbtRed;
                total_blue += image[i + h][w].rgbtBlue;
                total_green += image[i + h][w].rgbtGreen;
                total_red2 += image[i + h][width - 1 + w - 1].rgbtRed;
                total_blue2 += image[i + h][width - 1 + w - 1].rgbtBlue;
                total_green2 += image[i + h][width - 1 + w - 1].rgbtGreen;
            }
        }
        // Calculate average for blur function
        blurred[i][0].rgbtRed = round(total_red / (float) 6);
        blurred[i][0].rgbtBlue = round(total_blue / (float) 6);
        blurred[i][0].rgbtGreen = round(total_green / (float) 6);
        blurred[i][width - 1].rgbtRed = round(total_red2 / (float) 6);
        blurred[i][width - 1].rgbtBlue = round(total_blue2 / (float) 6);
        blurred[i][width - 1].rgbtGreen = round(total_green2 / (float) 6);
    }

    // Internal pixel blur function
    for (int i = 1; i < height - 1; i++)
    {
        for (int j = 1; j < width - 1; j++)
        {
            // Reset counts when we move to the next pixel
            total_red = 0;
            total_blue = 0;
            total_green = 0;
            // Traverse the bits adjacent to this pixel
            for (int h = -1; h < 2; h++)
            {
                for (int w = -1; w < 2; w++)
                {
                    total_red += image[i + h][j + w].rgbtRed;
                    total_blue += image[i + h][j + w].rgbtBlue;
                    total_green += image[i + h][j + w].rgbtGreen;
                }
            }
            // Calculate average for blur function
            blurred[i][j].rgbtRed = round(total_red / (float) 9);
            blurred[i][j].rgbtBlue = round(total_blue / (float) 9);
            blurred[i][j].rgbtGreen = round(total_green / (float) 9);
        }
    }

    // Case for corner cases
    // Reset counts for corner cases
    total_red = 0;
    total_blue = 0;
    total_green = 0;
    total_red2 = 0;
    total_blue2 = 0;
    total_green2 = 0;
    // Traverse the bits adjacent to this pixel
    for (int h = 0; h < 2; h++)
    {
        for (int w = 0; w < 2; w++)
        {
            total_red += image[h][w].rgbtRed;
            total_blue += image[h][w].rgbtBlue;
            total_green += image[h][w].rgbtGreen;
            total_red2 += image[h][width - 1 + w - 1].rgbtRed;
            total_blue2 += image[h][width - 1 + w - 1].rgbtBlue;
            total_green2 += image[h][width - 1 + w - 1].rgbtGreen;
        }
    }
    // Calculate average for blur function
    blurred[0][0].rgbtRed = round(total_red / (float) 4);
    blurred[0][0].rgbtBlue = round(total_blue / (float) 4);
    blurred[0][0].rgbtGreen = round(total_green / (float) 4);
    blurred[0][width - 1].rgbtRed = round(total_red2 / (float) 4);
    blurred[0][width - 1].rgbtBlue = round(total_blue2 / (float) 4);
    blurred[0][width - 1].rgbtGreen = round(total_green2 / (float) 4);

    // Reset counts for corner cases
    total_red = 0;
    total_blue = 0;
    total_green = 0;
    total_red2 = 0;
    total_blue2 = 0;
    total_green2 = 0;
    // Traverse the bits adjacent to this pixel
    for (int h = 0; h < 2; h++)
    {
        for (int w = 0; w < 2; w++)
        {
            total_red += image[height - 1 + h - 1][w].rgbtRed;
            total_blue += image[height - 1 + h - 1][w].rgbtBlue;
            total_green += image[height - 1 + h - 1][w].rgbtGreen;
            total_red2 += image[height - 1 + h - 1][width - 1 + w - 1].rgbtRed;
            total_blue2 += image[height - 1 + h - 1][width - 1 + w - 1].rgbtBlue;
            total_green2 += image[height - 1 + h - 1][width - 1 + w - 1].rgbtGreen;
        }
    }
    // Calculate average for blur function
    blurred[height - 1][0].rgbtRed = round(total_red / (float) 4);
    blurred[height - 1][0].rgbtBlue = round(total_blue / (float) 4);
    blurred[height - 1][0].rgbtGreen = round(total_green / (float) 4);
    blurred[height - 1][width - 1].rgbtRed = round(total_red2 / (float) 4);
    blurred[height - 1][width - 1].rgbtBlue = round(total_blue2 / (float) 4);
    blurred[height - 1][width - 1].rgbtGreen = round(total_green2 / (float) 4);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = blurred[i][j];
        }
    }
    // Free memory for image
    free(blurred);

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    float sobel_x_red = 0;
    float sobel_x_blue = 0;
    float sobel_x_green = 0;
    float sobel_y_red = 0;
    float sobel_y_blue = 0;
    float sobel_y_green = 0;
    int temp_red = 0;
    int temp_blue = 0;
    int temp_green = 0;

    // Allocate memory for image
    RGBTRIPLE(*edgedet)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (edgedet == NULL)
    {
        fprintf(stderr, "Not enough memory to store image.\n");
        return;
    }

    // Case for edge row edge detection function
    for (int j = 1; j < width - 1; j++)
    {
        // Calculate the sobel for each color and for x and y
        sobel_x_red = 2 * image[0][j + 1].rgbtRed - 2 * image[0][j - 1].rgbtRed;
        sobel_x_red += image[1][j + 1].rgbtRed - image[1][j - 1].rgbtRed;
        sobel_x_blue = 2 * image[0][j + 1].rgbtBlue - 2 * image[0][j - 1].rgbtBlue;
        sobel_x_blue += image[1][j + 1].rgbtBlue - image[1][j - 1].rgbtBlue;
        sobel_x_green = 2 * image[0][j + 1].rgbtGreen - 2 * image[0][j - 1].rgbtGreen;
        sobel_x_green += image[1][j + 1].rgbtGreen - image[1][j - 1].rgbtGreen;
        sobel_y_red = image[1][j - 1].rgbtRed + 2 * image[1][j].rgbtRed + image[1][j + 1].rgbtRed;
        sobel_y_blue = image[1][j - 1].rgbtBlue + 2 * image[1][j].rgbtBlue + image[1][j + 1].rgbtBlue;
        sobel_y_green = image[1][j - 1].rgbtGreen + 2 * image[1][j].rgbtGreen + image[1][j + 1].rgbtGreen;

        // Combine x and y sobel values for each color
        temp_red = (int) round(sqrt(pow(sobel_x_red, 2) + pow(sobel_y_red, 2)));
        temp_blue = (int) round(sqrt(pow(sobel_x_blue, 2) + pow(sobel_y_blue, 2)));
        temp_green = (int) round(sqrt(pow(sobel_x_green, 2) + pow(sobel_y_green, 2)));
        // Cap value to 255
        if (temp_red > 255)
        {
            edgedet[0][j].rgbtRed = 255;
        }
        else
        {
            edgedet[0][j].rgbtRed = (int) round(sqrt(pow(sobel_x_red, 2) + pow(sobel_y_red, 2)));
        }
        if (temp_blue > 255)
        {
            edgedet[0][j].rgbtBlue = 255;
        }
        else
        {
            edgedet[0][j].rgbtBlue = (int) round(sqrt(pow(sobel_x_blue, 2) + pow(sobel_y_blue, 2)));
        }
        if (temp_green > 255)
        {
            edgedet[0][j].rgbtGreen = 255;
        }
        else
        {
            edgedet[0][j].rgbtGreen = (int) round(sqrt(pow(sobel_x_green, 2) + pow(sobel_y_green, 2)));
        }

        // Calculate the sobel for each color and for x and y
        sobel_x_red = 2 * image[height - 1][j + 1].rgbtRed - 2 * image[height - 1][j - 1].rgbtRed;
        sobel_x_red += image[height - 2][j + 1].rgbtRed - image[height - 2][j - 1].rgbtRed;
        sobel_x_blue = 2 * image[height - 1][j + 1].rgbtBlue - 2 * image[height - 1][j - 1].rgbtBlue;
        sobel_x_blue += image[height - 2][j + 1].rgbtBlue - image[height - 2][j - 1].rgbtBlue;
        sobel_x_green = 2 * image[height - 1][j + 1].rgbtGreen - 2 * image[height - 1][j - 1].rgbtGreen;
        sobel_x_green += image[height - 2][j + 1].rgbtGreen - image[height - 2][j - 1].rgbtGreen;
        sobel_y_red = -image[height - 2][j - 1].rgbtRed - 2 * image[height - 2][j].rgbtRed - image[height - 2][j + 1].rgbtRed;
        sobel_y_blue = -image[height - 2][j - 1].rgbtBlue - 2 * image[height - 2][j].rgbtBlue - image[height - 2][j + 1].rgbtBlue;
        sobel_y_green = -image[height - 2][j - 1].rgbtGreen - 2 * image[height - 2][j].rgbtGreen - image[height - 2][j + 1].rgbtGreen;

        // Combine x and y sobel values for each color
        temp_red = (int) round(sqrt(pow(sobel_x_red, 2) + pow(sobel_y_red, 2)));
        temp_blue = (int) round(sqrt(pow(sobel_x_blue, 2) + pow(sobel_y_blue, 2)));
        temp_green = (int) round(sqrt(pow(sobel_x_green, 2) + pow(sobel_y_green, 2)));
        // Cap value to 255
        if (temp_red > 255)
        {
            edgedet[height - 1][j].rgbtRed = 255;
        }
        else
        {
            edgedet[height - 1][j].rgbtRed = (int) round(sqrt(pow(sobel_x_red, 2) + pow(sobel_y_red, 2)));
        }
        if (temp_blue > 255)
        {
            edgedet[height - 1][j].rgbtBlue = 255;
        }
        else
        {
            edgedet[height - 1][j].rgbtBlue = (int) round(sqrt(pow(sobel_x_blue, 2) + pow(sobel_y_blue, 2)));
        }
        if (temp_green > 255)
        {
            edgedet[height - 1][j].rgbtGreen = 255;
        }
        else
        {
            edgedet[height - 1][j].rgbtGreen = (int) round(sqrt(pow(sobel_x_green, 2) + pow(sobel_y_green, 2)));
        }
    }

    // Case for edge column edge detection function
    for (int i = 1; i < height - 1; i++)
    {
        // Calculate the sobel for each color and for x and y
        sobel_x_red = image[i - 1][1].rgbtRed + 2 * image[i][1].rgbtRed + image[i + 1][1].rgbtRed;
        sobel_x_blue = image[i - 1][1].rgbtBlue + 2 * image[i][1].rgbtBlue + image[i + 1][1].rgbtBlue;
        sobel_x_green = image[i - 1][1].rgbtGreen + 2 * image[i][1].rgbtGreen + image[i + 1][1].rgbtGreen;
        sobel_y_red = 2 * image[i + 1][0].rgbtRed - 2 * image[i - 1][0].rgbtRed;
        sobel_y_red += image[i + 1][1].rgbtRed - image[i - 1][1].rgbtRed;
        sobel_y_blue = 2 * image[i + 1][0].rgbtBlue - 2 * image[i - 1][0].rgbtBlue;
        sobel_y_blue += image[i + 1][1].rgbtBlue - image[i - 1][1].rgbtBlue;
        sobel_y_green = 2 * image[i + 1][0].rgbtGreen - 2 * image[i - 1][0].rgbtGreen;
        sobel_y_green += image[i + 1][1].rgbtGreen - image[i - 1][1].rgbtGreen;

        // Combine x and y sobel values for each color
        temp_red = (int) round(sqrt(pow(sobel_x_red, 2) + pow(sobel_y_red, 2)));
        temp_blue = (int) round(sqrt(pow(sobel_x_blue, 2) + pow(sobel_y_blue, 2)));
        temp_green = (int) round(sqrt(pow(sobel_x_green, 2) + pow(sobel_y_green, 2)));
        // Cap value to 255
        if (temp_red > 255)
        {
            edgedet[i][0].rgbtRed = 255;
        }
        else
        {
            edgedet[i][0].rgbtRed = (int) round(sqrt(pow(sobel_x_red, 2) + pow(sobel_y_red, 2)));
        }
        if (temp_blue > 255)
        {
            edgedet[i][0].rgbtBlue = 255;
        }
        else
        {
            edgedet[i][0].rgbtBlue = (int) round(sqrt(pow(sobel_x_blue, 2) + pow(sobel_y_blue, 2)));
        }
        if (temp_green > 255)
        {
            edgedet[i][0].rgbtGreen = 255;
        }
        else
        {
            edgedet[i][0].rgbtGreen = (int) round(sqrt(pow(sobel_x_green, 2) + pow(sobel_y_green, 2)));
        }

        // Calculate the sobel for each color and for x and y
        sobel_x_red = image[i - 1][width - 2].rgbtRed + 2 * image[i][width - 2].rgbtRed + image[i + 1][width - 2].rgbtRed;
        sobel_x_blue = image[i - 1][width - 2].rgbtBlue + 2 * image[i][width - 2].rgbtBlue + image[i + 1][width - 2].rgbtBlue;
        sobel_x_green = image[i - 1][width - 2].rgbtGreen + 2 * image[i][width - 2].rgbtGreen + image[i + 1][width - 2].rgbtGreen;
        sobel_y_red = 2 * image[i + 1][width - 1].rgbtRed - 2 * image[i - 1][width - 1].rgbtRed;
        sobel_y_red += image[i + 1][width - 2].rgbtRed - image[i - 1][width - 2].rgbtRed;
        sobel_y_blue = 2 * image[i + 1][width - 1].rgbtBlue - 2 * image[i - 1][width - 1].rgbtBlue;
        sobel_y_blue += image[i + 1][width - 2].rgbtBlue - image[i - 1][width - 2].rgbtBlue;
        sobel_y_green = 2 * image[i + 1][width - 1].rgbtGreen - 2 * image[i - 1][width - 1].rgbtGreen;
        sobel_y_green += image[i + 1][width - 2].rgbtGreen - image[i - 1][width - 2].rgbtGreen;

        // Combine x and y sobel values for each color
        temp_red = (int) round(sqrt(pow(sobel_x_red, 2) + pow(sobel_y_red, 2)));
        temp_blue = (int) round(sqrt(pow(sobel_x_blue, 2) + pow(sobel_y_blue, 2)));
        temp_green = (int) round(sqrt(pow(sobel_x_green, 2) + pow(sobel_y_green, 2)));
        // Cap value to 255
        if (temp_red > 255)
        {
            edgedet[i][width - 1].rgbtRed = 255;
        }
        else
        {
            edgedet[i][width - 1].rgbtRed = (int) round(sqrt(pow(sobel_x_red, 2) + pow(sobel_y_red, 2)));
        }
        if (temp_blue > 255)
        {
            edgedet[i][width - 1].rgbtBlue = 255;
        }
        else
        {
            edgedet[i][width - 1].rgbtBlue = (int) round(sqrt(pow(sobel_x_blue, 2) + pow(sobel_y_blue, 2)));
        }
        if (temp_green > 255)
        {
            edgedet[i][width - 1].rgbtGreen = 255;
        }
        else
        {
            edgedet[i][width - 1].rgbtGreen = (int) round(sqrt(pow(sobel_x_green, 2) + pow(sobel_y_green, 2)));
        }
    }

    // Internal pixel edge detection function
    for (int i = 1; i < height - 1; i++)
    {
        for (int j = 1; j < width - 1; j++)
        {
            // Calculate the sobel for each color and for x and y
            sobel_x_red = -image[i - 1][j - 1].rgbtRed - 2 * image[i][j - 1].rgbtRed - image[i + 1][j - 1].rgbtRed;
            sobel_x_red += image[i - 1][j + 1].rgbtRed + 2 * image[i][j + 1].rgbtRed + image[i + 1][j + 1].rgbtRed;
            sobel_x_blue = -image[i - 1][j - 1].rgbtBlue - 2 * image[i][j - 1].rgbtBlue - image[i + 1][j - 1].rgbtBlue;
            sobel_x_blue += image[i - 1][j + 1].rgbtBlue + 2 * image[i][j + 1].rgbtBlue + image[i + 1][j + 1].rgbtBlue;
            sobel_x_green = -image[i - 1][j - 1].rgbtGreen - 2 * image[i][j - 1].rgbtGreen - image[i + 1][j - 1].rgbtGreen;
            sobel_x_green += image[i - 1][j + 1].rgbtGreen + 2 * image[i][j + 1].rgbtGreen + image[i + 1][j + 1].rgbtGreen;
            sobel_y_red = -image[i - 1][j - 1].rgbtRed - 2 * image[i - 1][j].rgbtRed - image[i - 1][j + 1].rgbtRed;
            sobel_y_red += image[i + 1][j - 1].rgbtRed + 2 * image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed;
            sobel_y_blue = -image[i - 1][j - 1].rgbtBlue - 2 * image[i - 1][j].rgbtBlue - image[i - 1][j + 1].rgbtBlue;
            sobel_y_blue += image[i + 1][j - 1].rgbtBlue + 2 * image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue;
            sobel_y_green = -image[i - 1][j - 1].rgbtGreen - 2 * image[i - 1][j].rgbtGreen - image[i - 1][j + 1].rgbtGreen;
            sobel_y_green += image[i + 1][j - 1].rgbtGreen + 2 * image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen;

            // Combine x and y sobel values for each color
            temp_red = (int) round(sqrt(pow(sobel_x_red, 2) + pow(sobel_y_red, 2)));
            temp_blue = (int) round(sqrt(pow(sobel_x_blue, 2) + pow(sobel_y_blue, 2)));
            temp_green = (int) round(sqrt(pow(sobel_x_green, 2) + pow(sobel_y_green, 2)));
            // Cap value to 255
            if (temp_red > 255)
            {
                edgedet[i][j].rgbtRed = 255;
            }
            else
            {
                edgedet[i][j].rgbtRed = (int) round(sqrt(pow(sobel_x_red, 2) + pow(sobel_y_red, 2)));
            }
            if (temp_blue > 255)
            {
                edgedet[i][j].rgbtBlue = 255;
            }
            else
            {
                edgedet[i][j].rgbtBlue = (int) round(sqrt(pow(sobel_x_blue, 2) + pow(sobel_y_blue, 2)));
            }
            if (temp_green > 255)
            {
                edgedet[i][j].rgbtGreen = 255;
            }
            else
            {
                edgedet[i][j].rgbtGreen = (int) round(sqrt(pow(sobel_x_green, 2) + pow(sobel_y_green, 2)));
            }
        }
    }

    // Case for corner cases
    // Calculate the sobel for each color and for x and y
    sobel_x_red = 2 * image[0][1].rgbtRed + image[1][1].rgbtRed;
    sobel_x_blue = 2 * image[0][1].rgbtBlue + image[1][1].rgbtBlue;
    sobel_x_green = 2 * image[0][1].rgbtGreen + image[1][1].rgbtGreen;
    sobel_y_red = 2 * image[1][0].rgbtRed + image[1][1].rgbtRed;
    sobel_y_blue = 2 * image[1][0].rgbtBlue + image[1][1].rgbtBlue;
    sobel_y_green = 2 * image[1][0].rgbtGreen + image[1][1].rgbtGreen;

    // Combine x and y sobel values for each color
    temp_red = (int) round(sqrt(pow(sobel_x_red, 2) + pow(sobel_y_red, 2)));
    temp_blue = (int) round(sqrt(pow(sobel_x_blue, 2) + pow(sobel_y_blue, 2)));
    temp_green = (int) round(sqrt(pow(sobel_x_green, 2) + pow(sobel_y_green, 2)));
    // Cap value to 255
    if (temp_red > 255)
    {
        edgedet[0][0].rgbtRed = 255;
    }
    else
    {
        edgedet[0][0].rgbtRed = (int) round(sqrt(pow(sobel_x_red, 2) + pow(sobel_y_red, 2)));
    }
    if (temp_blue > 255)
    {
        edgedet[0][0].rgbtBlue = 255;
    }
    else
    {
        edgedet[0][0].rgbtBlue = (int) round(sqrt(pow(sobel_x_blue, 2) + pow(sobel_y_blue, 2)));
    }
    if (temp_green > 255)
    {
        edgedet[0][0].rgbtGreen = 255;
    }
    else
    {
        edgedet[0][0].rgbtGreen = (int) round(sqrt(pow(sobel_x_green, 2) + pow(sobel_y_green, 2)));
    }

    // Calculate the sobel for each color and for x and y
    sobel_x_red = -2 * image[0][width - 2].rgbtRed - image[1][width - 2].rgbtRed;
    sobel_x_blue = -2 * image[0][width - 2].rgbtBlue - image[1][width - 2].rgbtBlue;
    sobel_x_green = -2 * image[0][width - 2].rgbtGreen - image[1][width - 2].rgbtGreen;
    sobel_y_red = 2 * image[1][width - 1].rgbtRed + image[1][width - 2].rgbtRed;
    sobel_y_blue = 2 * image[1][width - 1].rgbtBlue + image[1][width - 2].rgbtBlue;
    sobel_y_green = 2 * image[1][width - 1].rgbtGreen + image[1][width - 2].rgbtGreen;

    // Combine x and y sobel values for each color
    temp_red = (int) round(sqrt(pow(sobel_x_red, 2) + pow(sobel_y_red, 2)));
    temp_blue = (int) round(sqrt(pow(sobel_x_blue, 2) + pow(sobel_y_blue, 2)));
    temp_green = (int) round(sqrt(pow(sobel_x_green, 2) + pow(sobel_y_green, 2)));
    // Cap value to 255
    if (temp_red > 255)
    {
        edgedet[0][width - 1].rgbtRed = 255;
    }
    else
    {
        edgedet[0][width - 1].rgbtRed = (int) round(sqrt(pow(sobel_x_red, 2) + pow(sobel_y_red, 2)));
    }
    if (temp_blue > 255)
    {
        edgedet[0][width - 1].rgbtBlue = 255;
    }
    else
    {
        edgedet[0][width - 1].rgbtBlue = (int) round(sqrt(pow(sobel_x_blue, 2) + pow(sobel_y_blue, 2)));
    }
    if (temp_green > 255)
    {
        edgedet[0][width - 1].rgbtGreen = 255;
    }
    else
    {
        edgedet[0][width - 1].rgbtGreen = (int) round(sqrt(pow(sobel_x_green, 2) + pow(sobel_y_green, 2)));
    }

    // Calculate the sobel for each color and for x and y
    sobel_x_red = 2 * image[height - 1][1].rgbtRed + image[height - 2][1].rgbtRed;
    sobel_x_blue = 2 * image[height - 1][1].rgbtBlue + image[height - 2][1].rgbtBlue;
    sobel_x_green = 2 * image[height - 1][1].rgbtGreen + image[height - 2][1].rgbtGreen;
    sobel_y_red = -2 * image[height - 2][0].rgbtRed - image[height - 2][1].rgbtRed;
    sobel_y_blue = -2 * image[height - 2][0].rgbtBlue - image[height - 2][1].rgbtBlue;
    sobel_y_green = -2 * image[height - 2][0].rgbtGreen - image[height - 2][1].rgbtGreen;

    // Combine x and y sobel values for each color
    temp_red = (int) round(sqrt(pow(sobel_x_red, 2) + pow(sobel_y_red, 2)));
    temp_blue = (int) round(sqrt(pow(sobel_x_blue, 2) + pow(sobel_y_blue, 2)));
    temp_green = (int) round(sqrt(pow(sobel_x_green, 2) + pow(sobel_y_green, 2)));
    // Cap value to 255
    if (temp_red > 255)
    {
        edgedet[height - 1][0].rgbtRed = 255;
    }
    else
    {
        edgedet[height - 1][0].rgbtRed = (int) round(sqrt(pow(sobel_x_red, 2) + pow(sobel_y_red, 2)));
    }
    if (temp_blue > 255)
    {
        edgedet[height - 1][0].rgbtBlue = 255;
    }
    else
    {
        edgedet[height - 1][0].rgbtBlue = (int) round(sqrt(pow(sobel_x_blue, 2) + pow(sobel_y_blue, 2)));
    }
    if (temp_green > 255)
    {
        edgedet[height - 1][0].rgbtGreen = 255;
    }
    else
    {
        edgedet[height - 1][0].rgbtGreen = (int) round(sqrt(pow(sobel_x_green, 2) + pow(sobel_y_green, 2)));
    }

    // Calculate the sobel for each color and for x and y
    sobel_x_red = -2 * image[height - 1][width - 2].rgbtRed - image[height - 2][width - 2].rgbtRed;
    sobel_x_blue = -2 * image[height - 1][width - 2].rgbtBlue - image[height - 2][width - 2].rgbtBlue;
    sobel_x_green = -2 * image[height - 1][width - 2].rgbtGreen - image[height - 2][width - 2].rgbtGreen;
    sobel_y_red = -2 * image[height - 2][width - 1].rgbtRed - image[height - 2][width - 2].rgbtRed;
    sobel_y_blue = -2 * image[height - 2][width - 1].rgbtBlue - image[height - 2][width - 2].rgbtBlue;
    sobel_y_green = -2 * image[height - 2][width - 1].rgbtGreen - image[height - 2][width - 2].rgbtGreen;

    // Combine x and y sobel values for each color
    temp_red = (int) round(sqrt(pow(sobel_x_red, 2) + pow(sobel_y_red, 2)));
    temp_blue = (int) round(sqrt(pow(sobel_x_blue, 2) + pow(sobel_y_blue, 2)));
    temp_green = (int) round(sqrt(pow(sobel_x_green, 2) + pow(sobel_y_green, 2)));
    // Cap value to 255
    if (temp_red > 255)
    {
        edgedet[height - 1][width - 1].rgbtRed = 255;
    }
    else
    {
        edgedet[height - 1][width - 1].rgbtRed = (int) round(sqrt(pow(sobel_x_red, 2) + pow(sobel_y_red, 2)));
    }
    if (temp_blue > 255)
    {
        edgedet[height - 1][width - 1].rgbtBlue = 255;
    }
    else
    {
        edgedet[height - 1][width - 1].rgbtBlue = (int) round(sqrt(pow(sobel_x_blue, 2) + pow(sobel_y_blue, 2)));
    }
    if (temp_green > 255)
    {
        edgedet[height - 1][width - 1].rgbtGreen = 255;
    }
    else
    {
        edgedet[height - 1][width - 1].rgbtGreen = (int) round(sqrt(pow(sobel_x_green, 2) + pow(sobel_y_green, 2)));
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = edgedet[i][j];
        }
    }

    // Free memory for image
    free(edgedet);

    return;
}
