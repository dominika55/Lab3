#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 10
#define DAY 1
#define HOUR 8
#define LEN 33
#define LENGTH 50

#define ITEM_EMPTY    0
#define ITEM_WOOD     1
#define ITEM_STONE    2
#define ITEM_SEEDS    3
#define ITEM_BERRIES  4
#define ITEM_WATER    5
#define ITEM_SAND     6
#define ITEM_STICK    7
#define ITEM_COAL     8
#define ITEM_BARK     9

#define MAX_ITEMS 10
#define MAX_NAME_LEN 20

#define MAX_MAP_SIZE 30
#define MIN_MAP_SIZE 5

#define ABYSS ' ' 



void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}



void rewind_time(int *current_day, int *current_hour) {
    int work_hours;

    printf("Введите кол-во часов на работу");

    if (scanf("%d", &work_hours) != 1) {
        printf("Ошибка: введите одно целое число\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    *current_hour += work_hours;

    if (*current_hour >= 24) *current_day += *current_hour / 24;
    *current_hour %= 24;

    printf("Текущее время: День %d, %02d:00\n", *current_day, *current_hour);
}



void print_inventory(int *inventory, char item_names[][MAX_NAME_LEN]) {
    for (int i = 0; i < SIZE; ++i) {

        int ID = inventory[i];

        if (i == SIZE - 1) printf("Слот %d: [%d] (%s)\n", i, ID, item_names[ID]);
        else printf("Слот %d: [%d] (%s), ", i, ID, item_names[ID]);
    }
    printf("\n");
}



void put_item(int *inventory) {
    int index;
    int ID;

    printf("Введите индекс слота (от 0 до 9) и ID предмета\n");

    if (scanf("%d %d", &index, &ID) != 2) {
        printf("Ошибка: введите два целых числа\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    if (index >= 0 && index < SIZE && ID >= 0 && ID < SIZE) inventory[index] = ID;
}



void drop_item(int *inventory) {
    int index_slot;

    printf("Введите индекс слота от 0 до 9\n");

    if (scanf("%d", &index_slot) != 1) {
        printf("Ошибка: введите одно целое число\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    if (index_slot >= 0 && index_slot < SIZE) {
        inventory[index_slot] = ITEM_EMPTY;
        printf("Слот %d очищен\n", index_slot);
    } else {
        printf("Ошибка: индекс слота должен быть от 0 до 9\n");
    }
}



void remove_dublicate(int *inventory, char names[][MAX_NAME_LEN]) {
    printf("Текущий инвентарь:\n");
    for (int i = 0; i < SIZE; ++i) {
        printf("Слот %d: [%d] (%s)\n", i, inventory[i], names[inventory[i]]);
    }

    int array[SIZE] = {0};
    for (int i = 0; i < SIZE; ++i) {
        if (inventory[i] == ITEM_EMPTY) continue;
        if (array[inventory[i]] == 1) inventory[i] = ITEM_EMPTY;
        else array[inventory[i]] = 1;
    }
    printf("\nПосле удаления дубликатов:\n");
    for (int i = 0; i < SIZE; ++i) printf("Слот %d: [%d] (%s)\n", i, inventory[i], names[inventory[i]]);
}



void search_item(int *inventory, char item_names[][MAX_NAME_LEN]) {
    char subject_name[MAX_NAME_LEN];

    printf("Введите название предмета: ");
    clear_input_buffer();

    if (fgets(subject_name, MAX_NAME_LEN, stdin)) {
        size_t len = strlen(subject_name);
        if (subject_name[len - 1] == '\n') subject_name[len - 1] = '\0';
    }

    int foundID = -1;
    for (int i = 0; i < SIZE; ++i) {
        if (strcmp(item_names[i], subject_name) == 0) { foundID = i; break; }
    }

    int count = 0;
    for (int slot = 0; slot < SIZE; ++slot) {
        if (inventory[slot] == foundID) {
            printf("Слот %d: [%d]\n", slot, inventory[slot]);
            count++;
        }
    }
    if (count == 0) printf("Такого предмета нет\n");
    printf("\n");
}



void writeToFarmerDiary(char *character_name, int *inventory, char item_names[][MAX_NAME_LEN], int current_day, int current_hour) {
    FILE *fp = fopen("diary.txt", "a");
    if (fp == NULL) { printf("Не удалось открыть diary.txt\n"); return; }

    fseek(fp, 0, SEEK_END);
    if (ftell(fp) > 0) fprintf(fp, "\n\n");

    fprintf(fp, "Фермер: %s\n", character_name);
    fprintf(fp, "День: %d, час: %02d:00\n", current_day, current_hour);
    fprintf(fp, "Предметы в инвентаре:\n");

    for (int i = 0; i < SIZE; ++i) {
        int id = inventory[i];
        if (id != ITEM_EMPTY && id >= 0 && id < MAX_ITEMS) {
            fprintf(fp, "   • %s\n", item_names[id]);
        }
    }
    fclose(fp);
    printf("Сводка успешно записана в diary.txt!\n");
}



void unpackMap() {
    FILE* fin = fopen("input.txt", "r");
    FILE* fout = fopen("output.txt", "w");

    if (fin == NULL) { printf("Ошибка: файл input.txt не найден!\n"); return; }
    if (fout == NULL) { printf("Ошибка: не удалось создать output.txt\n"); fclose(fin); return; }

    char line[256];
    int expectedLen = -1;
    bool success = true;

    while (fgets(line, sizeof(line), fin) != NULL && success) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') { line[len - 1] = '\0'; len--; }

        int currentLen = 0;
        int i = 0;

        while (i < len && success) {
            char currentChar = line[i];
            if (isdigit(currentChar)) {
                int count = currentChar - '0';
                i++;
                if (i >= len) { printf("Ошибка: цифра без символа!\n"); success = false; break; }
                char symbol = line[i];
                for (int k = 0; k < count; ++k) {
                    putchar(symbol);
                    fprintf(fout, "%c", symbol);
                    currentLen++;
                }
                i++;
            } else {
                putchar(currentChar);
                fprintf(fout, "%c", currentChar);
                currentLen++;
                i++;
            }
        }

        putchar('\n');
        fprintf(fout, "\n");

        if (expectedLen == -1) expectedLen = currentLen;
        else if (currentLen != expectedLen) {
            printf("Ошибка геометрии: строки разной длины!\n");
            success = false;
        }
    }

    fclose(fin);
    fclose(fout);

    if (success) printf("Карта успешно распакована в output.txt!\n");
    else printf("Распаковка завершена с ошибками\n");
    printf("\n");
}



char** generate_map(int *out_size, int *hero_y, int *hero_x) {
    int size = rand() % (MAX_MAP_SIZE - MIN_MAP_SIZE + 1) + MIN_MAP_SIZE;
    *out_size = size;

    char **map;
    printf("\nГенерируем карту размером %dx%d\n", size, size);

    map = (char **)malloc(size * sizeof(char *));
    if (map == NULL) { printf("Ошибка памяти!\n"); return NULL; }

    int obstacle_chance = rand() % (30 - 15 + 1) + 15;
    int hero_placed = 0;

    for (int i = 0; i < size; i++) {
        map[i] = malloc((size + 1) * sizeof(char));

        if (map[i] == NULL) {
            for (int j = 0; j < i; j++) free(map[j]);
            free(map);
            return NULL;
        }

        for (int j = 0; j < size; j++) {
            if (i == 0 || i == size - 1 || j == 0 || j == size - 1) {
                map[i][j] = '#';
            } else {
                map[i][j] = '.';

                if (rand() % 100 < obstacle_chance)
                    map[i][j] = (rand() % 2 == 0) ? 'T' : 'R';

                if (!hero_placed && map[i][j] == '.') {
                    map[i][j] = '@';
                    *hero_y = i;
                    *hero_x = j;
                    hero_placed = 1;
                }
            }
        }
        map[i][size] = '\0';
    }
    return map;
}



void setup_stair_U(char **map, int size, int hero_y, int hero_x) {
    int free_cells[MAX_MAP_SIZE * MAX_MAP_SIZE][2];
    int count = 0;

    for (int i = 1; i < size - 1; ++i)
        for (int j = 1; j < size - 1; ++j)
            if (map[i][j] == '.' && !(i == hero_y && j == hero_x)) {
                free_cells[count][0] = i;
                free_cells[count][1] = j;
                count++;
            }

    if (count > 0) {
        int r = rand() % count;
        map[free_cells[r][0]][free_cells[r][1]] = 'U';
        return;
    }

    for (int i = 1; i < size - 1; ++i)
        for (int j = 1; j < size - 1; ++j)
            if (!(i == hero_y && j == hero_x)) { map[i][j] = 'U'; return; }
}



bool move_hero(char **map, int size, int *y, int *x, char direction) {
    int new_y = *y, new_x = *x;

    switch (direction) {
        case 'W': case 'w': new_y--; break;
        case 'S': case 's': new_y++; break;
        case 'A': case 'a': new_x--; break;
        case 'D': case 'd': new_x++; break;
        default: return false;
    }

    if (new_y < 0 || new_y >= size || new_x < 0 || new_x >= size) {
        printf("Нельзя идти туда!\n");
        return false;
    }

    char cell = map[new_y][new_x];
    if (cell == '#' || cell == 'T' || cell == 'R') {
        printf("Там препятствие (%c)\n", cell);
        return false;
    }

    if (cell == ABYSS) {
        printf("Там бездна\n");
        return false;
    }

    *y = new_y;
    *x = new_x;
    return true;
}



void draw_map(char **map, int size, int hero_y, int hero_x) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == hero_y && j == hero_x) putchar('@');
            else putchar(map[i][j]);
        }
        putchar('\n');
    }
}



void collapse_perimeter(char **map, int size, int level) {
    int top = level, bottom = size - 1 - level;
    int left = level, right = size - 1 - level;

    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            if (i == top || i == bottom || j == left || j == right) {
                if (map[i][j] == 'D') continue; // не рушим выход D (если он на границе)
                map[i][j] = ABYSS;
            }
}



void collapsing_tower() {
    char **map;
    int map_size;
    int hero_y, hero_x;
    int floor_level = 1;

    map = generate_map(&map_size, &hero_y, &hero_x);
    if (map == NULL) { printf("Ошибка создания карты!\n"); return; }

    map[hero_y][hero_x] = 'D';
    setup_stair_U(map, map_size, hero_y, hero_x);

    int turn_count = 0;
    const int COLLAPSE_INTERVAL = 5;
    bool game_over = false;
    int level = 0;
    bool stair_lost = false;

    while (!game_over) {
        draw_map(map, map_size, hero_y, hero_x);

        printf("\nХодов: %d | Этаж: %d\n", turn_count, floor_level);

        if (stair_lost) {
            printf("Башня рушится! ЛЕСТНИЦА U ПРОВАЛИЛАСЬ!\n");
            printf("СРОЧНО эвакуируйся: нажми Q!\n");
        } else {
            printf("W/A/S/D — идти. Q — выйти домой (только стоя на D)\n");
        }
        printf("Твой ход: ");

        char direction;
        if (scanf(" %c", &direction) != 1) { clear_input_buffer(); continue; }

        if (direction == 'Q' || direction == 'q') {
            if (stair_lost || map[hero_y][hero_x] == 'D') {
                printf("\n✅ Ты вернулся домой\n");
                game_over = true;
                break;
            }
            printf("🚫 Ты не у выхода! Найди D.\n");
            continue;
        }

        // Блокировка WASD в режиме эвакуации
        if (stair_lost) {
            printf("\n⛔ Башня рушится! Только Q!\n");
            continue;
        }

        // если препятствие 
        if (!move_hero(map, map_size, &hero_y, &hero_x, direction)) continue;
        turn_count++;


        if (map[hero_y][hero_x] == 'U') {
            printf("\nТы нашел лестницу! Переходим на следующий этаж...\n");
            floor_level++;

            for (int i = 0; i < map_size; i++) free(map[i]);
            free(map);

            map = generate_map(&map_size, &hero_y, &hero_x);
            if (map == NULL) { printf("Критическая ошибка!\n"); return; }

            map[hero_y][hero_x] = 'D';
            setup_stair_U(map, map_size, hero_y, hero_x);

            turn_count = 0;
            level = 0;
            stair_lost = false;

            printf("Этаж %d! Продолжай путь\n\n", floor_level);
            continue;
        }

        if (turn_count % COLLAPSE_INTERVAL == 0) {
            collapse_perimeter(map, map_size, level);
            level++;

            bool stair_exists = false;
            for (int i = 0; i < map_size; i++) {
                for (int j = 0; j < map_size; j++)
                    if (map[i][j] == 'U') { stair_exists = true; break; }
                if (stair_exists) break;
            }

            if (!stair_exists) {
                // U провалилась — эвакуация
                stair_lost = true;

                // Стираем все D
                for (int i = 0; i < map_size; i++)
                    for (int j = 0; j < map_size; j++)
                        if (map[i][j] == 'D') map[i][j] = '.';

                // Ставим D под героем
                map[hero_y][hero_x] = 'D';

                printf("\n💥 Лестница U провалилась в бездну!\n");
                printf("Башня рушится! Выход D теперь под тобой. Нажми Q!\n\n");
            } else {
                printf("Карта сужается к центру.\n");
            }

            if (map[hero_y][hero_x] == ABYSS) {
                printf("💥 Пол исчез! Ты упал в бездну.\n");
                game_over = true;
            }
        }
        printf("\n");
    }

    for (int i = 0; i < map_size; i++) free(map[i]);
    free(map);

    printf("Возврат в главное меню.\n\n");
}



int main() {
    setlocale(LC_ALL, "");
    srand((unsigned int)time(NULL));

    int current_day = DAY, current_hour = HOUR;

    int inventory[SIZE] = {
        ITEM_EMPTY, ITEM_WOOD, ITEM_STONE, ITEM_SEEDS,
        ITEM_BERRIES, ITEM_WATER, ITEM_SAND, ITEM_STICK,
        ITEM_COAL, ITEM_BARK
    };

    char names[SIZE][MAX_NAME_LEN] = {
        "пусто", "дерево", "камень", "семена",
        "ягоды", "вода", "песок", "палка", "уголь", "кора"
    };

    char character_name[LEN];
    printf("Введите имя до 32 символов: ");
    if (fgets(character_name, LEN, stdin)) {
        size_t len = strlen(character_name);
        if (character_name[len - 1] == '\n') character_name[len - 1] = '\0';
    }

    char item_names[MAX_ITEMS][MAX_NAME_LEN];
    FILE *fp = fopen("items.txt", "r");

    if (fp == NULL) {
        printf("Не удалось открыть файл\n");
        for (int i = 0; i < MAX_ITEMS; i++) {
            strncpy(item_names[i], names[i], MAX_NAME_LEN - 1);
            item_names[i][MAX_NAME_LEN - 1] = '\0';
        }
    } else {
        int id;
        char temp[MAX_NAME_LEN];
        while (fscanf(fp, "%d %14s", &id, temp) == 2) {
            if (id >= 0 && id < MAX_ITEMS) {
                strncpy(item_names[id], temp, MAX_NAME_LEN - 1);
                item_names[id][MAX_NAME_LEN - 1] = '\0';
            }
        }
        fclose(fp);
    }

    int menu_item;

    while (1) {
        printf("Здравствуйте, %s!\n", character_name);
        printf("0. Выход\n");
        printf("1. Посмотреть время\n");
        printf("2. Промотать время\n");
        printf("3. Посмотреть инвентарь\n");
        printf("4. Положить предмет в слот\n");
        printf("5. Выбросить предмет\n");
        printf("6. Устранение дубликатов\n");
        printf("7. Поиск предмета в рюкзаке по названию\n");
        printf("8. Записать состояние в дневник фермера\n");
        printf("9. Распаковщик карты\n");
        printf("10. Отправиться в приключение\n");

        if (scanf("%d", &menu_item) != 1) {
            printf("Ошибка: введите одно целое число.\n");
            clear_input_buffer();
            continue;
        }

        switch (menu_item)
        {

        case 0: {
             return 0;
        }

        case 1: {

            printf("Текущее время: День %d, %02d:00\n", current_day, current_hour);
            break;
        }

        case 2: {
            rewind_time(&current_day, &current_hour);
            break;
        }
  
        case 3: {
            print_inventory(inventory, item_names);
            break;
        }
        
        case 4: {
            put_item(inventory);
            break;
        }

        case 5: {
            drop_item(inventory);
            break;
        }
            
        case 6: {
            remove_dublicate(inventory, item_names);
            break;
        }
        
        case 7: {
            search_item(inventory, item_names);
            break;
        }

        case 8: {
            writeToFarmerDiary(character_name, inventory, item_names, current_day, current_hour);
            break;
        }

        case 9: {
            unpackMap();
            break;
        }

        case 10: {
            collapsing_tower();
            break;
        }

        default: {
            printf("Неизвестный пункт меню!\n");
            break;
        }
    }
    }
}