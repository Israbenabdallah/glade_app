#include "memberstats.h"
#include <string.h>
#include <stdio.h>

MemberStats load_member_stats(const char *filename, const char *id) {
    MemberStats stats;

    // default values
    stats.num_classes_attended = 0;
    strcpy(stats.favorite_class, "");
    strcpy(stats.coach_name, "");
    stats.performance = 0;
    stats.consistency_score = 0;

    // optional: read from file
    FILE *file = fopen(filename, "r");
    if (file) {
        char line[256];
        while (fgets(line, sizeof(line), file)) {
            char file_id[50];
            sscanf(line, "%49s %d %49s %49s %d %d",
                   file_id,
                   &stats.num_classes_attended,
                   stats.favorite_class,
                   stats.coach_name,
                   &stats.performance,
                   &stats.consistency_score);
            if (strcmp(file_id, id) == 0) {
                fclose(file);
                return stats;
            }
        }
        fclose(file);
    }

    return stats;
}

void save_member_stats(const char *filename, const char *id, MemberStats stats) {
    // minimal stub for now
    printf("Saving stats for member %s\n", id);
}


