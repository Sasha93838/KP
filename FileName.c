/*
  Автор: Батищев Александр, бИЦ-241
  Программа для мониторинга изменений в битовых матрицах.
  Описание:
  - Генерация и сравнение битовых матриц.
  - Запись результатов сравнения в файл.
  - Сортировка и фильтрация результатов.
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

 // Структура для хранения битовой матрицы
struct BitMatrix_type {
    int width;              // Ширина матрицы
    int height;             // Высота матрицы
    unsigned char* data;    // Указатель на данные матрицы (бинарные значения)
};
typedef struct BitMatrix_type BitMatrix;

/* Прототипы функций */
char* generate_filename(void); // Генерация имени файла. Возвращает указатель на строку (char*).
bool write_bit_matrix_to_file(const char* filename); // Запись матрицы в файл. Возвращает true, если запись прошла успешно, иначе false (bool).
int destroy_bit_matrix(BitMatrix* matrix); // Уничтожение матрицы. Возвращает 0 в случае успеха (int).
BitMatrix* read_bit_matrix(const char* filename); // Чтение данных матрицы. Возвращает указатель на BitMatrix, если чтение прошло успешно, иначе NULL (BitMatrix*).
float compare_matrices(BitMatrix* matrix1, BitMatrix* matrix2); // Сравнение матриц. Возвращает процент различий между матрицами (float).
bool write_result_to_file(const char* time, float percentage); // Запись результатов в файл. Возвращает true, если запись прошла успешно, иначе false (bool).
int read_results_from_file(const char* filename, char (*times)[20], float* percentages); // Чтение результатов из файла. Возвращает количество считанных результатов (int).
int sort_results(char times[MAX_RESULTS][20], float percentages[MAX_RESULTS], int count); // Сортировка результатов. Возвращает 0 в случае успеха (int).
int filter_results(void* data[], float filter_value, bool is_greater); // Фильтрация результатов. Возвращает количество отфильтрованных результатов (int).
int get_time_in_seconds(const char* time); // Преобразует время в формате "ЧЧ-MM-SS" в секунды. Возвращает количество секунд с начала дня (int).

/* Основная функция программы */
int main() {
    setlocale(LC_CTYPE, "RUS");
    int monitoring_enabled = 0;
    int mode = 0;
    int file_counter = 0;
    char times[MAX_RESULTS][20];        // Массив для хранения времени
    float percentages[MAX_RESULTS];     // Массив для хранения процентов изменений
    int count = 0;                      // Количество результатов
    char filtered_times[MAX_RESULTS][20];       // Массив для хранения отфильтрованного времени
    float filtered_percentages[MAX_RESULTS];    // Массив для хранения отфильтрованных процентов
    int filtered_count = 0;                     // Количество отфильтрованных результатов

    printf("=================================================================================\n");
    printf("Добро пожаловать в программу мониторинга изменений в битовых матрицах! \n");
    printf("=================================================================================\n");
    printf("Эта программа позволяет:\n");
    printf(" - Автоматически генерировать и сравнивать битовые матрицы.\n");
    printf(" - Фиксировать изменения в матрицах и записывать результаты в файл.\n");
    printf(" - Сортировать результаты сравнения.\n");
    printf(" - Фильтровать результаты по значению (больше или меньше).\n");
    printf(" - Выводить предупреждения при значительных изменениях.\n");
    printf(" - Не забывайте очищать результаты после использования.\n");
    printf("=== Приятного использования! ===\n\n");

    srand((unsigned int)time(NULL)); // Инициализация генератора случайных чисел

    while (1) {
        printf("==================================================\n");
        printf("Выберите режим работы:\n");
        printf("1 - Включить мониторинг\n");
        printf("2 - Выключить мониторинг\n");
        printf("3 - Сортировать результаты\n");
        printf("4 - Фильтровать результаты (больше числа)\n");
        printf("5 - Фильтровать результаты (меньше числа)\n");
        printf("6 - Прочитать данные из файла результатов\n");
        printf("7 - Выход\n");
        printf("Ваш выбор: ");
        scanf("%d", &mode);
        printf("==================================================\n");

        switch (mode) {
        case 1:
            monitoring_enabled = 1;
            printf("--- Мониторинг включен. ---\n");
            break;
        case 2:
            monitoring_enabled = 0;
            printf("--- Мониторинг выключен. ---\n");
            break;
        case 3:
            count = read_results_from_file("результат", times, percentages);
            if (count == 0) {
                printf("=== Файл результатов не найден. ===\n");
                break;
            }
            sort_results(times, percentages, count);
            printf(" Результаты (отсортированные по проценту изменения): \n");
            printf("  Время    Процент изменения\n");
            for (int i = 0; i < count; i++) {
                printf(" %s      %.2f\n", times[i], percentages[i]);
            }
            break;
        case 4:
            count = read_results_from_file("результат", times, percentages);
            if (count == 0) {
                printf("=== Файл результатов не найден. ===\n");
                break;
            }
            float filter_value_greater;
            printf("Введите число для фильтрации (результаты больше этого числа): ");
            scanf("%f", &filter_value_greater);

            // Подготовка данных для передачи в функцию
            void* data_greater[] = { times, percentages, filtered_times, filtered_percentages, &count };

            filtered_count = filter_results(data_greater, filter_value_greater, true);
            printf(" Результаты фильтрации (больше %.2f): \n", filter_value_greater);
            for (int i = 0; i < filtered_count; i++) {
                printf("Время: %s, Процент изменения: %.2f%%\n", filtered_times[i], filtered_percentages[i]);
            }
            break;
        case 5:
            count = read_results_from_file("результат", times, percentages);
            if (count == 0) {
                printf("=== Файл результатов не найден. ===\n");
                break;
            }
            float filter_value_less;
            printf("Введите число для фильтрации (результаты меньше этого числа): ");
            scanf("%f", &filter_value_less);

            // Подготовка данных для передачи в функцию
            void* data_less[] = { times, percentages, filtered_times, filtered_percentages, &count };

            filtered_count = filter_results(data_less, filter_value_less, false);
            printf(" Результаты фильтрации (меньше %.2f): \n", filter_value_less);
            for (int i = 0; i < filtered_count; i++) {
                printf("Время: %s, Процент изменения: %.2f%%\n", filtered_times[i], filtered_percentages[i]);
            }
            break;
        case 6:
            count = read_results_from_file("результат", times, percentages);
            if (count == 0) {
                printf("=== Файл результатов не найден. ===\n");
                break;
            }
            printf(" Результаты из файла: \n");
            printf("  Время    Процент изменения\n");
            for (int i = 0; i < count; i++) {
                printf(" %s      %.2f\n", times[i], percentages[i]);
            }
            break;
        case 7:
            printf("=== Выход из программы. ===\n");
            exit(0);
        default:
            printf("=== Неверный выбор. ===\n");
            continue;
        }

        while (monitoring_enabled) {
            int loop_count = 10;
            char* previous_filename = NULL;

            for (int i = 0; i < loop_count; i++) {
                char* current_filename = generate_filename();
                if (!write_bit_matrix_to_file(current_filename)) {
                    printf("=== Ошибка записи матрицы в файл. ===\n");
                    free(current_filename);
                    continue;
                }

                printf(" Файл создан: %s \n", current_filename);
                Sleep(1000 * D); // Задержка в 2 секунды

                if (previous_filename != NULL) {
                    BitMatrix* previous_matrix = read_bit_matrix(previous_filename);
                    BitMatrix* current_matrix = read_bit_matrix(current_filename);

                    if (previous_matrix == NULL || current_matrix == NULL) {
                        printf("=== Ошибка чтения матриц. ===\n");
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
                            printf("=== ТРЕВОГА! Изображение изменилось более чем на %d%%! ===\n", N);
                        }

                        // Получаем текущее время для записи в файл результатов
                        time_t now = time(NULL);
                        struct tm* tm = localtime(&now);
                        char time[20];
                        sprintf(time, "%02d-%02d-%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);

                        if (!write_result_to_file(time, difference)) {
                            printf("=== Ошибка записи результата в файл. ===\n");
                        }
                    }

                    destroy_bit_matrix(previous_matrix);
                    destroy_bit_matrix(current_matrix);
                    free(previous_filename);
                }

                previous_filename = current_filename;
                file_counter++;

                if (file_counter % 4 == 0) {
                    printf("=== Созданы файлы. Продолжить мониторинг? (1 - Да, 0 - Нет): ===\n", file_counter);
                    int continue_monitoring;
                    scanf("%d", &continue_monitoring);
                    if (continue_monitoring == 0) {
                        monitoring_enabled = 0;
                        printf("--- Мониторинг остановлен. ---\n");
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
 * Генерирует имя файла на основе текущего времени (с учетом секунд).
 * Возвращает строку с именем файла.
 */
char* generate_filename() {
    time_t now = time(NULL); // Получаем текущее время
    struct tm* tm = localtime(&now); // Преобразуем в структуру tm

    char* filename = (char*)malloc(50);
    if (filename == NULL) {
        exit(1);
    }

    // Формируем имя файла в формате "ЧЧ-ММ-СС"
    sprintf(filename, "%02d-%02d-%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);
    return filename;
}

/*
 * Записывает бинарную матрицу в файл.
 * filename - имя файла для записи.
 * Возвращает true, если запись прошла успешно, иначе false.
 */
bool write_bit_matrix_to_file(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        return false;
    }

    fprintf(file, "%d %d\n", MATRIX_WIDTH, MATRIX_HEIGHT);

    // Генерация случайной матрицы
    for (int i = 0; i < MATRIX_HEIGHT; i++) {
        for (int j = 0; j < MATRIX_WIDTH; j++) {
            unsigned char value = rand() % 8; // Генерация 0 или 1
            fprintf(file, "%d ", value);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    return true;
}

/*
 * Освобождает память, выделенную для матрицы.
 * matrix - указатель на матрицу для освобождения.
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
 * Читает бинарную матрицу из файла.
 * filename - имя файла для чтения.
 * Возвращает указатель на матрицу, если чтение прошло успешно, иначе NULL.
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
 * Сравнивает две бинарные матрицы.
 * matrix1 - первая матрица для сравнения.
 * matrix2 - вторая матрица для сравнения.
 * Возвращает процент различий между матрицами.
 * Если размеры матриц не совпадают, возвращает -1.
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
 * Записывает результат сравнения в файл.
 * time - время, когда произошло сравнение.
 * percentage - процент различий между матрицами.
 * Возвращает true, если запись прошла успешно, иначе false.
 */
bool write_result_to_file(const char* time, float percentage) {
    FILE* file = fopen("результат", "a");
    if (file == NULL) {
        return false;
    }

    fprintf(file, "%s %.2f\n", time, percentage);
    fclose(file);
    return true;
}

/*
 * Читает результаты из файла.
 * filename - указатель на строку с именем файла.
 * times - указатель на массив для хранения времени.
 * percentages - указатель на массив для хранения процентов изменений.
 * Возвращает количество считанных результатов.
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
 * Сортирует результаты по проценту изменения.
 * times - массив для хранения времени.
 * percentages - массив для хранения процентов изменений.
 * count - количество результатов.
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
 * Фильтрует результаты по заданному значению.
 * data - массив указателей на данные:
 *   data[0] - массив времени (char*),
 *   data[1] - массив процентов изменений (float*),
 *   data[2] - массив для хранения отфильтрованного времени (char*),
 *   data[3] - массив для хранения отфильтрованных процентов (float*).
 * filter_value - значение для фильтрации.
 * is_greater - если true, фильтрует результаты больше filter_value, иначе меньше.
 * Возвращает количество отфильтрованных результатов.
 */
int filter_results(void* data[], float filter_value, bool is_greater) {
    char (*times)[20] = data[0];              // Массив времени
    float* percentages = data[1];             // Массив процентов изменений
    char (*filtered_times)[20] = data[2];     // Массив для отфильтрованного времени
    float* filtered_percentages = data[3];    // Массив для отфильтрованных процентов
    int count = *((int*)data[4]);             // Количество результатов

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
 * Преобразует время в формате "ЧЧ-MM-SS" в секунды.
 * time - строка с временем в формате "HH-MM-SS".
 * Возвращает количество секунд с начала дня.
 */
int get_time_in_seconds(const char* time) {
    int hours, minutes, seconds;
    sscanf(time, "%d-%d-%d", &hours, &minutes, &seconds);
    return hours * 3600 + minutes * 60 + seconds;
}