/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jerda-si <jerda-si@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 14:55:09 by jerda-si          #+#    #+#             */
/*   Updated: 2025/01/07 15:51:12 by jerda-si         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# include "./get_next_line/get_next_line.h"

# define LIBFT_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1000
# endif

# include <unistd.h>
# include <stdlib.h>

void			*ft_memset(void *ptr, int value, size_t num);
void			ft_bzero(void *s, size_t len);
void			*ft_memcpy(void *dest, const void *src, size_t n);
void			*ft_memmove(void *dest, const void *src, size_t n);
void			*ft_memchr(const void *s, int c, size_t n);
void			*ft_calloc(size_t nmemb, size_t size);
char			*ft_strdup(const char *src);
int				ft_toupper(int str);
int				ft_tolower(int str);
char			*ft_strnstr(const char *str, const char *to_find, size_t size);
char            *ft_strchr(char *s, int c);
char			*ft_strrchr(char *str, int s);
size_t          ft_strlen(const char *str);
int				ft_isdigit(int c);
int				ft_isalpha(int str);
int				ft_isalnum(int str);
int				ft_isprint(int str);
int				ft_isascii(int str);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
size_t			ft_strlcpy(char *dest, char *src, size_t size);
int				ft_atoi(const char *s);
int				ft_memcmp(const void *ptr1, const void *ptr2, size_t num);
size_t			ft_strlcat(char *dest, const char *src, size_t size);
void			*ft_calloc(size_t nmemb, size_t size);
void			ft_putchar_fd(char c, int fd);
void			ft_putstr_fd(char *s, int fd);
void			ft_putendl_fd(char *s, int fd);
void			ft_putnbr_fd(int n, int fd);
char			*ft_substr(char const *s, unsigned int start, size_t len);
char			*ft_strtrim(char const *s1, char const *set);
char			*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void			ft_striteri(char *s, void (*f)(unsigned int, char*));
char            *ft_strjoin(char *left_str, char *buff);
char			*ft_itoa(int n);
char			**ft_split(char const *s, char c);

#endif 
