#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_PATH_LENGTH 1024
#define BUFFER_SIZE 1024

void synchronize_directories(const char *source_dir, const char *dest_dir);
int compare_files(const char *file1, const char *file2);
void copy_file(const char *source_file, const char *dest_file);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <source_directory> <destination_directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    synchronize_directories(argv[1], argv[2]);

    printf("Directories synchronized successfully.\n");

    return 0;
}

void synchronize_directories(const char *source_dir, const char *dest_dir) 
{
    DIR *source_dp = opendir(source_dir);
    if (source_dp == NULL) 
    {
        perror("Failed to open source directory");
        exit(EXIT_FAILURE);
    }

    DIR *dest_dp = opendir(dest_dir);
    if (dest_dp == NULL) 
    {
        perror("Failed to open destination directory");
        closedir(source_dp);
        exit(EXIT_FAILURE);
    }

    struct dirent *source_entry;
    while ((source_entry = readdir(source_dp)) != NULL) 
    {
        if (strcmp(source_entry->d_name, ".") == 0 || strcmp(source_entry->d_name, "..") == 0) 
	{
            continue; 
        }

        char source_path[MAX_PATH_LENGTH];
        snprintf(source_path, sizeof(source_path), "%s/%s", source_dir, source_entry->d_name);

        struct stat source_stat;
        if (stat(source_path, &source_stat) == -1) 
	{
            perror("Failed to get file information");
            closedir(source_dp);
            closedir(dest_dp);
            exit(EXIT_FAILURE);
        }

        if (S_ISREG(source_stat.st_mode)) 
	{
            char dest_path[MAX_PATH_LENGTH];
            snprintf(dest_path, sizeof(dest_path), "%s/%s", dest_dir, source_entry->d_name);

            struct stat dest_stat;
            if (stat(dest_path, &dest_stat) == -1) 
	    {
                copy_file(source_path, dest_path);
            } 
	    else 
	    {
                if (compare_files(source_path, dest_path) != 0) 
		{
                    copy_file(source_path, dest_path);
                }
            }
        }
    }

    closedir(source_dp);
    closedir(dest_dp);
}

int compare_files(const char *file1, const char *file2) 
{
    FILE *fp1 = fopen(file1, "rb");
    if (fp1 == NULL) 
    {
        perror("Failed to open file 1 for comparison");
        exit(EXIT_FAILURE);
    }

    FILE *fp2 = fopen(file2, "rb");
    if (fp2 == NULL) 
    {
        perror("Failed to open file 2 for comparison");
        fclose(fp1);
        exit(EXIT_FAILURE);
    }

    int result = 0;
    char buffer1[BUFFER_SIZE];
    char buffer2[BUFFER_SIZE];
    size_t bytes_read1, bytes_read2;

    do {
        bytes_read1 = fread(buffer1, 1, BUFFER_SIZE, fp1);
        bytes_read2 = fread(buffer2, 1, BUFFER_SIZE, fp2);

        if (bytes_read1 != bytes_read2 || memcmp(buffer1, buffer2, bytes_read1) != 0) {
            result = -1; 
            break;
        }
    } while (bytes_read1 > 0 && bytes_read2 > 0);

    fclose(fp1);
    fclose(fp2);

    return result;
}

void copy_file(const char *source_file, const char *dest_file) {
    FILE *source_fp = fopen(source_file, "rb");
    if (source_fp == NULL) {
        perror("Failed to open source file for copying");
        exit(EXIT_FAILURE);
    }

    FILE *dest_fp = fopen(dest_file, "wb");
    if (dest_fp == NULL) {
        perror("Failed to create destination file");
        fclose(source_fp);
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, source_fp)) > 0) {
        fwrite(buffer, 1, bytes_read, dest_fp);
    }

    fclose(source_fp);
    fclose(dest_fp);
}
