#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MONTHS 12        // Months in a year
#define MAX_ENTRIES 512  // Maximum years in a file
#define LINE_SIZE 256
typedef struct
{
    double temperature; //releve brut par mois
    double precipitations; //releve brut par moi 
} WMonth;

typedef struct
{
    int year;
    WMonth months[MONTHS];
    double temperature; //moyenne annuel 
    double precipitations; //total annuel 
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

//sert a stocker les données brutes du fichier (annee, mois, temp du mois, precip du mois)
int collect_data(WData *data, FILE *fp) 
{
    char line[LINE_SIZE];

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        int year = 0;
        int month = 0;
        double temperature = 0.0;
        double precipitations = 0.0;

        //lire ligne de données meteo et ignorer en tete
        if (sscanf(line, "%d %d %lf %lf", &year, &month, &temperature, &precipitations) != 4)
            continue;

        //Check valeurs recues
        if (month < 1 || month > 12)
            return 1;
        
        //stocker valeur brut dans data et Wmonth
        //Start sert à convertir une année réelle en indice de tableau
        if (data->start == 0)
            data->start = year;
        
        int index_year = year - data->start;
        if (index_year < 0 || index_year >= MAX_ENTRIES)
            return 1;

        data->years[index_year].year = year;
        data->years[index_year].months[month - 1].temperature = temperature;
        data->years[index_year].months[month - 1].precipitations = precipitations;
    }

    return 0;
}

//a partir de WData et WMonth je remplis WYear (prep tot par annee, temp moy par annne)
void process_data(WData *data)
{
    // parcourir toutes les années possibles
    for (int i = 0; i < MAX_ENTRIES; i++)
    {
        // si cette année n'est pas utilisée, passer à la suivante 
        if (data->years[i].year == 0) 
            continue;
        
        // créer somme_temp et somme_precip
        double somme_temp = 0.0;
        double somme_precip = 0.0;

        // parcourir les 12 mois
        for (int j = 0; j < MONTHS; j++)
        {
            // ajouter température du mois
            somme_temp += data->years[i].months[j].temperature;

            // ajouter précipitations du mois
            somme_precip += data->years[i].months[j].precipitations;
        }

        // stocker la moyenne annuelle de temp
        data->years[i].temperature = somme_temp / MONTHS;

        // stocker le total annuel de precip
        data->years[i].precipitations = somme_precip;

    }
}

void fprint_csv(FILE *fp, WData *data)
{
    fprintf(fp, "year;temperature;precipitations\n");

    for (int i = 0; i < MAX_ENTRIES; i++)
    {
        if (data->years[i].year == 0)
            continue;

        fprintf(fp, "%d;%g;%.1f\n",
                data->years[i].year,
                data->years[i].temperature,
                data->years[i].precipitations);
    }
}

void fprint_binary(FILE *fp, WData *data)
{
    //binaire donc fwrite
    fwrite("WEATHER", sizeof(char), 7, fp);

    for (int i = 0; i < MAX_ENTRIES; i++)
    {
        //sauter les annes vides
        if (data->years[i].year == 0)
            continue;

        //prendre les valeurs annuelles et non mensuelles
        int year = data->years[i].year;
        //pas oublier () car multiplier avant et ensuite cast en int ccar 4 bytes
        int precip = (int)(data->years[i].precipitations * 100.0);
        int temp = (int)(data->years[i].temperature * 10.0);

        fwrite(&year, sizeof(int), 1, fp);
        fwrite(&precip, sizeof(int), 1, fp);
        fwrite(&temp, sizeof(int), 1, fp);
    }
}

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
    //printf("debut programme\n"); utile pour tester

    bool binary_output = false;
    char *input_filename = NULL;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--help") == 0)
        {
            help(stdout);
            return 0;
        }
        else if (strcmp(argv[i], "--version") == 0)
        {
            version(stdout);
            return 0;
        }
        else if (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--binary") == 0)
        {
            binary_output = true;
        }
        else
        {
            input_filename = argv[i];
        }
    }

    if (input_filename == NULL)
        return 1;

    FILE* fp = fopen(input_filename, "r");
    if (fp == NULL)
        return 1;

    //printf("fichier ouvert\n"); debug

    //collect data
    WData d = {0};
    if (collect_data(&d, fp) != 0)
    {
        fclose(fp);
        return 1;
    }
    fclose(fp);

    //printf("start %d\n", d.start);
    //printf("year %d\n", d.years[0].year);
    //printf("precip %f\n", d.years[0].months[0].precipitations);
    //printf("temp %f\n", d.years[0].months[0].temperature);
    
    //process data
    process_data(&d);
    //printf("annee %d\n", d.years[0].year);
    //printf("temp annuelle %.3f\n", d.years[0].temperature);
    //printf("prec annuelle %.1f\n", d.years[0].precipitations);

    if (binary_output)
        fprint_binary(stdout, &d);
    else
        fprint_csv(stdout, &d);

    return 0;
}