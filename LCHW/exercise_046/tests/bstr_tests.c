#include "minunit.h"
#include <lcthw/dbg.h>
#include <lcthw/bstrlib.h>

char *cs_0 = "Hello World!";
char *cs_1 = "Learn C the Hardway!";

#define print_bstr(a) {debug("mlen %d slen %d %p: [%s]", (a)->mlen, (a)->slen, (a)->data, (a)->data);}

#define print_bstrl(a) {debug("mlen %d qty %d %p", (a)->mlen, (a)->qty, (a)->entry);}

char *test_bstring()
{
    bstring bs_0 = bfromcstr(cs_0);
    bstring bs_1 = blk2bstr(cs_0, 12);
    bstring bs_2 = bstrcpy(bs_0);
    bstring bs_3 = bformat("%s", cs_1);
    struct bstrList *bsl_0 = bsplit(bs_3, ' ');
    print_bstr(bs_0);
    print_bstr(bs_1);
    print_bstr(bs_2);
    print_bstr(bs_3);
    print_bstrl(bsl_0);
    
    mu_assert(bstricmp(bs_0, bs_2) == 0, "Failed for comparing");
    mu_assert(biseq(bs_0, bs_2), "Failed for comparing");
    mu_assert(binstr(bs_0, 0, bs_2) >= 0, "Failed to check string in");
    bfindreplace(bs_2, bs_1, bs_3, 0);
    print_bstr(bs_2);
    print_bstr(bsl_0->entry[0]);
    bfindreplace(bs_2, bsl_0->entry[0], bs_3, 0);
    print_bstr(bs_2);
    debug("len:%d data:%s char:%c", blength(bs_2), bdata(bs_2), bchar(bs_2, 1));
    
    bdestroy(bs_0);
    bdestroy(bs_1);
    bdestroy(bs_2);
    bdestroy(bs_3);
    bstrListDestroy(bsl_0);
    return NULL;
}

char *all_tests()
{
    mu_suite_start();
    mu_run_test(test_bstring);
    return NULL;
}

RUN_TESTS(all_tests);
