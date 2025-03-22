#include "unity.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../../examples/autotest-validate/autotest-validate.h"
#include "../../assignment-autotest/test/assignment1/username-from-conf-file.h"

void test_validate_my_username()
{
    // 取得 `my_username()` 回傳的硬編碼使用者名稱
    const char* expected_username = my_username();

    // 取得 `malloc_username_from_conf_file()` 回傳的動態配置的使用者名稱
    char* actual_username = malloc_username_from_conf_file();

    // 確保動態配置的字串不是 NULL
    TEST_ASSERT_NOT_NULL_MESSAGE(actual_username, "malloc_username_from_conf_file() 回傳了 NULL");

    // 檢查兩者是否相等
    TEST_ASSERT_EQUAL_STRING_MESSAGE(expected_username, actual_username, "使用者名稱不匹配");

    // 釋放 `malloc_username_from_conf_file()` 配置的記憶體
    free(actual_username);
}

