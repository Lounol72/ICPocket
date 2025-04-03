#include "../include/save.h"
#include <check.h>

t_Team teamJ1;
t_Team teamJ2;

void setup(void) {   
    initTeam(&teamJ1, 1);
    initBlueTeam(&teamJ2, &teamJ1);
}

void teardown(void) {
    // vide car tout est statique
}

START_TEST(test_sauvegarder) {
    sauvegarder(&teamJ1, &teamJ2);
    // Vérifiez que le fichier de sauvegarde a été créé avec succès
    FILE *fichier = fopen("data/save/Save_1.txt", "r");
    ck_assert_ptr_nonnull(fichier);
    fclose(fichier);
}END_TEST
START_TEST(test_charger) {
    // Vérifiez que le fichier de sauvegarde ne peut charger correctement
    int result = charger("2", &teamJ1, &teamJ2);
    ck_assert_int_eq(result, -1);
}END_TEST
START_TEST(test_sauvegarder_charger) {
    // Vérifiez que le fichier de sauvegarde peut charger correctement
    teamJ1.nb_poke = 1;
    teamJ1.team[0].id = 1;
    teamJ1.team[0].lvl = 1;
    teamJ1.team[0].nature = 1;
    teamJ1.team[0].nb_move = 1;
    for(int i = 0; i < 6; i++) teamJ1.team[0].iv[i] = 1;
    for(int i = 0; i < 2; i++) teamJ1.team[0].type[i] = 1;
    teamJ1.team[0].moveList[0].id = 1;
    teamJ1.nb_enemiBeat = 1;
    strcpy(teamJ1.trainerName, "test");
    teamJ1.id_save = 1;
    teamJ2.id = 2;
    sauvegarder(&teamJ1, &teamJ2);
    int result = charger("1", &teamJ1, &teamJ2);
    ck_assert_int_eq(result, 1);
}END_TEST

Suite *save_suite(void) {
    Suite *s = suite_create("Save");
    TCase *tc_core = tcase_create("Core");
    tcase_add_checked_fixture(tc_core, setup, teardown);
    tcase_add_test(tc_core, test_sauvegarder);
    tcase_add_test(tc_core, test_charger);
    tcase_add_test(tc_core, test_sauvegarder_charger);
    suite_add_tcase(s, tc_core);
    return s;
}

int main(void) {
    Suite *s = save_suite();
    SRunner *sr = srunner_create(s);
    
    srunner_set_fork_status(sr, CK_NOFORK);
    
    srunner_run_all(sr, CK_NORMAL);
    
    int failed = srunner_ntests_failed(sr);
    int total = srunner_ntests_run(sr);
    
    //Pourcentage de réussite
    double success_rate = (total - failed) * 100.0 / total;
    
    int threshold_passed = (success_rate >= 80.0);
    
    //Message selon le résultat
    if (threshold_passed) {
        printf("✅ Test de Save : OK\n");
    } else {
        printf("❌ Test de Save : KO\n (%.2f%% des tests réussis)\n", success_rate);
    }
    
    srunner_free(sr);
    return (threshold_passed) ? 0 : 1;
}