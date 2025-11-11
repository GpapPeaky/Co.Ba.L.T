#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 50
#define MAX_COUNT 10

// Composite types
struct Student {
    char name[MAX_NAME_LEN];
    int age;
    float grade;
};

union Data {
    int i;
    float f;
    char str[20];
};

enum Day {
    MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, SUNDAY
};

// Typedef and storage classes
typedef struct Student Student;

static int global_count = 0;
extern int external_var;

// Function declarations
inline int max(int a, int b) { return a > b ? a : b; }
static void print_day(enum Day d);
const char* get_day_name(enum Day d);

// Function with control flow
int main(void) {
    auto int numbers[MAX_COUNT];
    register int i;

    for (i = 0; i < MAX_COUNT; i++) {
        numbers[i] = i * 2;
    }

    struct Student s1 = { "Alice", 20, 3.7f };
    union Data data;
    data.f = 3.14f;

    printf("Student: %s, Age: %d, Grade: %.2f\n", s1.name, s1.age, s1.grade);
    printf("Union value: %.2f\n", data.f);

    enum Day today = WEDNESDAY;
    print_day(today);

    int x = 5, y = 10;
    int larger = max(x, y);
    printf("Larger number is %d\n", larger);

    int value = 7;
    if (value > 10) {
        printf("Value is greater than 10\n");
    } else if (value == 10) {
        printf("Value is equal to 10\n");
    } else {
        printf("Value is less than 10\n");
    }

    int j = 0;
    while (j < 3) {
        printf("While loop iteration: %d\n", j);
        j++;
    }

    do {
        printf("Do-while loop running...\n");
        j--;
    } while (j > 0);

    switch (today) {
        case MONDAY:
            printf("Start of week\n");
            break;
        case FRIDAY:
            printf("End of week\n");
            break;
        default:
            printf("Midweek day\n");
            break;
    }

    goto end_label;

    printf("This will be skipped due to goto.\n");

end_label:
    printf("Reached the end label.\n");

    return 0;
}

void print_day(enum Day d) {
    printf("Today is %s\n", get_day_name(d));
}

const char* get_day_name(enum Day d) {
    switch (d) {
        case MONDAY: return "Monday";
        case TUESDAY: return "Tuesday";
        case WEDNESDAY: return "Wednesday";
        case THURSDAY: return "Thursday";
        case FRIDAY: return "Friday";
        case SATURDAY: return "Saturday";
        case SUNDAY: return "Sunday";
        default: return "Unknown";
    }
}
