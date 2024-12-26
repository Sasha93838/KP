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
#define D 1

 // Структура для хранения битовой матрицы
struct BitMatrix_type {
    int width;              // Ширина матрицы
    int height;             // Высота матрицы
    unsigned char* data;    // Указатель на данные матрицы (бинарные значения)
};
typedef struct BitMatrix_type BitMatrix;

// Структура для хранения результатов мониторинга
struct ResultEntry_type {
    char time[20];          // Время проведения мониторинга (строка формата "ЧЧ:MM")
    float percentage;       // Процент изменений между матрицами
};
typedef struct ResultEntry_type ResultEntry;

/* Прототипы функций */
char* generate_filename(void); // Генерация имени файла
bool write_bit_matrix_to_file(const char* filename); // Запись матрицы в файл
int destroy_bit_matrix(BitMatrix* matrix); // Уничтожение матрицы
BitMatrix* read_bit_matrix(const char* filename); // Чтение данных матрицы
float compare_matrices(BitMatrix* matrix1, BitMatrix* matrix2); // Сравнение матриц
bool write_result_to_file(const char* time, float percentage); // Запись результатов в файл
ResultEntry* read_results_from_file(int* count); // Чтение результатов из файла
int sort_results(ResultEntry* results, int count); // Сортировка результатов
ResultEntry* filter_results(ResultEntry* results, int count, float filter_value, bool is_greater, int* filtered_count); // Фильтрация результатов
int get_time_in_seconds(const char* time); // Получение времени

/* Глобальные переменные для хранения текущего времени */
int current_hour = 12;
int current_minute = 0;

/* Основная функция программы */
int main() {
    setlocale(LC_CTYPE, "RUS");
    int monitoring_enabled = 0;
    int mode = 0;
    int file_counter = 0;

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

    srand((unsigned int)time(NULL)); 

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
        case 3: {
            int count = 0;
            ResultEntry* results = read_results_from_file(&count);
            if (results == NULL) {
                printf("=== Файл результатов не найден. ===\n");
                break;
            }

            sort_results(results, count);
            printf(" Результаты (отсортированные по проценту изменения): \n");
            printf("  Время    Процент изменения\n");
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
                printf("=== Файл результатов не найден. ===\n");
                break;
            }

            float filter_value;
            printf("************************************************\n");
            printf("Введите число для фильтрации (результаты больше этого числа): ");
            scanf("%f", &filter_value);
            printf("************************************************\n");

            int filtered_count = 0;
            ResultEntry* filtered_results = filter_results(results, count, filter_value, true, &filtered_count);
            printf(" Результаты фильтрации (больше %.2f): \n", filter_value);
            for (int i = 0; i < filtered_count; i++) {
                printf("Время: %s, Процент изменения: %.2f%%\n", filtered_results[i].time, filtered_results[i].percentage);
            }
            free(filtered_results);
            free(results);
            break;
        }
        case 5: {
            int count = 0;
            ResultEntry* results = read_results_from_file(&count);
            if (results == NULL) {
                printf("=== Файл результатов не найден. ===\n");
                break;
            }

            float filter_value;
            printf("************************************************\n");
            printf("Введите число для фильтрации (результаты меньше этого числа): ");
            scanf("%f", &filter_value);
            printf("************************************************\n");

            int filtered_count = 0;
            ResultEntry* filtered_results = filter_results(results, count, filter_value, false, &filtered_count);
            printf(" Результаты фильтрации (меньше %.2f): \n", filter_value);
            for (int i = 0; i < filtered_count; i++) {
                printf("Время: %s, Процент изменения: %.2f%%\n", filtered_results[i].time, filtered_results[i].percentage);
            }
            free(filtered_results);
            free(results);
            break;
        }
        case 6: {
            int count = 0;
            ResultEntry* results = read_results_from_file(&count);
            if (results == NULL) {
                printf("=== Файл результатов не найден. ===\n");
                break;
            }

            printf(" Результаты из файла: \n");
            printf("  Время    Процент изменения\n");
            for (int i = 0; i < count; i++) {
                printf(" %s      %.2f\n", results[i].time, results[i].percentage);
            }

            free(results);
            break;
        }
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

                // Вывод сообщения о создании файла
                printf(" Файл создан: %s \n", current_filename);

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

                        char time[20];
                        sprintf(time, "%02d-%02d", current_hour, current_minute);
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
 * Генерирует имя файла на основе текущего времени.
 * Возвращает строку с именем файла.
 * Побочный эффект: увеличивает текущее время на 5 минут.
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
 * count - указатель на переменную, в которую будет записано количество результатов.
 * Возвращает массив структур ResultEntry с результатами, если чтение прошло успешно, иначе NULL.
 */
ResultEntry* read_results_from_file(int* count) {
    FILE* file = fopen("результат", "r");
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
 * Сортирует результаты по проценту изменения.
 * results - массив результатов для сортировки.
 * count - количество элементов в массиве.
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
 * Фильтрует результаты по заданному значению.
 * results - массив результатов для фильтрации.
 * count - количество элементов в массиве.
 * filter_value - значение для фильтрации.
 * is_greater - если true, фильтрует результаты больше filter_value, иначе меньше.
 * filtered_count - указатель на переменную, в которую будет записано количество отфильтрованных результатов.
 * Возвращает массив отфильтрованных результатов.
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
 * Преобразует время в формате "ЧЧ-MM" в секунды.
 * time - строка с временем в формате "HH-MM".
 * Возвращает количество секунд с начала дня.
 */
int get_time_in_seconds(const char* time) {
    int hours, minutes;
    sscanf(time, "%d-%d", &hours, &minutes);
    return hours * 3600 + minutes * 60;
}