#include <stdio.h>
#include <locale.h>
#include <string.h>

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
    else printf("Слот %d: [%d] (%s), ", i, ID, item_names[ID]);}
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
        if (inventory[i] == ITEM_EMPTY) {
            continue;
        }

        if (array[inventory[i]] == 1) {
            inventory[i] = ITEM_EMPTY;
        }

        else {
            array[inventory[i]] = 1;
        }
    }
    printf("\nПосле удаления дубликатов:\n");
    for (int i = 0; i < SIZE; ++i) printf("Слот %d: [%d] (%s)\n", i, inventory[i], names[inventory[i]]);
}



void search_item(int *inventory, char item_names[][MAX_NAME_LEN]) {
    char subject_name[MAX_NAME_LEN];
            
    printf("Введите название предмета: ");

    if (fgets(subject_name, MAX_NAME_LEN, stdin)) {
        size_t len = strlen(subject_name);
        if (subject_name[len - 1] == '\n') {
            subject_name[len - 1] = '\0';
        }
    }

    int foundID = -1;
    for (int i = 0; i < SIZE; ++i) {
        if (strcmp(item_names[i], subject_name) == 0) {
            foundID = i;
            break;
        }
    }
            
    int count = 0; 
            
    for (int slot = 0; slot < SIZE; ++slot) {
        // Сравниваем ID предмета в слоте с тем, который мы нашли по названию
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
   
    if (fp == NULL) {
        printf("Не удалось открыть diary.txt для дозаписи\n");
        return;
    }

    // Ставим курсор в самый конец файла
    fseek(fp, 0, SEEK_END);
    
    // Если файл не пустой (позиция > 0), добавляем два перевода строки
    if (ftell(fp) > 0) {
        fprintf(fp, "\n\n"); 
    }

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



int main() {
    setlocale(LC_ALL, "");

    int current_day = DAY, current_hour = HOUR;

    int inventory[SIZE] = {
           ITEM_EMPTY,   ITEM_WOOD, ITEM_STONE,  ITEM_SEEDS,
           ITEM_BERRIES,  ITEM_WATER, ITEM_SAND, ITEM_STICK,
           ITEM_COAL,  ITEM_BARK };

    char names[SIZE][MAX_NAME_LEN] = {
        "пусто", "дерево", "камень", "семена",
        "ягоды", "вода", "песок", "палка", "уголь",
        "кора"
    };

    char character_name[LEN];
    printf("Введите имя до 32 символов: ");

    if (fgets(character_name, LEN, stdin)) {
         size_t len = strlen(character_name);
         if (character_name[len - 1] == '\n') {
            character_name[len - 1] = '\0';
        }
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
    int count;

    while (1) {

        printf("Здравствуйте, %s!\n", character_name);
        printf("0. Выход\n");
        printf("1. Посмотреть время\n");
        printf("2. Промотать время\n");
        printf("3. Посмотреть инвентарь\n");
        printf("4. Положить предмет в слот\n");
        printf("5. Выбросить предмет\n");
        printf("6. Устранение дубликатов \n");
        printf("7. Поиск предмета в рюкзаке по названию\n");
        printf("8. Записать состояние в дневник фермера\n");

        if (scanf("%d", &menu_item) != 1) {
            printf("Ошибка: введите одно целое число.\n");
            while (getchar() != '\n');
            continue;
        }
        clear_input_buffer();


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
}
}
}