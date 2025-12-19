
  #include <stdio.h>
#include <string.h>
#include "Member.h"

// Add a new member to the file
int ajouter_member(char *filename, Member m) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        return -1;  // Error opening file
    }

    fprintf(file, "%s|%s|%s|%d|%d|%s|%s|%s|%d|%d|%d\n",
            m.id_member, m.first_name, m.last_name, m.age, m.gender,
            m.phone, m.email, m.subscription_type,
            m.private_coach, m.request_show_stats, m.join_course);

    fclose(file);
    return 1;  // Success
}

// Modify an existing member
int modifier_member(char *filename, char *id, Member nouv) {
    FILE *file = fopen(filename, "r");
    FILE *temp = fopen("temp_member.txt", "w");

    if (file == NULL || temp == NULL) {
        return -1;
    }

    Member m;
    char line[500];
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^|]|%[^|]|%[^|]|%d|%d|%[^|]|%[^|]|%[^|]|%d|%d|%d",
               m.id_member, m.first_name, m.last_name, &m.age, &m.gender,
               m.phone, m.email, m.subscription_type,
               &m.private_coach, &m.request_show_stats, &m.join_course);

        if (strcmp(m.id_member, id) == 0) {
            fprintf(temp, "%s|%s|%s|%d|%d|%s|%s|%s|%d|%d|%d\n",
                    nouv.id_member, nouv.first_name, nouv.last_name, nouv.age, nouv.gender,
                    nouv.phone, nouv.email, nouv.subscription_type,
                    nouv.private_coach, nouv.request_show_stats, nouv.join_course);
            found = 1;
        } else {
            fputs(line, temp);
        }
    }

    fclose(file);
    fclose(temp);

    if (found) {
        remove(filename);
        rename("temp_member.txt", filename);
        return 1;
    }

    remove("temp_member.txt");
    return -1;  // Member not found
}

// Delete a member
int supprimer_member(char *filename, char *id) {
    FILE *file = fopen(filename, "r");
    FILE *temp = fopen("temp_member.txt", "w");

    if (file == NULL || temp == NULL) {
        return -1;
    }

    Member m;
    char line[500];
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^|]|%[^|]|%[^|]|%d|%d|%[^|]|%[^|]|%[^|]|%d|%d|%d",
               m.id_member, m.first_name, m.last_name, &m.age, &m.gender,
               m.phone, m.email, m.subscription_type,
               &m.private_coach, &m.request_show_stats, &m.join_course);

        if (strcmp(m.id_member, id) == 0) {
            found = 1;
        } else {
            fputs(line, temp);
        }
    }

    fclose(file);
    fclose(temp);

    if (found) {
        remove(filename);
        rename("temp_member.txt", filename);
        return 1;
    }

    remove("temp_member.txt");
    return -1;
}

// Search for a member
Member chercher_member(char *filename, char *id) {
    FILE *file = fopen(filename, "r");
    Member m;
    strcpy(m.id_member, "");

    if (file == NULL) {
        return m;
    }

    char line[500];
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^|]|%[^|]|%[^|]|%d|%d|%[^|]|%[^|]|%[^|]|%d|%d|%d",
               m.id_member, m.first_name, m.last_name, &m.age, &m.gender,
               m.phone, m.email, m.subscription_type,
               &m.private_coach, &m.request_show_stats, &m.join_course);

        if (strcmp(m.id_member, id) == 0) {
            fclose(file);
            return m;
        }
    }

    fclose(file);
    strcpy(m.id_member, "");
    return m;
}

// Event: private coach request
void handle_private_coach_request(char *filename, char *id_member) {
    FILE *file = fopen(filename, "a");
    if (file != NULL) {
        fprintf(file, "[EVENT] Private coach request for member: %s\n", id_member);
        fclose(file);
    }
}

// Event: show stats
void handle_show_stats(char *filename, char *id_member) {
    FILE *file = fopen(filename, "a");
    if (file != NULL) {
        fprintf(file, "[EVENT] Member %s requested stats display\n", id_member);
        fclose(file);
    }
}

// Event: join course
void handle_join_course(char *filename, char *id_member) {
    FILE *file = fopen(filename, "a");
    if (file != NULL) {
        fprintf(file, "[EVENT] Member %s joined a course\n", id_member);
        fclose(file);
    }
}



