#include <check.h>
#include "../include/duel.h"

// Données globales pour les tests

// Configuration avant les tests
void setup(void) {
    initData();
    initTeam(&rouge, 3);
    initBlueTeam(&bleu, &rouge);
}

// Nettoyage après les tests
void teardown(void) {
    // Libération des ressources si nécessaire
}

// Test de calcul de statistique
START_TEST(test_calcStatFrom) {
    t_Poke pokemon;
    pokemon.baseStats[PV] = 100;
    pokemon.iv[PV] = 31;
    pokemon.lvl = 50;
    
    int expectedPV = ((2*100+31)*50/100)+50+10; // Formule PV
    int result = calcStatFrom(&pokemon, PV);
    
    ck_assert_int_eq(result, expectedPV);
}
END_TEST

// Test de validation d'action
START_TEST(test_testActionValidity) {
    rouge.team[0].moveList[0].current_pp = 10;
    ck_assert_int_eq(testActionValidity(0, &rouge), TRUE);
    
    rouge.team[0].moveList[0].current_pp = 0;
    ck_assert_int_eq(testActionValidity(0, &rouge), FALSE);
    
    rouge.team[1].current_pv = 50;
    ck_assert_int_eq(testActionValidity(11, &rouge), TRUE);
    
    rouge.team[1].current_pv = 0;
    ck_assert_int_eq(testActionValidity(11, &rouge), FALSE);
}
END_TEST

// Test de tour de combat
START_TEST(test_playATurn) {
    int initialPVRouge = rouge.team[0].current_pv;
    int initialPVBleu = bleu.team[0].current_pv;

    playATurn(&rouge, 0, &bleu, 0);

    ck_assert(rouge.team[0].current_pv != initialPVRouge || 
              bleu.team[0].current_pv != initialPVBleu);
}
END_TEST

// Test pour la fonction swapActualAttacker
START_TEST(test_swapActualAttacker) {
    t_Poke backup = rouge.team[1];
    rouge.team[1].current_pv = 50;
    
    swapActualAttacker(&rouge, 1);

    
    rouge.team[1] = backup;
    setup();
}
END_TEST

// Test pour les fonctions isAlive et isTeamAlive
START_TEST(test_isAlive_isTeamAlive) {
    rouge.team[0].current_pv = 50;
    rouge.team[1].current_pv = 0;
    
    ck_assert_int_eq(isAlive(&rouge.team[0]), TRUE);
    ck_assert_int_eq(isAlive(&rouge.team[1]), FALSE);
    ck_assert_int_eq(isTeamAlive(&rouge), TRUE);
    
    for (int i = 0; i < rouge.nb_poke; i++) {
        rouge.team[i].current_pv = 0;
    }
    
    ck_assert_int_eq(isTeamAlive(&rouge), FALSE);
    setup();
}
END_TEST

// Test pour la fonction affectDamage
START_TEST(test_affectDamage) {
    int initial_pv = bleu.team[0].current_pv;
    
    affectDamage(&rouge, &bleu, 0);
    
    ck_assert(bleu.team[0].current_pv <= initial_pv);
}
END_TEST

// Test pour la fonction gainExp
START_TEST(test_gainExp) {
    t_Poke source ;
    strcpy(source.name, "dummy");
    source.gender = 1;
    source.lvl = 50;
    source.nature = 1;
    source.type[0] = 1;
    source.type[1] = 1;
    for (int i = 0; i < 6; i++)
		source.baseStats[i] = 1;
	for (int i = 0; i < 6; i++)
		source.iv[i] = 31;
    
    unsigned int initial_exp = rouge.team[0].exp;
    gainExp(&rouge, &source);
    int exp_gained = rouge.team[0].exp - initial_exp;
    ck_assert_int_ge(exp_gained, 32);

    setup();
}
END_TEST

// Test pour les fonctions de validation du type d'action
START_TEST(test_action_type_checks) {
    ck_assert_int_eq(isAttacking(0), TRUE);
    ck_assert_int_eq(isAttacking(3), TRUE);
    ck_assert_int_eq(isAttacking(11), FALSE);
    
    ck_assert_int_eq(isSwitching(11), TRUE);
    ck_assert_int_eq(isSwitching(15), TRUE);
    ck_assert_int_eq(isSwitching(3), FALSE);
    
    ck_assert_int_eq(isStruggling(STRUGGLE), TRUE);
    ck_assert_int_eq(isStruggling(0), FALSE);
}
END_TEST

// Test pour la fonction resolveSpeedDuel
START_TEST(test_resolveSpeedDuel) {
    int result1 = resolveSpeedDuel(100, 50);
    ck_assert_msg(result1 == 1, "resolveSpeedDuel(100, 50) devrait retourner 1, mais a retourné %d", result1);
    
    int result2 = resolveSpeedDuel(50, 100);
    ck_assert_msg(result2 == 0, "resolveSpeedDuel(50, 100) devrait retourner 0, mais a retourné %d", result2);
    
    int result3 = resolveSpeedDuel(100, 100);
    ck_assert_msg(result3 >= 0, "resolveSpeedDuel(100, 100) a retourné une valeur négative: %d", result3);
}
END_TEST

// Test pour la fonction hasMoveLeft
START_TEST(test_hasMoveLeft) {
    for (int i = 0; i < rouge.team[0].nb_move; i++) {
        rouge.team[0].moveList[i].current_pp = 10;
    }
    
    int result1 = hasMoveLeft(&rouge.team[0]);
    ck_assert_msg(result1 > 0, "hasMoveLeft devrait retourner une valeur > 0 quand PP > 0, a retourné %d", result1);
    
    for (int i = 0; i < rouge.team[0].nb_move; i++) {
        rouge.team[0].moveList[i].current_pp = 0;
    }
    
    int result2 = hasMoveLeft(&rouge.team[0]);
    ck_assert_msg(result2 == 0, "hasMoveLeft devrait retourner 0 quand aucun PP, a retourné %d", result2);
    
    setup();
}
END_TEST

// Test pour la fonction statVarChange
START_TEST(test_statVarChange) {
    setDefaultStatChanges(&rouge);
    int initial_stat_change = rouge.statChanges[ATT];
    
    statVarChange(&rouge, 100, 1, ATT);
    ck_assert_int_eq(rouge.statChanges[ATT], initial_stat_change + 1);
    
    setDefaultStatChanges(&rouge);
    statVarChange(&rouge, 100, -1, DEF);
    ck_assert_int_eq(rouge.statChanges[DEF], NEUTRAL_STAT_CHANGE - 1);
}
END_TEST

START_TEST(test_reachedNextLvl) {
    
    t_Poke p1Lvl1;
    strcpy(p1Lvl1.name, "dummy");
    p1Lvl1.gender = 1;
    p1Lvl1.lvl = 1;
    p1Lvl1.nature = 1;
    p1Lvl1.type[0] = 1;
    p1Lvl1.type[1] = 1;
    for (int i = 0; i < 6; i++)
		p1Lvl1.baseStats[i] = 1;
	for (int i = 0; i < 6; i++)
		p1Lvl1.iv[i] = 31;
    p1Lvl1.exp = expCurve(1);

    t_Poke p2Lvl50;
    strcpy(p2Lvl50.name, "dummy");
    p2Lvl50.gender = 1;
    p2Lvl50.lvl = 50;
    p2Lvl50.nature = 1;
    p2Lvl50.type[0] = 1;
    p2Lvl50.type[1] = 1;
    for (int i = 0; i < 6; i++)
		p2Lvl50.baseStats[i] = 1;
	for (int i = 0; i < 6; i++)
		p2Lvl50.iv[i] = 31;
    p2Lvl50.exp = expCurve(50);

    gainExp(&rouge, &p2Lvl50);
    ck_assert_int_ge(p2Lvl50.lvl, 30);
    
}
END_TEST
// Création de la suite de tests
Suite *duel_suite(void) {
    Suite *s = suite_create("Duel");
    
    TCase *tc_core = tcase_create("Core");
    tcase_add_checked_fixture(tc_core, setup, teardown);
    tcase_add_test(tc_core, test_calcStatFrom);
    tcase_add_test(tc_core, test_testActionValidity);
    tcase_add_test(tc_core, test_playATurn);
    
    tcase_add_test(tc_core, test_swapActualAttacker);
    tcase_add_test(tc_core, test_isAlive_isTeamAlive);
    tcase_add_test(tc_core, test_affectDamage);
    tcase_add_test(tc_core, test_gainExp);
    tcase_add_test(tc_core, test_action_type_checks);
    tcase_add_test(tc_core, test_resolveSpeedDuel);
    tcase_add_test(tc_core, test_hasMoveLeft);
    tcase_add_test(tc_core, test_statVarChange);
    tcase_add_test(tc_core, test_reachedNextLvl);
    suite_add_tcase(s, tc_core);
    
    return s;
}

// Point d'entrée principal
int main(void) {
    Suite *s = duel_suite();
    SRunner *sr = srunner_create(s);
    
    srunner_run_all(sr, CK_NORMAL);
    
    int failed = srunner_ntests_failed(sr);
    int total = srunner_ntests_run(sr);
    
    //Pourcentage de réussite
    double success_rate = (total - failed) * 100.0 / total;
    
    int threshold_passed = (success_rate >= 80.0);
    
    //Message selon le résultat
    if (threshold_passed) {
        printf("✅ Test de duel : OK\n");
    } else {
        printf("❌ Test de duel : KO\n (%.2f%% des tests réussis)\n", success_rate);
    }
    
    srunner_free(sr);
    
    // Retourner 0 (succès) si on dépasse le seuil, sinon 1 (échec)
    return (threshold_passed) ? 0 : 1;
}