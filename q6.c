#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define START_SIZE 8
#define NAME_SIZE 100
#define STR_SIZE 20
#define DATE_SIZE 12

typedef struct {
    int id;
    char name[NAME_SIZE];
    char batch[STR_SIZE];
    char type[STR_SIZE];
    char regDate[DATE_SIZE];
    char dob[DATE_SIZE];
    char interest[STR_SIZE];
} Member;

typedef struct {
    Member *list;
    int count;
    int limit;
} DB;

void initDB(DB *db);
void loadFile(DB *db, const char *file);
void saveFile(DB *db, const char *file);
int searchByID(DB *db, int id);
void addRecord(DB *db, const char *file);
void modifyRecord(DB *db, const char *file);
void removeRecord(DB *db, const char *file);
void showAll(DB *db);
void reportBatch(DB *db);

void initDB(DB *db) {
    db->list = (Member*)malloc(sizeof(Member) * START_SIZE);
    db->count = 0;
    db->limit = START_SIZE;
}

void loadFile(DB *db, const char *file) {
    FILE *fp = fopen(file, "rb");
    if (!fp) return;

    Member temp;
    while (fread(&temp, sizeof(Member), 1, fp)) {
        if (db->count == db->limit) {
            db->limit *= 2;
            db->list = (Member*)realloc(db->list, db->limit * sizeof(Member));
        }
        db->list[db->count++] = temp;
    }
    fclose(fp);
}

void saveFile(DB *db, const char *file) {
    FILE *fp = fopen(file, "wb");
    if (!fp) {
        printf("Unable to save database.\n");
        return;
    }
    fwrite(db->list, sizeof(Member), db->count, fp);
    fclose(fp);
}

int searchByID(DB *db, int id) {
    for (int i = 0; i < db->count; i++) {
        if (db->list[i].id == id) return i;
    }
    return -1;
}

void addRecord(DB *db, const char *file) {
    Member m;
    printf("Enter ID: ");
    scanf("%d", &m.id);
    getchar();

    if (searchByID(db, m.id) != -1) {
        printf("ID already registered.\n");
        return;
    }

    printf("Full Name: ");
    fgets(m.name, NAME_SIZE, stdin);
    m.name[strcspn(m.name, "\n")] = 0;

    printf("Batch (CS/SE/Cyber/AI): ");
    fgets(m.batch, STR_SIZE, stdin);
    m.batch[strcspn(m.batch, "\n")] = 0;

    printf("Membership (IEEE/ACM): ");
    fgets(m.type, STR_SIZE, stdin);
    m.type[strcspn(m.type, "\n")] = 0;

    printf("Reg Date (YYYY-MM-DD): ");
    fgets(m.regDate, DATE_SIZE, stdin);
    m.regDate[strcspn(m.regDate, "\n")] = 0;

    printf("DOB (YYYY-MM-DD): ");
    fgets(m.dob, DATE_SIZE, stdin);
    m.dob[strcspn(m.dob, "\n")] = 0;

    printf("Interest (IEEE/ACM/Both): ");
    fgets(m.interest, STR_SIZE, stdin);
    m.interest[strcspn(m.interest, "\n")] = 0;

    if (db->count == db->limit) {
        db->limit += 5; // different resizing style
        db->list = (Member*)realloc(db->list, db->limit * sizeof(Member));
    }

    db->list[db->count++] = m;

    FILE *fp = fopen(file, "ab");
    if (fp) {
        fwrite(&m, sizeof(Member), 1, fp);
        fclose(fp);
    }

    printf("Student registered.\n");
}

void modifyRecord(DB *db, const char *file) {
    int id;
    printf("Enter ID to update: ");
    scanf("%d", &id);
    getchar();

    int pos = searchByID(db, id);
    if (pos == -1) {
        printf("Record not found.\n");
        return;
    }

    printf("New Batch (current %s): ", db->list[pos].batch);
    fgets(db->list[pos].batch, STR_SIZE, stdin);
    db->list[pos].batch[strcspn(db->list[pos].batch, "\n")] = 0;

    printf("New Membership (current %s): ", db->list[pos].type);
    fgets(db->list[pos].type, STR_SIZE, stdin);
    db->list[pos].type[strcspn(db->list[pos].type, "\n")] = 0;

    saveFile(db, file);
    printf("Record updated.\n");
}

void removeRecord(DB *db, const char *file) {
    int id;
    printf("Enter ID to delete: ");
    scanf("%d", &id);
    getchar();

    int pos = searchByID(db, id);
    if (pos == -1) {
        printf("No such record.\n");
        return;
    }

    for (int i = pos; i < db->count - 1; i++) {
        db->list[i] = db->list[i + 1];
    }
    db->count--;

    saveFile(db, file);
    printf("Record removed.\n");
}

void showAll(DB *db) {
    if (db->count == 0) {
        printf("No records.\n");
        return;
    }

    for (int i = 0; i < db->count; i++) {
        Member m = db->list[i];
        printf("\nID: %d\nName: %s\nBatch: %s\nMembership: %s\nReg: %s\nDOB: %s\nInterest: %s\n",
               m.id, m.name, m.batch, m.type, m.regDate, m.dob, m.interest);
    }
}

void reportBatch(DB *db) {
    char batch[STR_SIZE], interest[STR_SIZE];

    printf("Batch (CS/SE/Cyber/AI): ");
    fgets(batch, STR_SIZE, stdin);
    batch[strcspn(batch, "\n")] = 0;

    printf("Interest (IEEE/ACM/Both): ");
    fgets(interest, STR_SIZE, stdin);
    interest[strcspn(interest, "\n")] = 0;

    int found = 0;
    for (int i = 0; i < db->count; i++) {
        if (!strcmp(db->list[i].batch, batch) &&
            !strcmp(db->list[i].interest, interest)) {

            printf("\nID: %d | %s | %s\n",
                   db->list[i].id,
                   db->list[i].name,
                   db->list[i].type);
            found = 1;
        }
    }

    if (!found) printf("No matching records.\n");
}

int main() {
    const char *file = "members.dat";
    DB db;
    initDB(&db);
    loadFile(&db, file);

    int option;

    while (1) {
        printf("\n--- Membership System ---\n");
        printf("1. Register Student\n");
        printf("2. Update Record\n");
        printf("3. Delete Record\n");
        printf("4. View All\n");
        printf("5. Batch Report\n");
        printf("6. Exit\n");
        printf("Choose: ");
        scanf("%d", &option);
        getchar();

        switch (option) {
            case 1: addRecord(&db, file); break;
            case 2: modifyRecord(&db, file); break;
            case 3: removeRecord(&db, file); break;
            case 4: showAll(&db); break;
            case 5: reportBatch(&db); break;
            case 6:
                saveFile(&db, file);
                free(db.list);
                printf("Goodbye.\n");
                return 0;
            default:
                printf("Invalid choice.\n");
        }
    }
}