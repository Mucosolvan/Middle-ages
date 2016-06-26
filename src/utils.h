#ifndef UTILS_H
#define UTILS_H

/* If this is being built for a unit test. */
#ifdef UNIT_TESTING

/* Redirect calloc and free to test_calloc() and test_free() so cmocka can
 * check for memory leaks. */
#ifdef calloc
#undef calloc
#endif /* calloc */
#define calloc(num, size) _test_calloc(num, size, __FILE__, __LINE__)
#ifdef free
#undef free
#endif /* free */
#define free(ptr) _test_free(ptr, __FILE__, __LINE__)
void* _test_calloc(const size_t number_of_elements, const size_t size,
                   const char* file, const int line);
void _test_free(void* const ptr, const char* file, const int line);

#ifdef fgets
#undef fgets
#endif /* fgets */
#define fgets(ptr, size, stream) mock_fgets(ptr, size, stream)
extern char* mock_fgets(char *s, int size, FILE *stream);
#ifdef malloc
#undef malloc
#endif /* malloc */
#define malloc(size) _test_malloc(size, __FILE__, __LINE__)
void* _test_malloc(const size_t size, const char* file, const int line);

int middle_ages_main();
/* Function main is defined in the unit test so redefine name of the main function
 * here. */
#define main middle_ages_main

/* All functions in this object need to be exposed to the test application,
 * so redefine static to nothing. */
#define static

#endif /* UNIT_TESTING */

#endif /* UTILS_H*/
