#include <stdio.h>
#include <string.h>

#define MAX_TASKS 100
#define DATA_FILE "homework_data.txt"

// Структура за задача
typedef struct {
    char title[100];
    char dueDate[20];
    int status;
} Homework;

// Глобални променливи
Homework taskList[MAX_TASKS];
int taskCount = 0;

// Декларирам функциите
void loadData();
void saveData();
void addTask();
void viewTasks();
void markDone();
void searchTask();
void editTask();
void deleteTask();

int main() {
    loadData(); // Зареждаме старите задачите при стартиране

    int choice;
    while (1) {
        printf("\n=== HOMEWORK TRACKER ===\n");
        printf("1. Add Task\n");
        printf("2. View Tasks\n");
        printf("3. Mark Done\n");
        printf("4. Search\n");
        printf("5. Edit\n");
        printf("6. Delete\n");
        printf("7. Save & Exit\n");
        printf("Select: ");
        
        scanf("%d", &choice);
        getchar(); // Премахва '\n' след scanf

        if (choice == 1) addTask();
        else if (choice == 2) viewTasks();
        else if (choice == 3) markDone();
        else if (choice == 4) searchTask();
        else if (choice == 5) editTask();
        else if (choice == 6) deleteTask();
        else if (choice == 7) {
            saveData(); // Записваме промените във файла преди изход
            printf("Goodbye!\n");
            break;
        }
    }
    return 0;
}

// Функцията добавя нова задача в масива
void addTask() {
    if (taskCount < MAX_TASKS) {
        printf("Title: ");
        fgets(taskList[taskCount].title, 100, stdin);
        // Премахваме символа за нов ред, който fgets добавя в края
        taskList[taskCount].title[strcspn(taskList[taskCount].title, "\n")] = 0;
        
        printf("Due Date: ");
        fgets(taskList[taskCount].dueDate, 20, stdin);
        taskList[taskCount].dueDate[strcspn(taskList[taskCount].dueDate, "\n")] = 0;
        
        taskList[taskCount].status = 0; // Новата задача винаги е "чакаща"
        taskCount++;
        printf("Task added!\n");
    }
}

// Извежда всички задачи в табличен вид
void viewTasks() {
    printf("\nID | STATUS | TITLE | DATE\n");
    for (int i = 0; i < taskCount; i++) {
        // Проверка за текста на статуса
        char *st = "PENDING";
        if (taskList[i].status == 1) st = "DONE";
        printf("%d | %s | %s | %s\n", i + 1, st, taskList[i].title, taskList[i].dueDate);
    }
}

// Променя статуса на задача на "готова"
void markDone() {
    int id;
    printf("Enter ID to mark as DONE: ");
    scanf("%d", &id);
    // Проверяваме дали въведеният номер съществува в списъка
    if (id > 0 && id <= taskCount) {
        taskList[id - 1].status = 1; // -1, защото масивите почват от 0
        printf("Updated!\n");
    }
}

// Търси задача по заглавие (може и само част от името)
void searchTask() {
    char query[100];
    printf("Search for title: ");
    fgets(query, 100, stdin);
    query[strcspn(query, "\n")] = 0;
    
    for (int i = 0; i < taskCount; i++) {
        // strstr проверява дали 'query' се съдържа в заглавието
        if (strstr(taskList[i].title, query) != NULL) {
            printf("Found: ID %d - %s\n", i + 1, taskList[i].title);
        }
    }
}

// Позволява промяна на името на съществуваща задача
void editTask() {
    int id;
    printf("Enter ID to edit: ");
    scanf("%d", &id);
    getchar(); // Чистим буфера след scanf преди fgets
    if (id > 0 && id <= taskCount) {
        printf("New Title: ");
        fgets(taskList[id - 1].title, 100, stdin);
        taskList[id - 1].title[strcspn(taskList[id - 1].title, "\n")] = 0;
        printf("Task changed!\n");
    }
}

// Изтрива задача и пренарежда останалите в масива
void deleteTask() {
    int id;
    printf("Enter ID to delete: ");
    scanf("%d", &id);
    int idx = id - 1;
    if (idx >= 0 && idx < taskCount) {
        // Местим всяка следваща задача с една позиция наляво
        for (int i = idx; i < taskCount - 1; i++) {
            taskList[i] = taskList[i + 1];
        }
        taskCount--; // Намаляваме броя на задачите
        printf("Task deleted!\n");
    }
}

// Записва данните от масива в текстов файл
void saveData() {
    FILE *f = fopen(DATA_FILE, "w");
    if (f == NULL) return; // Ако файлът не може да се отвори излизаме
    
    // Първо записваме колко задачи има общо
    fprintf(f, "%d\n", taskCount);
    // После всяка задача на отделни редове
    for (int i = 0; i < taskCount; i++) {
        fprintf(f, "%s\n%s\n%d\n", taskList[i].title, taskList[i].dueDate, taskList[i].status);
    }
    fclose(f);
}

// Чете данните от текстовия файл обратно в масива
void loadData() {
    FILE *f = fopen(DATA_FILE, "r");
    if (f == NULL) return; // Ако файлът го няма спирам
    
    fscanf(f, "%d\n", &taskCount);
    for (int i = 0; i < taskCount; i++) {
        fgets(taskList[i].title, 100, f);
        taskList[i].title[strcspn(taskList[i].title, "\n")] = 0;
        fgets(taskList[i].dueDate, 20, f);
        taskList[i].dueDate[strcspn(taskList[i].dueDate, "\n")] = 0;
        fscanf(f, "%d\n", &taskList[i].status);
    }
    fclose(f);
}