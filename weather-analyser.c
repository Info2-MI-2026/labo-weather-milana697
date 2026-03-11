/**
 * Weather Analyzer.
 */
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MONTHS 12        // Months in a year
#define MAX_ENTRIES 512  // Maximum years in a file

typedef struct
{
    double temperature;
    double precipitations;
} WMonth;

typedef struct
{
    int year;
    WMonth months[MONTHS];
    double temperature;
    double precipitations;
} WYear;

typedef struct
{
    WYear years[MAX_ENTRIES];
    int start;
} WData;

typedef struct
{
    char *in_filename;
    char *out_filename;
    bool binary_output;
} Options;

void version(FILE *fp)
{
    fprintf(fp,
            "Version 0.0.1 "
            "Copyright(c) HEIG-VD\n");
}

void help(FILE *fp)
{
    fprintf(fp,
            "USAGE: ./weather-analyser [options] [filename]\n\n"
            "This program processes weather data issues from the internet. \n"
            "It reads from [filename] or if missing, \n"
            "directly from STDIN.\n\n"
            "The output format is CSV compliant.\n\n"
            "OPTIONS:\n\n"
            "    --version      Shows the version\n"
            "    --help         Shows this help dialog\n\n"
            "    --binary, -b   Output in binary mode, not CSV\n\n"
            "    -o<filename>   Write output on <filename>\n\n");
}

int collect_data(WData *data, FILE *fp) {}

void process_data(WData *data) {}

void fprint_csv(FILE *fp, WData *data) {}

void fprint_binary(FILE *fp, WData *data) {}

void process_arg(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--version") == 0)
        {
            version(stdout);
        }
    }
}

int main(int argc, char *argv[])
{
    process_arg(argc, argv);
}
