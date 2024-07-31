#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define MAX_COURSES 4
#define DATA_FILE "student_data.txt"

typedef struct {
    char name[10];
    int credits;
    char grade;
} Course;

typedef struct {
    int id;
    char full_name[50];
    char department[10];
    Course enrolled_courses[MAX_COURSES];
    int num_courses;
    float gpa;
} Student;

Student student_list[MAX_STUDENTS];
int total_students = 0;

int convert_grade_to_points(char grade) {
    switch (grade) {
        case 'S': return 10;
        case 'A': return 9;
        case 'B': return 8;
        case 'C': return 7;
        case 'D': return 6;
        case 'E': return 5;
        case 'F': return 0;
        default: return 0;
    }
}

void update_gpa(Student *stu) {
    int points_sum = 0;
    int credits_sum = 0;
    for (int i = 0; i < stu->num_courses; i++) {
        points_sum += convert_grade_to_points(stu->enrolled_courses[i].grade) * stu->enrolled_courses[i].credits;
        credits_sum += stu->enrolled_courses[i].credits;
    }
    stu->gpa = (credits_sum > 0) ? (float) points_sum / credits_sum : 0.0f;
}

void add_new_student() {
    if (total_students >= MAX_STUDENTS) {
        printf("Student limit reached.\n");
        return;
    }
    Student *stu = &student_list[total_students++];
    printf("Enter student ID: ");
    scanf("%d", &stu->id);
    printf("Enter full name: ");
    scanf("%s", stu->full_name);
    printf("Enter department: ");
    scanf("%s", stu->department);
    printf("Enter number of courses (3 to 4): ");
    scanf("%d", &stu->num_courses);
    for (int i = 0; i < stu->num_courses; i++) {
        printf("Enter course %d name: ", i + 1);
        scanf("%s", stu->enrolled_courses[i].name);
        printf("Enter course %d credits: ", i + 1);
        scanf("%d", &stu->enrolled_courses[i].credits);
        printf("Enter course %d grade: ", i + 1);
        scanf(" %c", &stu->enrolled_courses[i].grade);
    }
    update_gpa(stu);
}

void initialize_gpa_for_all() {
    for (int i = 0; i < total_students; i++) {
        update_gpa(&student_list[i]);
    }
    printf("GPA column has been updated for all students.\n");
}

void remove_course(int id, const char *course_name) {
    for (int i = 0; i < total_students; i++) {
        if (student_list[i].id == id) {
            for (int j = 0; j < student_list[i].num_courses; j++) {
                if (strcmp(student_list[i].enrolled_courses[j].name, course_name) == 0) {
                    for (int k = j; k < student_list[i].num_courses - 1; k++) {
                        student_list[i].enrolled_courses[k] = student_list[i].enrolled_courses[k + 1];
                    }
                    student_list[i].num_courses--;
                    update_gpa(&student_list[i]);
                    printf("Course %s removed for student %d.\n", course_name, id);
                    return;
                }
            }
        }
    }
    printf("Course not found for the student.\n");
}

void add_course(int id, const char *course_name, int credits, char grade) {
    for (int i = 0; i < total_students; i++) {
        if (student_list[i].id == id) {
            if (student_list[i].num_courses >= MAX_COURSES) {
                printf("Cannot add more courses.\n");
                return;
            }
            Course *new_course = &student_list[i].enrolled_courses[student_list[i].num_courses++];
            strcpy(new_course->name, course_name);
            new_course->credits = credits;
            new_course->grade = grade;
            update_gpa(&student_list[i]);
            printf("Course %s added for student %d.\n", course_name, id);
            return;
        }
    }
    printf("Student not found.\n");
}

void change_course_name(int id, const char *old_name, const char *new_name) {
    for (int i = 0; i < total_students; i++) {
        if (student_list[i].id == id) {
            for (int j = 0; j < student_list[i].num_courses; j++) {
                if (strcmp(student_list[i].enrolled_courses[j].name, old_name) == 0) {
                    strcpy(student_list[i].enrolled_courses[j].name, new_name);
                    printf("Course name changed from %s to %s for student %d.\n", old_name, new_name, id);
                    return;
                }
            }
        }
    }
    printf("Course not found for the student.\n");
}

void adjust_grades(char grade, int new_points) {
    for (int i = 0; i < total_students; i++) {
        for (int j = 0; j < student_list[i].num_courses; j++) {
            if (student_list[i].enrolled_courses[j].grade == grade) {
                student_list[i].enrolled_courses[j].grade = new_points;
            }
        }
        update_gpa(&student_list[i]);
    }
    printf("Grades have been updated for all students.\n");
}

void refresh_student_gpa(int id) {
    for (int i = 0; i < total_students; i++) {
        if (student_list[i].id == id) {
            update_gpa(&student_list[i]);
            printf("GPA updated for student %d.\n", id);
            return;
        }
    }
    printf("Student not found.\n");
}

void print_grade_report(int id) {
    for (int i = 0; i < total_students; i++) {
        if (student_list[i].id == id) {
            printf("Grade Report for student %d:\n", id);
            printf("Course Name    Grade\n");
            printf("--------------------\n");
            for (int j = 0; j < student_list[i].num_courses; j++) {
                printf("%-15s %c\n", student_list[i].enrolled_courses[j].name, student_list[i].enrolled_courses[j].grade);
            }
            printf("GPA: %.2f\n", student_list[i].gpa);
            return;
        }
    }
    printf("Student not found.\n");
}


void display_main_menu() {
    printf("1. Add a new student\n");
    printf("2. Update GPA for all students\n");
    printf("3. Remove a course\n");
    printf("4. Add a course\n");
    printf("5. Change course name\n");
    printf("6. Refresh GPA for all students\n");
    printf("7. Upgrade grades\n");
    printf("8. Refresh GPA for a student\n");
    printf("9. Print grade report\n");
    printf("10. Exit\n");
}

void load_student_data(const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (!file) {
        printf("Failed to open file %s.\n", file_path);
        return;
    }
    total_students = 0;
    while (fscanf(file, "%d %s %s %d", &student_list[total_students].id, student_list[total_students].full_name,
                  student_list[total_students].department, &student_list[total_students].num_courses) == 4) {
        for (int i = 0; i < student_list[total_students].num_courses; i++) {
            fscanf(file, "%s %d %c", student_list[total_students].enrolled_courses[i].name,
                   &student_list[total_students].enrolled_courses[i].credits, &student_list[total_students].enrolled_courses[i].grade);
        }
        update_gpa(&student_list[total_students]);
        total_students++;
        if (total_students >= MAX_STUDENTS) {
            printf("Student limit reached.\n");
            break;
        }
    }
    fclose(file);
}

void save_student_data(const char *file_path) {
    FILE *file = fopen(file_path, "w");
    if (!file) {
        printf("Failed to open file %s for writing.\n", file_path);
        return;
    }
    for (int i = 0; i < total_students; i++) {
        fprintf(file, "%d, %s, %s, %d ", student_list[i].id, student_list[i].full_name, student_list[i].department, student_list[i].num_courses);
        for (int j = 0; j < student_list[i].num_courses; j++) {
            fprintf(file, "%s, %d, %c ", student_list[i].enrolled_courses[j].name, student_list[i].enrolled_courses[j].credits, student_list[i].enrolled_courses[j].grade);
        }
        fprintf(file, "\n");
    }
    fclose(file);
    printf("Student data saved to file %s.\n", file_path);
}

void append_student_data(const char *file_path, Student *stu) {
    FILE *file = fopen(file_path, "a");
    if (!file) {
        printf("Failed to open file %s for appending.\n", file_path);
        return;
    }
    fprintf(file, "%d, %s, %s, %d,", stu->id, stu->full_name, stu->department, stu->num_courses);
    for (int i = 0; i < stu->num_courses; i++) {
        fprintf(file, "%s, %d, %c,", stu->enrolled_courses[i].name, stu->enrolled_courses[i].credits, stu->enrolled_courses[i].grade);
    }
    fprintf(file, "\n");
    fclose(file);
    printf("Student data appended to file %s.\n", file_path);
}

void remove_student_data(const char *file_path, int id) {
    FILE *file = fopen(file_path, "r");
    if (!file) {
        printf("Failed to open file %s.\n", file_path);
        return;
    }

    FILE *temp_file = fopen("temp.txt", "w");
    if (!temp_file) {
        fclose(file);
        printf("Failed to create temporary file.\n");
        return;
    }

    int found = 0;
    char line[256];

    while (fgets(line, sizeof(line), file)) {
        int current_id;
        sscanf(line, "%d", &current_id);
        if (current_id == id) {
            found = 1;
            continue;
        }
        fputs(line, temp_file);
    }

    fclose(file);
    fclose(temp_file);

    if (found) {
        remove(file_path);
        rename("temp.txt", file_path);
        printf("Student with ID %d removed from file.\n", id);
    } else {
        remove("temp.txt");
        printf("Student with ID %d not found in file.\n", id);
    }
}

int main() {
    int choice;
    const char *file_path = DATA_FILE;

    load_student_data(file_path);

    do {
        display_main_menu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                add_new_student();
                append_student_data(file_path, &student_list[total_students - 1]);
                break;
            case 2:
                initialize_gpa_for_all();
                break;
            case 3: {
                int id;
                char course_name[10];
                printf("Enter student ID: ");
                scanf("%d", &id);
                printf("Enter course name: ");
                scanf("%s", course_name);
                remove_course(id, course_name);
                save_student_data(file_path);
                break;
            }
            case 4: {
                int id;
                char course_name[10];
                int credits;
                char grade;
                printf("Enter student ID: ");
                scanf("%d", &id);
                printf("Enter course name: ");
                scanf("%s", course_name);
                printf("Enter credits: ");
                scanf("%d", &credits);
                printf("Enter grade: ");
                scanf(" %c", &grade);
                add_course(id, course_name, credits, grade);
                save_student_data(file_path);
                break;
            }
            case 5: {
                int id;
                char old_name[10], new_name[10];
                printf("Enter student ID: ");
                scanf("%d", &id);
                printf("Enter old course name: ");
                scanf("%s", old_name);
                printf("Enter new course name: ");
                scanf("%s", new_name);
                change_course_name(id, old_name, new_name);
                save_student_data(file_path);
                break;
            }
            case 6:
                initialize_gpa_for_all();
                break;
            case 7: {
                char grade;
                int points;
                printf("Enter grade to upgrade: ");
                scanf(" %c", &grade);
                printf("Enter new points: ");
                scanf("%d", &points);
                adjust_grades(grade, points);
                save_student_data(file_path);
                break;
            }
            case 8: {
                int id;
                printf("Enter student ID: ");
                scanf("%d", &id);
                refresh_student_gpa(id);
                save_student_data(file_path);
                break;
            }
            case 9: {
                int id;
                printf("Enter student ID: ");
                scanf("%d", &id);
                print_grade_report(id);
                break;
            }
            case 10:
                printf("Exiting the program...\n");
                break;
            default:
                printf("Invalid selection. Please try again.\n");
        }
    } while (choice != 10);

    return 0;
}
