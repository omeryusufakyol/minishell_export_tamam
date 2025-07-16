/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oakyol <oakyol@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 23:39:51 by oakyol            #+#    #+#             */
/*   Updated: 2025/07/17 00:14:14 by oakyol           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# ifndef MAX_CMDS
#  define MAX_CMDS 256
# endif

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>
# include <signal.h>
# include <errno.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_ms
{
	char	**env; // Ortam değişkenleri
	int		last_exit; // Son komutun çıkış kodu ($?)
	char	*raw_input;
	char	**export_only;
}	t_ms;

typedef struct s_cmd
{
	char			**args;     // Komut + argümanlar
	char			*infile;    // <
	char			*outfile;   // > veya >>
	int				append;     // >> varsa 1, > varsa 0
	int				heredoc;    // << varsa 1, yoksa 0
    char            **heredoc_delims; // birden fazla heredoc için
	struct s_cmd	*next;      // Pipe için sonraki komut
}	t_cmd;

// main.c
void	handle_signals(void);

// minishell.c
void	mini_loop(t_ms *ms);

// init.c
void	init_minishell(t_ms *ms, char **envp);

// free.c
void	free_minishell(t_ms *ms);

// lexer.c
char	**lexer(const char *input);

// lexer_utils.c
int		count_tokens(const char *s);
int		token_len(const char *s, int i);

// sytanx_check.c
int		check_syntax(char **tokens, t_ms *ms);

// parser.c
t_cmd	*parser(char **tokens);

// parser_utils.c
t_cmd	*init_cmd(void);
void	add_cmd(t_cmd **cmds, t_cmd *new_cmd);
int		count_args(char **tokens, int i);
char	**copy_args(char **tokens, int start, int end);

// executor.c
void	redirect(t_cmd *cmd, t_ms *ms);
void	execute(t_cmd *cmds, t_ms *ms);

// executor_utils.c
char	*find_path(char *cmd, char **env);

// is_builtin.c
int		is_builtin(char *cmd);

// run_builtin.c
int		run_builtin(t_cmd *cmd, t_ms *ms);

// ft_echo.c
int		ft_echo(char **args);

// ft_pwd.c
int		ft_pwd(void);

// ft_env.c
int		ft_env(char **env);

// ft_cd.c
int		ft_cd(char **args, t_ms *ms);

// ft_cd_utils.c
int	cd_check_args(char **args, t_ms *ms);

// ft_exit.c
int		ft_exit(char **args, t_ms *ms);

// ft_export.c
void	update_env(t_ms *ms, const char *arg);
int		ft_export(char **args, t_ms *ms);

// ft_export_utils.c
int	has_equal(const char *s);
int	update_if_exists(t_ms *ms, char *clean);
void	add_to_export_only(t_ms *ms, const char *arg);
void	remove_from_export_only(t_ms *ms, const char *arg);

// ft_print_export.c
void	print_export(t_ms *ms);

// ft_unset.c
int		ft_unset(char **args, t_ms *ms);

// expand.c
char	*get_env_value(t_ms *ms, const char *name);
char	**expand_tokens(char **tokens, t_ms *ms);

// heredoc.c
void	add_heredoc(t_cmd *cmd, char *delim);
int	handle_heredoc(t_cmd *cmd, t_ms *ms);

// heredoc_utils.c
char	*expand_heredoc_line_envonly(char *line);

// pipeline.c
int		run_pipeline(t_cmd *cmds, t_ms *ms);

// DEBUG.c
void	print_cmds(t_cmd *cmd);

#endif
