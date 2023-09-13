#include "netstack.hpp"
#include <Arduino.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <liblwm2m.h>

void *lwm2m_malloc(size_t s)
{
    return std::malloc(s);
}

void lwm2m_free(void *p)
{
    std::free(p);
}

char *lwm2m_strdup(const char *str)
{
    size_t len   = strlen(str);
    char *newStr = (char *)lwm2m_malloc(len + 1);
    if (newStr == NULL)
        return NULL;
    memcpy(newStr, str, len);
    newStr[len] = 0;
    return newStr;
}

int lwm2m_strncmp(const char *s1, const char *s2, size_t n)
{
    return strncmp(s1, s2, n);
}

int lwm2m_strcasecmp(const char *str1, const char *str2)
{
    return strcasecmp(str1, str2);
}

time_t lwm2m_gettime(void)
{
    return (time_t)millis() / 1000;
}

int lwm2m_seed(void)
{
    return random(UINT32_MAX);
}

extern NetStack net;

uint8_t lwm2m_buffer_send(void *sessionH, uint8_t *buffer, size_t length, void *userData)
{
    (void)userData;
    (void)sessionH;

    return net.send(buffer, length) != -1 ? length : 0;
}

bool lwm2m_session_is_equal(void *session1, void *session2, void *userData)
{
    (void)userData;
    return session1 == session2;
}

void *lwm2m_connect_server(uint16_t secObjInstID, void *userData)
{
    return &net;
}

void lwm2m_close_connection(void *sessionH, void *userData)
{
}

void lwm2m_printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}
