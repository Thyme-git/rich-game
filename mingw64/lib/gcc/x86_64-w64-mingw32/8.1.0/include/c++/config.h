// Copyright (C) 2018 Martin Weigel <mail@MartinWeigel.com>
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

// Version: 2018-12-01

#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>

// Internal type definition. Please use macros to create ConfigItems.
typedef struct ConfigItem ConfigItem;
typedef enum ConfigType ConfigType;


//******************************************************************************
// PUBLIC INTERFACE
//******************************************************************************
/**
 * Parses the input file. If the key of a line matches a ConfigItem, its value
 * pointer is set to the data inside the config file.
 * For CONFIG_CALLBACK, a function is called (see Config_callback).
 * For CONFIG_NEW_STRING, new memory will be allocated for the value.
 * @filepath:
 *      Path to the configuration file.
 * @items:
 *      Array of config items. All items should be created using macros.
 */
void Config_load(const char* filepath, ConfigItem* items);

/**
 * Callback definition for CONFIG_CALLBACK(key, callback).
 * @sender:
 *      Pointer to a user-specified object.
 * @key:
 *      The key of the read config.
 * @value:
 *      The value of the read config.
 */
typedef void Config_callback(void* sender, const char* key, const char* value);

/*
 * Macro definitions to create config items
 */
#define CONFIG_END()                            { CONFIGTYPE_END, NULL, NULL, NULL, 0, false }
#define CONFIG_BOOL(key, valuePtr)              { CONFIGTYPE_BOOL, key, valuePtr, NULL, 0, false }
#define CONFIG_INT(key, valuePtr)               { CONFIGTYPE_INT, key, valuePtr, NULL, 0, false }
#define CONFIG_INT8(key, valuePtr)              { CONFIGTYPE_INT8, key, valuePtr, NULL, 0, false }
#define CONFIG_INT16(key, valuePtr)             { CONFIGTYPE_INT16, key, valuePtr, NULL, 0, false }
#define CONFIG_INT32(key, valuePtr)             { CONFIGTYPE_INT32, key, valuePtr, NULL, 0, false }
#define CONFIG_INT64(key, valuePtr)             { CONFIGTYPE_INT64, key, valuePtr, NULL, 0, false }
#define CONFIG_UINT8(key, valuePtr)             { CONFIGTYPE_UINT8, key, valuePtr, NULL, 0, false }
#define CONFIG_UINT16(key, valuePtr)            { CONFIGTYPE_UINT16, key, valuePtr, NULL, 0, false }
#define CONFIG_UINT32(key, valuePtr)            { CONFIGTYPE_UINT32, key, valuePtr, NULL, 0, false }
#define CONFIG_UINT64(key, valuePtr)            { CONFIGTYPE_UINT64, key, valuePtr, NULL, 0, false }
#define CONFIG_DOUBLE(key, valuePtr)            { CONFIGTYPE_DOUBLE, key, valuePtr, NULL, 0, false }
#define CONFIG_STRING(key, valuePtr, size)      { CONFIGTYPE_STRING, key, valuePtr, NULL, size, false }
#define CONFIG_NEW_STRING(key, valuePtr)        { CONFIGTYPE_NEW_STRING, key, valuePtr, NULL, 0, false }
#define CONFIG_CALLBACK(key, callback, sender)  { CONFIGTYPE_CALLBACK, key, sender, callback, 0, false }



//******************************************************************************
// Internal definition of data structures
//******************************************************************************
typedef enum ConfigType
{
    CONFIGTYPE_END,
    CONFIGTYPE_INT,
    CONFIGTYPE_INT8,
    CONFIGTYPE_INT16,
    CONFIGTYPE_INT32,
    CONFIGTYPE_INT64,
    CONFIGTYPE_BOOL,
    CONFIGTYPE_UINT8,
    CONFIGTYPE_UINT16,
    CONFIGTYPE_UINT32,
    CONFIGTYPE_UINT64,
    CONFIGTYPE_DOUBLE,
    CONFIGTYPE_STRING,
    CONFIGTYPE_NEW_STRING,
    CONFIGTYPE_CALLBACK
} ConfigType;

typedef struct ConfigItem
{
    ConfigType type;
    char* key;
    void *pointer;
    void *callback;
    size_t stringSize;
    bool isSet;
} ConfigItem;



//******************************************************************************
// Implementation section. Include using:
// #define CONFIG_IMPLEMENTATION
// #include "Config.h"
//******************************************************************************
#ifdef CONFIG_IMPLEMENTATION

#ifdef CONFIG_VERBOSE
    // Activate all verbose messages
    #define CONFIG_VERBOSE_UNUSED
    #define CONFIG_VERBOSE_MISSING
    #define CONFIG_VERBOSE_DUPLICATE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

const int CONFIG_BUFFERSIZE = 256;
const char CONFIG_EQUALSIGN = '=';
const char CONFIG_COMMENTSIGN = '#';

// SET EXIT MACRO
#ifdef CONFIG_NO_EXIT_ON_ERROR
    #define CONFIG_EXIT() (return false)
#else
    #define CONFIG_EXIT() (exit(EXIT_FAILURE))
#endif

// Returns if value is an int in bounds and saves value to output
bool CONFIG_IntFromString(ConfigItem* item, char* string, int64_t min, int64_t max, int64_t* output)
{
    errno = 0;
    char* lastParsedChar = NULL;
    long long longValue = strtoll(string, &lastParsedChar, 10);
    // Check for errors
    if(*lastParsedChar != '\0') {
        printf("[CONFIG] Value for key '%s' is not a number: %s\n", item->key, string);
        CONFIG_EXIT();
    }
    if(errno == ERANGE || longValue < min || longValue > max) {
        printf("[CONFIG] Value for key '%s' is out of bounds [%"PRId64", %"PRId64"]: %s\n", item->key, min, max, string);
        CONFIG_EXIT();
    }
    *output = longValue;
    return true;
}

// Returns if value is an uint in bounds and saves value to output
bool CONFIG_uIntFromString(ConfigItem* item, char* string, uint64_t max, uint64_t* output)
{
    errno = 0;
    char* lastParsedChar = NULL;
    unsigned long long longValue = strtoull(string, &lastParsedChar, 10);
    // Check for errors
    if(*lastParsedChar != '\0') {
        printf("[CONFIG] Value for key '%s' is not a number: %s\n", item->key, string);
        CONFIG_EXIT();
    }
    if(errno == ERANGE || string[0] == '-' || longValue > max) {
        printf("[CONFIG] Value for key '%s' is out of bounds [0, %"PRIu64"]: %s\n", item->key, max, string);
        CONFIG_EXIT();
    }
    *output = longValue;
    return true;
}

void Config_handleItem(ConfigItem* item, char* value)
{
    int64_t sResult;
    uint64_t uResult;
    char* lastParsedChar = NULL;
    switch(item->type) {
        case CONFIGTYPE_INT:
            if(CONFIG_IntFromString(item, value, INT_MIN, INT_MAX, &sResult)) {
                int* output = item->pointer;
                *output = sResult;
            }
            break;
        case CONFIGTYPE_INT8:
            if(CONFIG_IntFromString(item, value, INT8_MIN, INT8_MAX, &sResult)) {
                int8_t* output = item->pointer;
                *output = sResult;
            }
            break;
        case CONFIGTYPE_INT16:
            if(CONFIG_IntFromString(item, value, INT16_MIN, INT16_MAX, &sResult)) {
                int16_t* output = item->pointer;
                *output = sResult;
            }
            break;
        case CONFIGTYPE_INT32:
            if(CONFIG_IntFromString(item, value, INT32_MIN, INT32_MAX, &sResult)) {
                int32_t* output = item->pointer;
                *output = sResult;
            }
            break;
        case CONFIGTYPE_INT64:
            if(CONFIG_IntFromString(item, value, INT64_MIN, INT64_MAX, &sResult)) {
                int64_t* output = item->pointer;
                *output = sResult;
            }
            break;
        case CONFIGTYPE_BOOL:
            if(CONFIG_uIntFromString(item, value, 1, &uResult)) {
                bool* output = item->pointer;
                *output = uResult;
            }
            break;
        case CONFIGTYPE_UINT8:
            if(CONFIG_uIntFromString(item, value, UINT8_MAX, &uResult)) {
                uint8_t* output = item->pointer;
                *output = uResult;
            }
            break;
        case CONFIGTYPE_UINT16:
            if(CONFIG_uIntFromString(item, value, UINT16_MAX, &uResult)) {
                uint16_t* output = item->pointer;
                *output = uResult;
            }
            break;
        case CONFIGTYPE_UINT32:
            if(CONFIG_uIntFromString(item, value, UINT32_MAX, &uResult)) {
                uint32_t* output = item->pointer;
                *output = uResult;
            }
            break;
        case CONFIGTYPE_UINT64:
            if(CONFIG_uIntFromString(item, value, UINT64_MAX, &uResult)) {
                uint64_t* output = item->pointer;
                *output = uResult;
            }
            break;
        case CONFIGTYPE_DOUBLE: {
            double result = strtod(value, &lastParsedChar);
            // Check for errors
            if(*lastParsedChar != '\0') {
                printf("[CONFIG] Value for key '%s' is not an double: %s\n", item->key, value);
                CONFIG_EXIT();
            } else {
                double* output = (double*)item->pointer;
                *output = result;
            }
            break;
        }
        case CONFIGTYPE_STRING: {
            // Check if allocated size (string + '\0') is big enough
            if(strlen(value) + 1 > item->stringSize) {
                printf("[CONFIG] String size is too large for key '%s' (size: %zu): %s\n", item->key, item->stringSize, value);
                CONFIG_EXIT();
            } else {
                char* output = (char*)item->pointer;
                strcpy(output, value);
            }
            break;
        }
        case CONFIGTYPE_NEW_STRING: {
            char** output = (char**)item->pointer;
            *output = malloc(sizeof(value));
            strcpy(*output, value);
            break;
        }
        case CONFIGTYPE_CALLBACK: {
            Config_callback* output = (Config_callback*)item->callback;
            output(item->pointer, item->key, value);
            break;
        }
        default:
            fprintf(stderr, "[CONFIG] Invalid config type: %d\n", item->type);
            CONFIG_EXIT();
    }
}

// Removes all whitespace by modifying string and returning new start pointer
char* Config_trimString(char *input)
{
    // Trim leading space
    while(isspace((unsigned char)*input))
        input++;

    if(*input != 0) {
        // Trim trailing space
        char *end = input + strlen(input) - 1;
        while(end > input && isspace((unsigned char)*end))
            end--;
        // Write new null terminator character
        end[1] = '\0';
    }
    return input;
}

void Config_load(const char* filepath, ConfigItem* items)
{
    // Load the configuration file
    FILE* file = fopen(filepath, "r");
    if(file == NULL) {
        fprintf(stderr, "[CONFIG] Unable to open '%s': %s\n", filepath, strerror(errno));
        exit(EXIT_FAILURE);
    }

    uint32_t lineNumber = 0;
    char buffer[CONFIG_BUFFERSIZE];
    #ifdef CONFIG_VERBOSE_UNUSED
    bool handlerFound;
    #endif
    while(fgets(buffer, CONFIG_BUFFERSIZE, file)) {
        lineNumber++;
        // Get key from the line
        char* startPointer = Config_trimString(buffer);
        if(*startPointer == 0) {
            #ifdef CONFIG_DEBUG
            fprintf(stderr, "[CONFIG] Line %d: Empty line\n", lineNumber);
            #endif
            continue;
        } else if(*startPointer == CONFIG_COMMENTSIGN) {
            #ifdef CONFIG_DEBUG
            fprintf(stderr, "[CONFIG] Line %d: Comment '%s'\n", lineNumber, startPointer);
            #endif
            continue;
        }

        // Check for comment inside the line
        char* endOfLineComment = strchr(startPointer, CONFIG_COMMENTSIGN);
        if(endOfLineComment != NULL) {
            *endOfLineComment = '\0';
            startPointer = Config_trimString(buffer);
        }

        // We have some non-comment content in the line, let's parse
        char* equalSign = strchr(startPointer, CONFIG_EQUALSIGN);
        if(equalSign == NULL) {
            fprintf(stderr, "[CONFIG] PARSING ERROR! No '=' found in line %d\n", lineNumber);
            exit(EXIT_FAILURE);
        }

        // Equal sign position is used as a separator
        *equalSign = '\0';
        char* keyPtr = Config_trimString(startPointer);
        char* valPtr = Config_trimString(equalSign+1);

        // Check that key and value are set
        if(*keyPtr == '\0') {
            fprintf(stderr, "[CONFIG] PARSING ERROR! No key given in line %d.\n", lineNumber);
            exit(EXIT_FAILURE);
        }
        if(*valPtr == '\0') {
            fprintf(stderr, "[CONFIG] PARSING ERROR! No value given in line %d.\n", lineNumber);
            exit(EXIT_FAILURE);
        }

        #ifdef CONFIG_DEBUG
        fprintf(stderr, "[CONFIG] Line %d: Parsed item (%s : %s)\n", lineNumber, keyPtr, valPtr);
        #endif

        // Check if key is in specified items
        #ifdef CONFIG_VERBOSE_UNUSED
        handlerFound = false;
        #endif
        ConfigItem* item = items;
        while(item->type != CONFIGTYPE_END) {
            // Check if item matches the key
            if(strcmp(item->key, keyPtr) == 0) {
                #ifdef CONFIG_DEBUG
                fprintf(stderr, "[CONFIG] Line %d: Found handler for (%s : %s)\n", lineNumber, keyPtr, valPtr);
                #endif

                // Check if key was already set
                #ifdef CONFIG_VERBOSE_DUPLICATE
                if(item->isSet) {
                    fprintf(stderr, "[CONFIG] Key duplicate in line %d: %s\n", lineNumber, keyPtr);
                }
                #endif

                #ifdef CONFIG_VERBOSE_UNUSED
                handlerFound = true;
                #endif

                // Handle item depending on its type
                Config_handleItem(item, valPtr);
                item->isSet = true;
                break;
            }
            item++;
        }

        // Check if all handlers were set or print error
        #ifdef CONFIG_VERBOSE_UNUSED
        if(!handlerFound) {
            fprintf(stderr, "[CONFIG] Key in line %d not used: %s\n", lineNumber, keyPtr);
        }
        #endif
    }
    fclose(file);

    // Check that all items were successfully read from config
    #ifdef CONFIG_VERBOSE_MISSING
    ConfigItem* item = items;
    while(item->type != CONFIGTYPE_END) {
        if(!item->isSet) {
            fprintf(stderr, "[CONFIG] Config does not contain key: %s\n", item->key);
        }
        item++;
    }
    #endif
}

#endif