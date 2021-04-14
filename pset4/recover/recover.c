#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Check for invalid usage
    if (argc != 2)
    {
        printf("Usage: %s image\n", argv[0]);
        return 1;
    }

    // Open File given on command line
    FILE *inptr = fopen(argv[1], "r");
    if (!inptr)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }

    // Create output filename
    char *output_filename = malloc(7 * sizeof(char));
    int output_file_number = 0;
    sprintf(output_filename, "%03d.jpg", output_file_number);
    // Open first output file
    FILE *outptr = fopen(output_filename, "w");
    if (!outptr)
    {
        fclose(inptr);
        fprintf(stderr, "Could not open %s.\n", output_filename);
        return 3;
    }

    // Create storage for each 512B chunk
    BYTE bytes[512];
    // While there are still 512B to get from the input file
    while (fread(bytes, sizeof(BYTE), 512, inptr) != 0)
    {
        // Check first three bytes for new photo
        if (bytes[0] == 0xff && bytes[1] == 0xd8 && bytes[2] == 0xff)
        {
            // Create new filename
            sprintf(output_filename, "%03d.jpg", output_file_number);
            if (strcmp(output_filename, "000.jpg") != 0)
            {
                // If this is not the first file, then close the previous file
                fclose(outptr);
                // Open output file
                outptr = fopen(output_filename, "w");
                if (outptr == NULL)
                {
                    fclose(inptr);
                    fprintf(stderr, "Could not create %s.\n", output_filename);
                    return 3;
                }
            }
            // Increment output file number
            output_file_number++;
        }
        // If the first photo is found, write to output file
        if (output_file_number > 0)
        {
            fwrite(bytes, sizeof(BYTE), 512, outptr);
        }
    }

    // Close inptr & outptr
    fclose(inptr);
    fclose(outptr);

    return 0;
}
