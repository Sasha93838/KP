/*
  �����: ������� ���������, ���-241
  ��������� ��� ����������� ��������� � ������� ��������.
  ��������:
  - ��������� � ��������� ������� ������.
  - ������ ����������� ��������� � ����.
  - ���������� � ���������� �����������.
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <locale.h>
#include <windows.h>
#define MATRIX_HEIGHT 10
#define MATRIX_WIDTH 10
#define N 90
#define K 400
#define D 1

 // ��������� ��� �������� ������� �������
struct BitMatrix_type {
    int width;              // ������ �������
    int height;             // ������ �������
    unsigned char* data;    // ��������� �� ������ ������� (�������� ��������)
};
typedef struct BitMatrix_type BitMatrix;

// ��������� ��� �������� ����������� �����������
struct ResultEntry_type {
    char time[20];          // ����� ���������� ����������� (������ ������� "��:MM")
    float percentage;       // ������� ��������� ����� ���������
};
typedef struct ResultEntry_type ResultEntry;

/* ��������� ������� */
char* generate_filename(void); // ��������� ����� �����
bool write_bit_matrix_to_file(const char* filename); // ������ ������� � ����
int destroy_bit_matrix(BitMatrix* matrix); // ����������� �������
BitMatrix* read_bit_matrix(const char* filename); // ������ ������ �������
float compare_matrices(BitMatrix* matrix1, BitMatrix* matrix2); // ��������� ������
bool write_result_to_file(const char* time, float percentage); // ������ ����������� � ����
ResultEntry* read_results_from_file(int* count); // ������ ����������� �� �����
int sort_results(ResultEntry* results, int count); // ���������� �����������
ResultEntry* filter_results(ResultEntry* results, int count, float filter_value, bool is_greater, int* filtered_count); // ���������� �����������
int get_time_in_seconds(const char* time); // ��������� �������

/* ���������� ���������� ��� �������� �������� ������� */
int current_hour = 12;
int current_minute = 0;

/* �������� ������� ��������� */
int main() {
    setlocale(LC_CTYPE, "RUS");
    int monitoring_enabled = 0;
    int mode = 0;
    int file_counter = 0;

    printf("=================================================================================\n");
    printf("����� ���������� � ��������� ����������� ��������� � ������� ��������! \n");
    printf("=================================================================================\n");
    printf("��� ��������� ���������:\n");
    printf(" - ������������� ������������ � ���������� ������� �������.\n");
    printf(" - ����������� ��������� � �������� � ���������� ���������� � ����.\n");
    printf(" - ����������� ���������� ���������.\n");
    printf(" - ����������� ���������� �� �������� (������ ��� ������).\n");
    printf(" - �������� �������������� ��� ������������ ����������.\n");
    printf(" - �� ��������� ������� ���������� ����� �������������.\n");
    printf("=== ��������� �������������! ===\n\n");

    srand((unsigned int)time(NULL)); 

    while (1) {
        printf("==================================================\n");
        printf("�������� ����� ������:\n");
        printf("1 - �������� ����������\n");
        printf("2 - ��������� ����������\n");
        printf("3 - ����������� ����������\n");
        printf("4 - ����������� ���������� (������ �����)\n");
        printf("5 - ����������� ���������� (������ �����)\n");
        printf("6 - ��������� ������ �� ����� �����������\n");
        printf("7 - �����\n");
        printf("��� �����: ");
        scanf("%d", &mode);
        printf("==================================================\n");

        switch (mode) {
        case 1:
            monitoring_enabled = 1;
            printf("--- ���������� �������. ---\n");
            break;
        case 2:
            monitoring_enabled = 0;
            printf("--- ���������� ��������. ---\n");
            break;
        case 3: {
            int count = 0;
            ResultEntry* results = read_results_from_file(&count);
            if (results == NULL) {
                printf("=== ���� ����������� �� ������. ===\n");
                break;
            }

            sort_results(results, count);
            printf(" ���������� (��������������� �� �������� ���������): \n");
            printf("  �����    ������� ���������\n");
            for (int i = 0; i < count; i++) {
                printf(" %s      %.2f\n", results[i].time, results[i].percentage);
            }

            free(results);
            break;
        }
        case 4: {
            int count = 0;
            ResultEntry* results = read_results_from_file(&count);
            if (results == NULL) {
                printf("=== ���� ����������� �� ������. ===\n");
                break;
            }

            float filter_value;
            printf("************************************************\n");
            printf("������� ����� ��� ���������� (���������� ������ ����� �����): ");
            scanf("%f", &filter_value);
            printf("************************************************\n");

            int filtered_count = 0;
            ResultEntry* filtered_results = filter_results(results, count, filter_value, true, &filtered_count);
            printf(" ���������� ���������� (������ %.2f): \n", filter_value);
            for (int i = 0; i < filtered_count; i++) {
                printf("�����: %s, ������� ���������: %.2f%%\n", filtered_results[i].time, filtered_results[i].percentage);
            }
            free(filtered_results);
            free(results);
            break;
        }
        case 5: {
            int count = 0;
            ResultEntry* results = read_results_from_file(&count);
            if (results == NULL) {
                printf("=== ���� ����������� �� ������. ===\n");
                break;
            }

            float filter_value;
            printf("************************************************\n");
            printf("������� ����� ��� ���������� (���������� ������ ����� �����): ");
            scanf("%f", &filter_value);
            printf("************************************************\n");

            int filtered_count = 0;
            ResultEntry* filtered_results = filter_results(results, count, filter_value, false, &filtered_count);
            printf(" ���������� ���������� (������ %.2f): \n", filter_value);
            for (int i = 0; i < filtered_count; i++) {
                printf("�����: %s, ������� ���������: %.2f%%\n", filtered_results[i].time, filtered_results[i].percentage);
            }
            free(filtered_results);
            free(results);
            break;
        }
        case 6: {
            int count = 0;
            ResultEntry* results = read_results_from_file(&count);
            if (results == NULL) {
                printf("=== ���� ����������� �� ������. ===\n");
                break;
            }

            printf(" ���������� �� �����: \n");
            printf("  �����    ������� ���������\n");
            for (int i = 0; i < count; i++) {
                printf(" %s      %.2f\n", results[i].time, results[i].percentage);
            }

            free(results);
            break;
        }
        case 7:
            printf("=== ����� �� ���������. ===\n");
            exit(0);
        default:
            printf("=== �������� �����. ===\n");
            continue;
        }

        while (monitoring_enabled) {
            int loop_count = 10;
            char* previous_filename = NULL;

            for (int i = 0; i < loop_count; i++) {
                char* current_filename = generate_filename();
                if (!write_bit_matrix_to_file(current_filename)) {
                    printf("=== ������ ������ ������� � ����. ===\n");
                    free(current_filename);
                    continue;
                }

                // ����� ��������� � �������� �����
                printf(" ���� ������: %s \n", current_filename);

                if (previous_filename != NULL) {
                    BitMatrix* previous_matrix = read_bit_matrix(previous_filename);
                    BitMatrix* current_matrix = read_bit_matrix(current_filename);

                    if (previous_matrix == NULL || current_matrix == NULL) {
                        printf("=== ������ ������ ������. ===\n");
                        free(previous_filename);
                        free(current_filename);
                        continue;
                    }

                    float difference = compare_matrices(previous_matrix, current_matrix);

                    int previous_time = get_time_in_seconds(previous_filename);
                    int current_time = get_time_in_seconds(current_filename);
                    int time_difference = current_time - previous_time;

                    if (time_difference <= K) {
                        if (difference > N) {
                            printf("=== �������! ����������� ���������� ����� ��� �� %d%%! ===\n", N);
                        }

                        char time[20];
                        sprintf(time, "%02d-%02d", current_hour, current_minute);
                        if (!write_result_to_file(time, difference)) {
                            printf("=== ������ ������ ���������� � ����. ===\n");
                        }
                    }

                    destroy_bit_matrix(previous_matrix);
                    destroy_bit_matrix(current_matrix);
                    free(previous_filename);
                }

                previous_filename = current_filename;
                file_counter++;

                if (file_counter % 4 == 0) {
                    printf("=== ������� �����. ���������� ����������? (1 - ��, 0 - ���): ===\n", file_counter);
                    int continue_monitoring;
                    scanf("%d", &continue_monitoring);
                    if (continue_monitoring == 0) {
                        monitoring_enabled = 0;
                        printf("--- ���������� ����������. ---\n");
                        break;
                    }
                }
            }

            free(previous_filename);
        }
    }

    return 0;
}

/*
 * ���������� ��� ����� �� ������ �������� �������.
 * ���������� ������ � ������ �����.
 * �������� ������: ����������� ������� ����� �� 5 �����.
 */
char* generate_filename() {
    current_minute += 5;
    if (current_minute >= 60) {
        current_minute -= 60;
        current_hour++;
        if (current_hour >= 24) {
            current_hour = 0;
        }
    }

    char* filename = (char*)malloc(50);
    if (filename == NULL) {
        exit(1);
    }

    sprintf(filename, "%02d-%02d", current_hour, current_minute);
    return filename;
}

/*
 * ���������� �������� ������� � ����.
 * filename - ��� ����� ��� ������.
 * ���������� true, ���� ������ ������ �������, ����� false.
 */
bool write_bit_matrix_to_file(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        return false;
    }

    fprintf(file, "%d %d\n", MATRIX_WIDTH, MATRIX_HEIGHT);

    // ��������� ��������� �������
    for (int i = 0; i < MATRIX_HEIGHT; i++) {
        for (int j = 0; j < MATRIX_WIDTH; j++) {
            unsigned char value = rand() % 8; // ��������� 0 ��� 1
            fprintf(file, "%d ", value);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    return true;
}

/*
 * ����������� ������, ���������� ��� �������.
 * matrix - ��������� �� ������� ��� ������������.
 */
int destroy_bit_matrix(BitMatrix* matrix) {
    if (matrix != NULL && matrix->data != NULL) {
        free(matrix->data);
        matrix->data = NULL;
    }
    free(matrix);
    return 0;
}

/*
 * ������ �������� ������� �� �����.
 * filename - ��� ����� ��� ������.
 * ���������� ��������� �� �������, ���� ������ ������ �������, ����� NULL.
 */
BitMatrix* read_bit_matrix(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        return NULL;
    }

    int width, height;
    if (fscanf(fp, "%d %d", &width, &height) != 2) {
        fclose(fp);
        return NULL;
    }

    BitMatrix* matrix = malloc(sizeof(BitMatrix));
    if (matrix == NULL) {
        fclose(fp);
        return NULL;
    }

    matrix->width = width;
    matrix->height = height;
    matrix->data = malloc(width * height * sizeof(unsigned char));
    if (matrix->data == NULL) {
        free(matrix);
        fclose(fp);
        return NULL;
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int value;
            if (fscanf(fp, "%d", &value) != 1) {
                destroy_bit_matrix(matrix);
                fclose(fp);
                return NULL;
            }
            matrix->data[i * width + j] = (unsigned char)value;
        }
    }

    fclose(fp);
    return matrix;
}

/*
 * ���������� ��� �������� �������.
 * matrix1 - ������ ������� ��� ���������.
 * matrix2 - ������ ������� ��� ���������.
 * ���������� ������� �������� ����� ���������.
 * ���� ������� ������ �� ���������, ���������� -1.
 */
float compare_matrices(BitMatrix* matrix1, BitMatrix* matrix2) {
    if (matrix1->width != matrix2->width || matrix1->height != matrix2->height) {
        return -1;
    }

    int total_pixels = matrix1->width * matrix1->height;
    int different_pixels = 0;

    for (int i = 0; i < total_pixels; i++) {
        if (matrix1->data[i] != matrix2->data[i]) {
            different_pixels++;
        }
    }

    return (float)different_pixels / total_pixels * 100;
}

/*
 * ���������� ��������� ��������� � ����.
 * time - �����, ����� ��������� ���������.
 * percentage - ������� �������� ����� ���������.
 * ���������� true, ���� ������ ������ �������, ����� false.
 */
bool write_result_to_file(const char* time, float percentage) {
    FILE* file = fopen("���������", "a");
    if (file == NULL) {
        return false;
    }

    fprintf(file, "%s %.2f\n", time, percentage);
    fclose(file);
    return true;
}

/*
 * ������ ���������� �� �����.
 * count - ��������� �� ����������, � ������� ����� �������� ���������� �����������.
 * ���������� ������ �������� ResultEntry � ������������, ���� ������ ������ �������, ����� NULL.
 */
ResultEntry* read_results_from_file(int* count) {
    FILE* file = fopen("���������", "r");
    if (file == NULL) {
        return NULL;
    }

    ResultEntry* results = malloc(100 * sizeof(ResultEntry));
    if (results == NULL) {
        fclose(file);
        return NULL;
    }

    *count = 0;
    while (fscanf(file, "%s %f", results[*count].time, &results[*count].percentage) != EOF) {
        (*count)++;
    }

    fclose(file);
    return results;
}

/*
 * ��������� ���������� �� �������� ���������.
 * results - ������ ����������� ��� ����������.
 * count - ���������� ��������� � �������.
 */
int sort_results(ResultEntry* results, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (results[i].percentage < results[j].percentage) {
                ResultEntry temp = results[i];
                results[i] = results[j];
                results[j] = temp;
            }
        }
    }
    return 0;
}

/*
 * ��������� ���������� �� ��������� ��������.
 * results - ������ ����������� ��� ����������.
 * count - ���������� ��������� � �������.
 * filter_value - �������� ��� ����������.
 * is_greater - ���� true, ��������� ���������� ������ filter_value, ����� ������.
 * filtered_count - ��������� �� ����������, � ������� ����� �������� ���������� ��������������� �����������.
 * ���������� ������ ��������������� �����������.
 */
ResultEntry* filter_results(ResultEntry* results, int count, float filter_value, bool is_greater, int* filtered_count) {
    ResultEntry* filtered_results = malloc(count * sizeof(ResultEntry));
    if (filtered_results == NULL) {
        return NULL;
    }

    *filtered_count = 0;
    for (int i = 0; i < count; i++) {
        if ((is_greater && results[i].percentage > filter_value) ||
            (!is_greater && results[i].percentage < filter_value)) {
            filtered_results[*filtered_count] = results[i];
            (*filtered_count)++;
        }
    }

    return filtered_results;
}

/*
 * ����������� ����� � ������� "��-MM" � �������.
 * time - ������ � �������� � ������� "HH-MM".
 * ���������� ���������� ������ � ������ ���.
 */
int get_time_in_seconds(const char* time) {
    int hours, minutes;
    sscanf(time, "%d-%d", &hours, &minutes);
    return hours * 3600 + minutes * 60;
}