#include <cc_string.h>
#include <cc_type.h>

#define IN 1
#define OUT 0
#define ASCIISET 25

double cc_atof(const char *s)
{
	double val, power;
	int i, sign;

	for (i = 0; s[i] == ' '; i++);
	sign = (s[i] == '-') ? -1 : 1;
	if (s[i] == '-' || s[i] == '+')
		i++;

	for (val = 0.0; cc_isdigit(s[i]); i++)
		val = val * 10.0 + (s[i] - '0');
	power = 1.0;
	if (s[i] == '.') {
		i++;
		for (; cc_isdigit(s[i]); i++) {
			val = val * 10.0 + (s[i] - '0');
			power *= 10.0;
		}
	}

	return sign * val / power;
}

int cc_atoi(const char *s)
{
	return (int)cc_atof(s);
}

int cc_htoi(const char *s)
{
	int ret;
	int i, dec;

	i = 0;
	if (s[1] == 'x' || s[1] == 'X')
		i = 2;
	for (; s[i] != '\0'; i++) {
		if (s[i] >= '0' && s[i] <= '9')
			dec = s[i] - '0';
		else if (s[i] >= 'a' && s[i] <= 'z')
			dec = s[i] - 'a' + 10;
		else
			dec = s[i] - 'A' + 10;
		ret = ret * 16 + dec;
	}

	return ret;
}

size_t cc_strlen(const char *src)
{
	const char *s;

	for (s = src; *s != '\0'; ++s);

	return (s - src);
}

char *cc_strrev(char *dest, const char *src)
{
	int i = 0;
	size_t src_len = cc_strlen(src);

	while (src_len > 0 && (dest[i++] = src[--src_len]));
	dest[i] = '\0';

	return dest;
}

int cc_tolower(int src)
{
	return (src >= 'A' && src <= 'Z') ? (src - 'A' + 'a') : src;
}

int cc_toupper(int src)
{
	return (src >= 'a' && src <= 'z') ? (src - 'a' + 'A') : src;
}

char *cc_strcat(char *dest, const char *src)
{
	char *tmp;

	for (tmp = dest; *tmp; tmp++);
	while ((*tmp++ = *src++));

	return dest;
}

char *cc_strncat(char *dest, const char *src, size_t n)
{
	size_t dest_len = cc_strlen(dest);
	size_t i;

	for(i = 0; i < n && src[i] != '\0'; i++)
		dest[dest_len + i] = src[i];
	dest[dest_len + i] = '\0';

	return dest;
}

char *cc_trim(char *s)
{
	char *end;

	while (cc_isspace(*s++));
	if (*s == '\0')
		return s;
	end = s + cc_strlen(s) - 1;
	while (end > s && cc_isspace(*end--));
	*(end + 1) = '\0';

	return s;
}

char *cc_strsqueeze(char *s1, const char *s2)
{
	int i, j, k;
	int asci[ASCIISET] = {OUT};

	for (i = 0; s2[i] != '\0'; i++)
		asci[cc_tolower(s2[i]) - 'a'] = IN;

	for (k = 0; k < ASCIISET; k++)
		if (asci[k] == IN)
			for (i = j = 0; s1[i] != '\0'; i++)
				if (s1[i] - 'a' != k)
					s1[j++] = s1[i];
	s1[j] = '\0';

	return s1;
}

int cc_strindex(const char *src, const char *dest)
{
	int i, j, k;

	for(i = 0; src[i] != '\0'; i++) {
		for(j = i, k = 0; dest[k] != '\0' && src[j] == dest[k]; j++, k++);
		if (k > 0 && dest[k] == '\0')
			return i;
	}

	return -1;
}

char *cc_strcpy(char *dest, const char *src)
{
	char *tmp = dest;

	while ((*tmp++ = *src++));

	return dest;
}

char *cc_strncpy(char *dest, const char *src, size_t n)
{
	size_t i;

	for (i = 0; i < n && src[i] != '\0'; ++i)
		*(dest + i) = *(src + i);
	for (; i < n; ++i)
		*(dest + i) = '\0';

	return dest;
}

int cc_strcmp(char *s1, const char *s2)
{
	for (; *s1 == *s2; s1++, s2++);

	return (*s1 == '\0') ? 0 : *s1 - *s2;
}

void *cc_memset(void *s, int c, size_t n)
{
	while (n-- > 0)
		*((char *)s + n) = c;

	return s;
}

void *cc_memcpy(void *dest, const void *src, size_t n)
{
	char *temp = dest;

	while (n-- && (*temp++ = *(const char *)src++));

	return dest;
}
