#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 512
#define FILENAME_LENGTH 8  // 3 digits + .jpg + null terminator

int main(int argc, char *argv[])
{
    // Accept a single command-line argument
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    // Open the memory card
    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        printf("File cannot be opened.");
        return 1;
    }

    FILE *img = NULL;

    char filename[FILENAME_LENGTH];
    int n_img = 0;
    uint8_t buffer[BLOCK_SIZE];

    // While there's still data left to read from the memory card
    while (fread(buffer, BLOCK_SIZE, 1, card))
    {
        // Read four bytes at one time to identify JPEG header, then create and open the new JPEG file
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // If there is no first JPEG created then close the previous file
            if (n_img != 0)
            {
                fclose(img);
            }

            sprintf(filename, "%03i.jpg", n_img);
            img = fopen(filename, "w");
            n_img++;
        }
        
        // If the JPEG file is already found, keep writing on it
        if (img != NULL)
        {
            fwrite(buffer, BLOCK_SIZE, 1, img);
        }
    }

    // Close any remaining files
    fclose(img);
    fclose(card);
}
