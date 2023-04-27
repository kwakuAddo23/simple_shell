#include "shell.h"
#include <string.h>

/**
 * _strlen - returns the length of a string
 * @s: the string whose length to check
 *
 * Return: integer length of string
 */
int _strlen(const char *s)
{
    int i = 0;

    if (!s)
        return (0);

    while (*s++)
        i++;
    return (i);
}

/**
 * _strcmp - performs lexicographic comparison of two strings.
 * @s1: the first string
 * @s2: the second string
 *
 * Return: negative if s1 < s2, positive if s1 > s2, zero if s1 == s2
 */
int _strcmp(const char *s1, const char *s2)
{
    return strncmp(s1, s2, _strlen(s2));
}

/**
 * starts_with - checks if needle starts with haystack
 * @haystack: string to search
 * @needle: the substring to find
 *
 * Return: address of next char of haystack or NULL
 */
const char *starts_with(const char *haystack, const char *needle)
{
    int len = _strlen(needle);
    if (strncmp(haystack, needle, len) == 0)
        return haystack + len;
    return NULL;
}

