#include <check.h>

#include "vector_test.h"
#include "token_test.h"

int main(void)
{
    int no_faild = 0;
    Suite *s;
    SRunner *runner;

    s = vector_suite();
    runner = srunner_create(s);

    srunner_run_all(runner, CK_NORMAL);
    no_faild = srunner_ntests_failed(runner);
    srunner_free(runner);

     s = token_suite();
    runner = srunner_create(s);

    srunner_run_all(runner, CK_NORMAL);
    no_faild += srunner_ntests_failed(runner);
    srunner_free(runner);

    return (no_faild == 0) ? 0 : -1;
}