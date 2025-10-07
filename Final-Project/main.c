#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void showAllMeetings() {
    FILE *file = fopen("meetings.csv", "r");
    char name[100], date[20], time[20], location[100];
    int count = 0;

    if (file == NULL) {
        printf("No meetings found!\n");
        return;
    }

    printf("\n%-5s %-25s %-12s %-8s %-20s\n", "No.", "Meeting Name", "Date", "Time", "Location");
    printf("-------------------------------------------------------------------------------\n");

    while (fscanf(file, " %99[^,],%19[^,],%19[^,],%99[^\n]", name, date, time, location) == 4) {
        printf("%-5d %-25s %-12s %-8s %-20s\n", ++count, name, date, time, location);
    }

    if (count == 0)
        printf("No meeting records available.\n");

    fclose(file);
}

int isDuplicateMeeting(char *name, char *date, char *time, char *location) {
    FILE *file = fopen("meetings.csv", "r");
    if (!file) return 0;

    char n[100], d[20], t[20], l[100];
    while (fscanf(file, " %99[^,],%19[^,],%19[^,],%99[^\n]", n, d, t, l) == 4) {
        if (strcmp(name, n) == 0 && strcmp(date, d) == 0 && strcmp(time, t) == 0 && strcmp(location, l) == 0) {
            fclose(file);
            return 1; // duplicate
        }
    }
    fclose(file);
    return 0;
}

void addMeeting() {
    char name[100], date[20], time[20], location[100];

    printf("Enter meeting name: ");
    scanf(" %[^\n]", name);
    printf("Enter date (YYYY-MM-DD): ");
    scanf(" %s", date);
    printf("Enter time (HH:MM): ");
    scanf(" %s", time);
    printf("Enter location: ");
    scanf(" %[^\n]", location);

    if (isDuplicateMeeting(name, date, time, location)) {
        printf("Error: Meeting with same name, date, time, and location already exists!\n");
        return;
    }

    FILE *file = fopen("meetings.csv", "a");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }

    fprintf(file, "%s,%s,%s,%s\n", name, date, time, location);
    fclose(file);
    printf("Meeting added successfully!\n");
}

void searchMeeting() {
    char keyword[100], line[256];
    char name[100], date[20], time[20], location[100];
    int found = 0;

    printf("Enter meeting name to search: ");
    scanf(" %[^\n]", keyword);

    FILE *file = fopen("meetings.csv", "r");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }

    printf("\nSearch results:\n");
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%99[^,],%19[^,],%19[^,],%99[^\n]", name, date, time, location) == 4) {
            if (strcmp(name, keyword) == 0) {
                printf("MeetingName: %s\nDate: %s\nTime: %s\nLocation: %s\n\n",
                       name, date, time, location);
                found = 1;
            }
        }
    }

    if (!found) printf("No meeting found with name '%s'\n", keyword);
    fclose(file);
}

void updateMeeting() {
    showAllMeetings();

    char keyword[100], line[256];
    char name[100], date[20], time[20], location[100];
    int found = 0;

    char allNames[100][100], allDates[100][20], allTimes[100][20], allLocations[100][100];
    int count = 0;

    printf("\nEnter meeting name to update: ");
    scanf(" %[^\n]", keyword);

    FILE *file = fopen("meetings.csv", "r");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%99[^,],%19[^,],%19[^,],%99[^\n]", name, date, time, location) == 4) {
            strcpy(allNames[count], name);
            strcpy(allDates[count], date);
            strcpy(allTimes[count], time);
            strcpy(allLocations[count], location);
            count++;
        }
    }
    fclose(file);

    for (int i = 0; i < count; i++) {
        if (strcmp(allNames[i], keyword) == 0) {
            printf("\nMeeting found. Enter new details:\n");
            printf("New name (current: %s): ", allNames[i]);
            scanf(" %[^\n]", allNames[i]);
            printf("New date (current: %s): ", allDates[i]);
            scanf(" %s", allDates[i]);
            printf("New time (current: %s): ", allTimes[i]);
            scanf(" %s", allTimes[i]);
            printf("New location (current: %s): ", allLocations[i]);
            scanf(" %[^\n]", allLocations[i]);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("No meeting found with name '%s'\n", keyword);
        return;
    }

    file = fopen("meetings.csv", "w");
    if (!file) {
        printf("Error opening file for writing!\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s,%s,%s,%s\n", allNames[i], allDates[i], allTimes[i], allLocations[i]);
    }
    fclose(file);
    printf("Meeting updated successfully!\n");
}

void deleteMeeting() {
    showAllMeetings();

    char keyword[100], line[256];
    char name[100], date[20], time[20], location[100];
    int found = 0;

    char allNames[100][100], allDates[100][20], allTimes[100][20], allLocations[100][100];
    int count = 0;

    printf("\nEnter meeting name to delete: ");
    scanf(" %[^\n]", keyword);

    FILE *file = fopen("meetings.csv", "r");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%99[^,],%19[^,],%19[^,],%99[^\n]", name, date, time, location) == 4) {
            strcpy(allNames[count], name);
            strcpy(allDates[count], date);
            strcpy(allTimes[count], time);
            strcpy(allLocations[count], location);
            count++;
        }
    }
    fclose(file);

    file = fopen("meetings.csv", "w");
    if (!file) {
        printf("Error opening file for writing!\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        if (strcmp(allNames[i], keyword) != 0) {
            fprintf(file, "%s,%s,%s,%s\n", allNames[i], allDates[i], allTimes[i], allLocations[i]);
        } else {
            found = 1; // delete only first matching
            // break; // uncomment if want to delete only first occurrence
        }
    }
    fclose(file);

    if (found) printf("Meeting '%s' deleted successfully!\n", keyword);
    else printf("No meeting found with name '%s'\n", keyword);
}

// ---------------------- Unit Tests ----------------------
void test_isDuplicateMeeting() {
    printf("\n--- Unit Test: Duplicate Meeting ---\n");
    if (isDuplicateMeeting("Nonexistent", "2025-01-01", "10:00", "Room A"))
        printf("Failed: Should not be duplicate\n");
    else
        printf("Passed: Not duplicate\n");
}

void test_addMeetingUnit() {
    printf("\n--- Unit Test: Add Meeting ---\n");
    // just check duplicate logic
    addMeeting(); // manual input
}

// ---------------------- End-to-End Test ----------------------
void test_E2E() {
    printf("\n--- E2E Test: Add, Search, Update, Delete ---\n");
    // 1. Add
    printf("Add a meeting for E2E test:\n");
    addMeeting();

    // 2. Search
    printf("Search meeting for E2E test:\n");
    searchMeeting();

    // 3. Update
    printf("Update meeting for E2E test:\n");
    updateMeeting();

    // 4. Delete
    printf("Delete meeting for E2E test:\n");
    deleteMeeting();
}

// ---------------------- Test Runner ----------------------
void runTests() {
    test_isDuplicateMeeting();
    test_addMeetingUnit();
    test_E2E();
}

// ---------------------- Main ----------------------
int main() {
    int choice;

    while (1) {
        printf("\nMeeting Management System\n");
        printf("0. Exit\n");
        printf("1. Add Meeting\n");
        printf("2. Search Meeting\n");
        printf("3. Update Meeting\n");
        printf("4. Delete Meeting\n");
        printf("5. Run Tests\n");
        printf("Select menu: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addMeeting(); break;
            case 2: searchMeeting(); break;
            case 3: updateMeeting(); break;
            case 4: deleteMeeting(); break;
            case 5: runTests(); break;
            case 0: printf("Exit program\n"); exit(0);
            default: printf("Invalid menu!\n");
        }
    }

    return 0;
}
