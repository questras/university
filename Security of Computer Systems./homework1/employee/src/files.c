#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "utils.h"
#include "files.h"
#include "definitions.h"

#define FILE_EXT_SIZE 4
#define CREDITS_DIRNAME "credits"
#define DEPOSITS_DIRNAME "deposits"

void build_filepath_from_id(unsigned long id, bool is_credit, char **output) {
    char *directory = is_credit ? "credits/" : "deposits/";
    int directory_size = strlen(directory);

    const int id_size = snprintf(NULL, 0, "%lu", id);
    const int filepath_size = directory_size + id_size + FILE_EXT_SIZE;
    char *filepath = malloc(filepath_size + 1);

    snprintf(filepath, filepath_size + 1, "%s%lu.txt", directory, id);
    *output = filepath;
}

void build_filepath(char *filename, char *directory, char **output) {
    size_t filepath_size = strlen(filename) + strlen(directory) + 1;
    char *filepath = malloc(filepath_size + 1);
    snprintf(filepath, filepath_size + 1, "%s/%s", directory, filename);

    *output = filepath;
}

bool add_new_file(struct passwd *client, char *date, double sum, double percentage, unsigned long id, bool is_credit) {
    char *filepath = NULL;
    build_filepath_from_id(id, is_credit, &filepath);

    print_info();
    printf("Built filepath: %s\n", filepath);

    print_info();
    printf("Opening file.\n");

    FILE *fp;
    fp = fopen(filepath, "w");

    if (fp == NULL) {
        print_error();
        printf("Could not create file.\n");
        return false;
    } else if (chown(filepath, client->pw_uid, client->pw_gid) != 0) {
        if (DEBUG) printf("chown error: %s\n", strerror(errno));
        print_error();
        printf("Could not set client as file owner.\n");
        fclose(fp);
        remove(filepath);
        return false;
    }

    fprintf(fp, "Name: %s\n", client->pw_gecos);
    fprintf(fp, "Number: %lu\n", id);
    fprintf(fp, "Sum: %lf\n", sum);
    fprintf(fp, "Date: %s\n", date);
    fprintf(fp, "Procent: %lf\n", percentage);

    fclose(fp);
    print_info();
    printf("File closed successfully.\n");

    return true;
}

void show_file(char *filepath) {
    FILE *fp;
    fp = fopen(filepath, "r");

    if (fp == NULL) {
        print_error();
        printf("Could not read file.\n");
        return;
    }

    char *buf = NULL;
    size_t buf_size = 0;

    while (getline(&buf, &buf_size, fp) != EOF) {
        printf("%s", buf);
    }

    fclose(fp);
}

bool is_owned_by_client(char *filepath, struct passwd *client) {
    struct stat info;
    stat(filepath, &info);
    struct passwd *file_pw = getpwuid(info.st_uid);

    return client->pw_uid == file_pw->pw_uid;
}

void show_client_files_in_dir(char *dirname, struct passwd *client) {
    DIR *d;
    struct dirent *dir;
    d = opendir(dirname);
    if (d) {
        char *filepath = NULL;
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_name[0] != '.') {
                build_filepath(dir->d_name, dirname, &filepath);
                if (!is_owned_by_client(filepath, client)) {
                    continue;
                }

                printf("filepath: %s\n", filepath);
                printf("********************************\n");
                show_file(filepath);
                printf("********************************\n");

                free(filepath);
            }
        }
        closedir(d);
    }
}

bool file_exists(char *filepath) {
    // Opening file in append mode, it will only open if it exists.
    int fd = open(filepath, O_APPEND);
    if (fd < 0) {
        return false;
    }

    close(fd);
    return true;
}

void show_client_files(struct passwd *client) {
    show_client_files_in_dir(CREDITS_DIRNAME, client);
    show_client_files_in_dir(DEPOSITS_DIRNAME, client);
}

bool update_file(unsigned long id, char *date, double sum, double percentage, bool is_credit) {
    char *filepath = NULL;
    build_filepath_from_id(id, is_credit, &filepath);

    print_info();
    printf("Opening file.\n");

    if (!file_exists(filepath)) {
        print_error();
        printf("Object with id=%lu does not exist!\n", id);
        return false;
    }

    FILE *fp;
    fp = fopen(filepath, "a");

    if (fp == NULL) {
        print_error();
        printf("Could not open file.\n");
        return false;
    }

    if (sum >= 0) {
        fprintf(fp, "Date: %s\n", date);
        fprintf(fp, "Sum: %lf\n", sum);
    }
    if (percentage >= 0) {
        fprintf(fp, "Date: %s\n", date);
        fprintf(fp, "Procent: %lf\n", percentage);
    }
    if (percentage < 0 && sum < 0) {
        // End credit/deposit.
        fprintf(fp, "Date: %s\n", date);
    }

    fclose(fp);
    print_info();
    printf("File closed successfully.\n");

    return true;
}

bool update_file_with_sum(unsigned long id, char *date, double sum, double percentage, bool is_credit) {
    return update_file(id, date, sum, percentage, is_credit);
}

bool update_file_with_percentage(unsigned long id, char *date, double percentage, bool is_credit) {
    return update_file(id, date, -1, percentage, is_credit);
}

bool end_file(unsigned long id, char *date, bool is_credit) {
    return update_file(id, date, -1, -1, is_credit);
}

void list_all_files(bool is_credit) {
    char *dirname = is_credit ? CREDITS_DIRNAME : DEPOSITS_DIRNAME;
    DIR *d;
    struct dirent *dir;
    d = opendir(dirname);

    if (d) {
        printf("%s ids:\n", dirname);
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_name[0] != '.') {
                size_t filename_size_without_extension = strlen(dir->d_name) - FILE_EXT_SIZE;
                printf("%.*s\n", (int) filename_size_without_extension, dir->d_name);
            }
        }
        closedir(d);
    } else {
        print_error();
        printf("Could not read %s.\n", dirname);
    }
}
