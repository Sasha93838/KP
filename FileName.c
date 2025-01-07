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
#define D 2
#define MAX_RESULTS 100

// ��������� ��� �������� ������� �������
struct BitMatrix_type {
    int width;              // ������ �������
    int height;             // ������ �������
    unsigned char* data;    // ��������� �� ������ ������� (�������� ��������)
};
typedef struct BitMatrix_type BitMatrix;

/* ��������� ������� */
char* generate_filename(void); // ��������� ����� �����. ���������� ��������� �� ������ (char*).
bool write_bit_matrix_to_file(const char* filename); // ������ ������� � ����. ���������� true, ���� ������ ������ �������, ����� false (bool).
int destroy_bit_matrix(BitMatrix* matrix); // ����������� �������. ���������� 0 � ������ ������ (int).
BitMatrix* read_bit_matrix(const char* filename); // ������ ������ �������. ���������� ��������� �� BitMatrix, ���� ������ ������ �������, ����� NULL (BitMatrix*).
float compare_matrices(BitMatrix* matrix1, BitMatrix* matrix2); // ��������� ������. ���������� ������� �������� ����� ��������� (float).
bool write_result_to_file(const char* time, float percentage); // ������ ����������� � ����. ���������� true, ���� ������ ������ �������, ����� false (bool).
int read_results_from_file(const char* filename, char (*times)[20], float* percentages); // ������ ����������� �� �����. ���������� ���������� ��������� ����������� (int).
int sort_results(char times[MAX_RESULTS][20], float percentages[MAX_RESULTS], int count); // ���������� �����������. ���������� 0 � ������ ������ (int).
int filter_results(void* data[], float filter_value, bool is_greater); // ���������� �����������. ���������� ���������� ��������������� ����������� (int).
int get_time_in_seconds(const char* time); // ����������� ����� � ������� "��-MM-SS" � �������. ���������� ���������� ������ � ������ ��� (int).

// ����� �������
bool write_random_matrix(const char* filename, int min_size, int max_size); // ��������� � ������ ��������� ������� � ����. ���������� true, ���� ������ ������ �������, ����� false (bool).
float compare_matrices_from_files(BitMatrix* matrix1, BitMatrix* matrix2); // ��������� ������ �� ������. ���������� ������� �������� ����� ��������� (float).

int main() {
    setlocale(LC_CTYPE, "RUS");
    int monitoring_enabled = 0;
    int mode = 0;
    int file_counter = 0;
    char times[MAX_RESULTS][20];        // ������ ��� �������� �������
    float percentages[MAX_RESULTS];     // ������ ��� �������� ��������� ���������
    int count = 0;                      // ���������� �����������
    char filtered_times[MAX_RESULTS][20];       // ������ ��� �������� ���������������� �������
    float filtered_percentages[MAX_RESULTS];    // ������ ��� �������� ��������������� ���������
    int filtered_count = 0;                     // ���������� ��������������� �����������

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

    srand((unsigned int)time(NULL)); // ������������� ���������� ��������� �����

    while (1) {
        printf("==================================================\n");
        printf("�������� ����� ������:\n");
        printf("1 - �������� ����������\n");
        printf("2 - ��������� ����������\n");
        printf("3 - ����������� ����������\n");
        printf("4 - ����������� ���������� (������ �����)\n");
        printf("5 - ����������� ���������� (������ �����)\n");
        printf("6 - ��������� ������ �� ����� �����������\n");
        printf("7 - ������� ��� ��������� ������� � �������� ��\n");
        printf("8 - �����\n");
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
        case 3:
            count = read_results_from_file("���������", times, percentages);
            if (count == 0) {
                printf("=== ���� ����������� �� ������. ===\n");
                break;
            }
            sort_results(times, percentages, count);
            printf(" ���������� (��������������� �� �������� ���������): \n");
            printf("  �����    ������� ���������\n");
            for (int i = 0; i < count; i++) {
                printf(" %s      %.2f\n", times[i], percentages[i]);
            }
            break;
        case 4:
            count = read_results_from_file("���������", times, percentages);
            if (count == 0) {
                printf("=== ���� ����������� �� ������. ===\n");
                break;
            }
            float filter_value_greater;
            printf("������� ����� ��� ���������� (���������� ������ ����� �����): ");
            scanf("%f", &filter_value_greater);

            // ���������� ������ ��� �������� � �������
            void* data_greater[] = { times, percentages, filtered_times, filtered_percentages, &count };

            filtered_count = filter_results(data_greater, filter_value_greater, true);
            printf(" ���������� ���������� (������ %.2f): \n", filter_value_greater);
            for (int i = 0; i < filtered_count; i++) {
                printf("�����: %s, ������� ���������: %.2f%%\n", filtered_times[i], filtered_percentages[i]);
            }
            break;
        case 5:
            count = read_results_from_file("���������", times, percentages);
            if (count == 0) {
                printf("=== ���� ����������� �� ������. ===\n");
                break;
            }
            float filter_value_less;
            printf("������� ����� ��� ���������� (���������� ������ ����� �����): ");
            scanf("%f", &filter_value_less);

            // ���������� ������ ��� �������� � �������
            void* data_less[] = { times, percentages, filtered_times, filtered_percentages, &count };

            filtered_count = filter_results(data_less, filter_value_less, false);
            printf(" ���������� ���������� (������ %.2f): \n", filter_value_less);
            for (int i = 0; i < filtered_count; i++) {
                printf("�����: %s, ������� ���������: %.2f%%\n", filtered_times[i], filtered_percentages[i]);
            }
            break;
        case 6:
            count = read_results_from_file("���������", times, percentages);
            if (count == 0) {
                printf("=== ���� ����������� �� ������. ===\n");
                break;
            }
            printf(" ���������� �� �����: \n");
            printf("  �����    ������� ���������\n");
            for (int i = 0; i < count; i++) {
                printf(" %s      %.2f\n", times[i], percentages[i]);
            }
            break;
        case 7: {
            // ��������� ���� ������
            char* filename1 = generate_filename();
            Sleep(1000 * D);
            char* filename2 = generate_filename();

            // �������� � ������ ���� ��������� ������
            if (write_random_matrix(filename1, 4, 8)) {
                printf("������� 1 ������� � �������� � ����: %s\n", filename1);
            }
            else {
                printf("������ �������� ������� 1.\n");
            }

            if (write_random_matrix(filename2, 4, 8)) {
                printf("������� 2 ������� � �������� � ����: %s\n", filename2);
            }
            else {
                printf("������ �������� ������� 2.\n");
            }

            // ��������� ������ �� ������
            float difference = compare_matrices_from_files(filename1, filename2);
            if (difference >= 0) {
                printf("������� �������� ����� ���������: %.2f%%\n", difference);
            }
            else {
                printf("������ ��������� ������.\n");
            }

            // ����������� ������
            free(filename1);
            free(filename2);
            break;
        }
        case 8:
            printf("=== ����� �� ���������. ===\n");
            exit(0);
        default:
            printf("=== �������� �����. ===\n");
            break;
        }

        // ���� �����������
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

                printf(" ���� ������: %s \n", current_filename);
                Sleep(1000 * D); // �������� � 2 �������

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

                        // �������� ������� ����� ��� ������ � ���� �����������
                        time_t now = time(NULL);
                        struct tm* tm = localtime(&now);
                        char time[20];
                        sprintf(time, "%02d-%02d-%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);

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
                    printf("=== ������� �����. ���������� ����������? (1 - ��, 0 - ���): ===\n");
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
 * ���������� ��� ����� �� ������ �������� ������� (� ������ ������).
 * ���������� ������ � ������ �����.
 */
char* generate_filename() {
    time_t now = time(NULL); // �������� ������� �����
    struct tm* tm = localtime(&now); // ����������� � ��������� tm

    char* filename = (char*)malloc(50);
    if (filename == NULL) {
        exit(1);
    }

    // ��������� ��� ����� � ������� "��-��-��"
    sprintf(filename, "%02d-%02d-%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);
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
            unsigned char value = rand() % 8; 
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
 * filename - ��������� �� ������ � ������ �����.
 * times - ��������� �� ������ ��� �������� �������.
 * percentages - ��������� �� ������ ��� �������� ��������� ���������.
 * ���������� ���������� ��������� �����������.
 */
int read_results_from_file(const char* filename, char (*times)[20], float* percentages) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return 0;
    }

    int count = 0;
    while (fscanf(file, "%s %f", times[count], &percentages[count]) != EOF) {
        count++;
    }

    fclose(file);
    return count;
}

/*
 * ��������� ���������� �� �������� ���������.
 * times - ������ ��� �������� �������.
 * percentages - ������ ��� �������� ��������� ���������.
 * count - ���������� �����������.
 */
int sort_results(char times[MAX_RESULTS][20], float percentages[MAX_RESULTS], int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (percentages[i] < percentages[j]) {
                float temp_percentage = percentages[i];
                percentages[i] = percentages[j];
                percentages[j] = temp_percentage;

                char temp_time[20];
                strcpy(temp_time, times[i]);
                strcpy(times[i], times[j]);
                strcpy(times[j], temp_time);
            }
        }
    }
    return 0;
}

/*
 * ��������� ���������� �� ��������� ��������.
 * data - ������ ���������� �� ������:
 *   data[0] - ������ ������� (char*),
 *   data[1] - ������ ��������� ��������� (float*),
 *   data[2] - ������ ��� �������� ���������������� ������� (char*),
 *   data[3] - ������ ��� �������� ��������������� ��������� (float*).
 * filter_value - �������� ��� ����������.
 * is_greater - ���� true, ��������� ���������� ������ filter_value, ����� ������.
 * ���������� ���������� ��������������� �����������.
 */
int filter_results(void* data[], float filter_value, bool is_greater) {
    char (*times)[20] = data[0];              // ������ �������
    float* percentages = data[1];             // ������ ��������� ���������
    char (*filtered_times)[20] = data[2];     // ������ ��� ���������������� �������
    float* filtered_percentages = data[3];    // ������ ��� ��������������� ���������
    int count = *((int*)data[4]);             // ���������� �����������

    int filtered_count = 0;
    for (int i = 0; i < count; i++) {
        if ((is_greater && percentages[i] > filter_value) ||
            (!is_greater && percentages[i] < filter_value)) {
            strcpy(filtered_times[filtered_count], times[i]);
            filtered_percentages[filtered_count] = percentages[i];
            filtered_count++;
        }
    }
    return filtered_count;
}

/*
 * ����������� ����� � ������� "��-MM-SS" � �������.
 * time - ������ � �������� � ������� "HH-MM-SS".
 * ���������� ���������� ������ � ������ ���.
 */
int get_time_in_seconds(const char* time) {
    int hours, minutes, seconds;
    sscanf(time, "%d-%d-%d", &hours, &minutes, &seconds);
    return hours * 3600 + minutes * 60 + seconds;
}

/*
 * ���������� ��������� ������� � ���������� � � ����.
 * filename - ��� ����� ��� ������.
 * min_size - ����������� ������ �������.
 * max_size - ������������ ������ �������.
 * ���������� true, ���� ������ ������ �������, ����� false.
 */
bool write_random_matrix(const char* filename, int min_size, int max_size) {
    // ��������� ��������� �������� �������
    int width = rand() % (max_size - min_size + 1) + min_size;
    int height = rand() % (max_size - min_size + 1) + min_size;

    // �������� �������
    BitMatrix* matrix = malloc(sizeof(BitMatrix));
    if (matrix == NULL) {
        return false;
    }

    matrix->width = width;
    matrix->height = height;
    matrix->data = malloc(width * height * sizeof(unsigned char));
    if (matrix->data == NULL) {
        free(matrix);
        return false;
    }

    // ���������� ������� ���������� ���������� (0 ��� 1)
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            matrix->data[i * width + j] = rand() % 2;
        }
    }

    // ������ ������� � ����
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        destroy_bit_matrix(matrix);
        return false;
    }

    fprintf(file, "%d %d\n", width, height);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            fprintf(file, "%d ", matrix->data[i * width + j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    destroy_bit_matrix(matrix);
    return true;
}

/*
 * ���������� ��� ������� �� ������, �������� �������� � ��������.
 * filename1 - ��� ����� ������ �������.
 * filename2 - ��� ����� ������ �������.
 * ���������� ������� �������� ����� ���������.
 * ���� ��������� ������, ���������� -1.
 */
float compare_matrices_from_files(const char* filename1, const char* filename2) {
    BitMatrix* matrix1 = read_bit_matrix(filename1);
    if (matrix1 == NULL) {
        return -1; 
    }
    BitMatrix* matrix2 = read_bit_matrix(filename2);
    if (matrix2 == NULL) {
        destroy_bit_matrix(matrix1); // ����������� ������ ������ �������
        return -1; 
    }

    // ���������� ������������ ������� ��� ���������
    int max_width = (matrix1->width > matrix2->width) ? matrix1->width : matrix2->width;
    int max_height = (matrix1->height > matrix2->height) ? matrix1->height : matrix2->height;
    int total_elements = max_width * max_height;
    int different_elements = 0;

    // ���������� �������� � �������� ����� �������
    for (int i = 0; i < max_height; i++) {
        for (int j = 0; j < max_width; j++) {
            // �������� �������� �� ������ ������� (��� -1, ���� ����� �� �������)
            int value1 = (i < matrix1->height && j < matrix1->width) ? matrix1->data[i * matrix1->width + j] : -1;

            // �������� �������� �� ������ ������� (��� -1, ���� ����� �� �������)
            int value2 = (i < matrix2->height && j < matrix2->width) ? matrix2->data[i * matrix2->width + j] : -1;

            if (value1 != value2) {
                different_elements++;
            }
        }
    }
    destroy_bit_matrix(matrix1);
    destroy_bit_matrix(matrix2);
    return (float)different_elements / total_elements * 100;
}