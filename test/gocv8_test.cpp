#include <boost/test/unit_test.hpp>

#include "gocv8/gocv8.h"

const static char* EVENT_JSON = "{\"a_field\":\"TROLOLOL I'm an Event!!!\"}";
const static char* EVENT_JS = "event_data.data";

BOOST_AUTO_TEST_SUITE(_gocv8)

BOOST_AUTO_TEST_CASE(new_free)
{
    void* ctx = NULL;
    ctx = gocv8_context_new();
    BOOST_CHECK(NULL != ctx);
    gocv8_context_free(ctx);
}

BOOST_AUTO_TEST_CASE(execute_js)
{
    void* ctx = NULL;
    void* status = NULL;
    ctx = gocv8_context_new();
    BOOST_CHECK(NULL != ctx);

    bool result = gocv8_process_event(
        ctx,
        EVENT_JSON,
        strlen(EVENT_JSON),
        EVENT_JS,
        strlen(EVENT_JS),
        &status);

    gocv8_context_free(ctx);
}

BOOST_AUTO_TEST_SUITE_END()
