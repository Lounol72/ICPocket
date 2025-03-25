#include <check.h>
#include "../include/Map.h"


SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

void setup(void) {
    window = SDL_CreateWindow("Test Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
}

void teardown(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

// Test pour la fonction generatemap
START_TEST(test_generateMap) {
    setenv("SDL_VIDEODRIVER", "dummy", 1);
    
    int spawnX = 0;
    int spawnY = 0;
    Map *map = initMap(renderer, "assets/Tileset/Map/2.png", 16, 16, &spawnX, &spawnY);
    if (map != NULL) {
        ck_assert_int_eq(map->width, 16);
        ck_assert_int_eq(map->height, 16);
        destroyMap(map);
    }else{
        ck_assert_msg(map != NULL, "Map initialization failed");
    }
}
END_TEST

// Création de la suite de tests
Suite *map_suite(void) {
    Suite *s = suite_create("Map");
    
    // Create a test case instead of using tc_core as a function
    TCase *tc_core = tcase_create("Core");
    
    tcase_add_checked_fixture(tc_core, setup, teardown);
    tcase_add_test(tc_core, test_generateMap);
    // Add the test case to the suite
    suite_add_tcase(s, tc_core);
    
    return s;
}

// Point d'entrée principal
int main(void) {
    Suite *s = map_suite();
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
        printf("✅ Test de Map : OK\n");
    } else {
        printf("❌ Test de Map : KO\n (%.2f%% des tests réussis)\n", success_rate);
    }
    
    srunner_free(sr);
    return (threshold_passed) ? 0 : 1;
}