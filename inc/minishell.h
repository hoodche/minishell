/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igcastil <igcastil@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 17:11:06 by igcastil          #+#    #+#             */
/*   Updated: 2024/07/22 09:58:48 by igcastil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/*
**								HEADERS
**______________________________________________________________________________
*/
# include "../libft/libft.h"
# include <stdio.h>				//printf 
# include <stdlib.h>			//malloc, free
# include <unistd.h>			//getcwd, write, execve, pipe, dup, STDIN_FILENO
# include <readline/readline.h>	//readline, rl_clear_history, the variable 
								//extern char *rl_line_buffer;(gcc.. -lreadline)
# include <readline/history.h>	//add_history
# include <signal.h>			//signal, SIGINT, SIGQUIT
# include <asm-generic/ioctls.h> //TIOCSTI
# include <sys/ioctl.h> //ioctl
# include <sys/wait.h> //waitpid
# include <fcntl.h>
# include <stdbool.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <dirent.h>
# include <linux/limits.h>
# include <errno.h>

extern int	g_status;
/*
**								MACROS
**______________________________________________________________________________
*/

# ifndef DEBUG_MODE
#  define DEBUG_MODE 1
# endif

# ifndef PROMPT_SIZE
#  define PROMPT_SIZE 256
# endif

# ifndef MAX_CMDS
#  define MAX_CMDS 32
# endif

# ifndef MAX_CMD_ARGS
#  define MAX_CMD_ARGS 32
# endif

# ifndef MAX_REDIRECS
#  define MAX_REDIRECS 64
# endif

# ifndef MAX_QUOTES
#  define MAX_QUOTES 64
# endif

# ifndef SHELL_NM
#  define SHELL_NM "minishell"
# endif

# define ERR_SYNTAX "minishell: syntax error"
# define ERR_EXPORT_ID "not a valid identifier"
# define ERR_EXIT_ARG "numeric argument required"
# define CMD_NOT_FOUND "command not found"
# define ERR_ISDIR "Is a directory"
# define F_TEMP "temp_hdoc_"
# define HEREDOC_PROMPT "> "
# define ENV_PATH "PATH"
# define S_ECHO "echo"
# define S_CD "cd"
# define S_PWD "pwd"
# define S_EXPORT "export"
# define S_UNSET "unset"
# define S_ENV "env"
# define S_EXIT "exit"
# define ERROR -1
# define OK 0

enum e_pipe_fd
{
	READ_END,
	WRITE_END
};

enum e_env_status
{
	ORIGINAL,
	CHANGED_CD,
	CHANGED_EXPORT,
	CHANGED_UNSET
};

typedef enum e_mini_error
{
	UNB_SING_QUOT = 1,
	UNB_DOUB_QUOT = 2,
	CLOBBER = 3,
	OR = 4,
	PIPE_SIGN = 5,
	UNEXPECTED_NL = 6,
	DUPERR = 7,
	FORKERR = 8,
	PIPERR = 9,
	PIPENDERR = 10,
	MEM = 11,
	IS_DIR = 12,
	NOT_DIR = 13
}	t_mini_error;

typedef enum e_redirection
{
	IN = 1,// <
	OUT = 2,// >
	APPEND = 3,// >>
	HEREDOC = 4,// <<
}	t_redir_kind;

typedef enum e_builtin
{
	NOT_BUILTIN,
	IS_ECHO,
	IS_CD,
	IS_PWD,
	IS_EXPORT,
	IS_UNSET,
	IS_ENV,
	IS_EXIT
}	t_builtin;

//*******************************    STRUCTS    *******************************

typedef struct s_dlist
{
	int				status;
	char			*backup;
	char			*key;
	char			*value;
	struct s_dlist	*next;
	struct s_dlist	*prev;
}	t_dlist;

typedef struct s_redir
{
	t_redir_kind	kind;
	int				fd;
	char			*filename;
	char			*delimeter; // for heredoc
	int				expand_heredoc_content; //1 if delimeter contained quotes 
		//($ expansion must NOT be made on heredoc content file), 0 otherwise
}	t_redir;

typedef struct s_cmd
{
	char			*str;
	char			*ruling_dbl_quotes[MAX_QUOTES]; //stores couples of opening 
							//and closing quotes, so its length is always even
	char			*ruling_sgl_quotes[MAX_QUOTES];
	char			*redirec_ptr[MAX_REDIRECS];//pointers to positions in str 
												//where redirections are
	t_redir			redir[MAX_REDIRECS];
	char			*argv_for_execve[MAX_CMD_ARGS];
	pid_t			pid;
	int				fd_infile;
	int				fd_outredir;
}	t_cmd;

typedef struct s_read_input
{
	char			*typed;
	t_dlist			*env;
	char			*ruling_dbl_quotes[MAX_QUOTES];
	char			*ruling_sgl_quotes[MAX_QUOTES];
	char			*redirec_ptr[MAX_REDIRECS];
	char			*pipes[MAX_CMDS];
	t_cmd			cmds[MAX_CMDS];
	int				cmd_count;
	int				heredoc_count;
	char			**cmd_paths;
	int				pipe_count;
	int				**pipe_fd;
}	t_read_input;
/*
**								FUNCTION PROTOTYPES
**______________________________________________________________________________
*/

void	set_prompt(char prompt[PROMPT_SIZE]);
int		is_valid_input(char *input_str);
int		are_input_quotes_balanced(char *typed);
void	locate_input_quotes(t_read_input *read_input);
void	init_environ(t_read_input	*input, char **envv);
void	minishell_init(t_read_input	*input, char **envv);
void	reset_data(t_read_input	*input);
void	init_prompt(t_read_input *input);
void	free_matrix(char **matrix);
t_dlist	*mini_getenv(char *key, t_dlist	*env);
char	*mini_getenv_value(char *key, t_dlist *env);
int		mini_setenv(char *var, char *val, int status, t_dlist **env);
void	mini_replace_env(t_dlist *l, char *value, int status);
char	**makes_env_matrix_from_env_list(t_dlist *env);
void	mini_perror(t_mini_error err_type);
void	handle_sigint(int sig);
int		extract_redirs_from_cmds(t_read_input *input);
void	init_index_quotes_counters(int *si, int *di);
void	get_cmds_2_be_executed(t_read_input *input);
int		parse(t_read_input *input);
void	get_cmds_2_be_executed(t_read_input *input);
char	*get_word(char **cmd_str, t_cmd *cmd, int *i_sgl, int *i_dbl);
int		is_quoted_in_input(t_read_input *input, char *let);
int		is_quoted_in_cmd(t_cmd *cmd, char *let);
int		is_sgl_quoted_in_cmd(t_cmd *cmd, char *let);
void	expand_dollar(t_read_input *input);
void	repoint_ptrs_in_cm(t_cmd *cmd, char *old_or, size_t exp_of, int exp_sz);
void	repoint_qt(t_cmd *cmd, char *old_orig, size_t exp_offset, int exp_size);
int		expand(t_cmd *cmd, char *old, size_t size, char *new);
char	*ft_realloc(char *str, size_t new_size);
void	switch_case_dollar(t_read_input *input, int i, char **let);
void	exp_dol_to_empty(t_cmd *cmd, char **let, char *old_or, size_t dol_of);
void	exp_dol_qestn_mark(t_cmd *cmd, char **let, char *old_or, size_t dol_of);
size_t	get_envvar_name_length(char **let);
int		is_in_heredoc_delimeter(t_cmd *cmd, char *let);
void	print_debug_log(t_read_input	*input);
void	get_quotes_in_cm_sbstr(t_cmd *c, char *s, char *e, char *q[MAX_QUOTES]);
char	*extr_red_flnm_frm_cm(t_cmd *cmd, char *start, char *end, t_redir *red);
void	reclculate_dbl_quote_ptrs(t_read_input *input, int cmd_idx, int *i_dbl);
void	reclculate_sgl_quote_ptrs(t_read_input *input, int cmd_idx, int *i_sgl);
void	recalculate_redirs_ptrs(t_read_input *input, int cmd_idx, int *i_red);
void	recalc_dbl_qote_ptrs_last(t_read_input *input, int cmd_idx, int *i_dbl);
void	recalc_sgl_qote_ptrs_last(t_read_input *input, int cmd_idx, int *i_sgl);
/****************** T_DLIST *******************/
t_dlist	*ft_dlistnew(char *key, char *value);
void	ft_dlistadd_front(t_dlist **l, t_dlist *new);
t_dlist	*ft_dlistlast(t_dlist *l);
void	ft_dlistadd_back(t_dlist **l, t_dlist *new);
void	ft_dlistclear(t_dlist **l);
/****************** GENERAL UTILS *******************/
int		ft_startswith(char *s, char *start);
char	*ft_join_strs(char *s1, char *s2, char *s3);
/***************** REDIRECTIONS PS ******************/
int		mini_heredoc_process(t_read_input *in);
void	mini_heredoc_del_temp(t_cmd *cmd);
int		write_and_expand_heredoc(t_read_input *in, int fd, char *line);
int		inredir_process(t_read_input *in, t_cmd *cmd, int pread, int *copy_in);
int		outred_process(t_read_input *in, t_cmd *cmd, int pwrite, int *copy_out);
void	find_first_outredir(t_cmd *cmd);
/***************************** PIPES ************************************/
void	ft_pipes_init(t_read_input *in);
void	ft_pipes_start(t_read_input *in);
void	ft_pipes_close(t_read_input *in);
void	ft_pipes_destroy(t_read_input *in);
/**************************** CHILDREN **********************************/
void	ft_fork_only_child(t_read_input *in);
void	ft_fork_children(t_read_input *in);
void	ft_wait_children(t_read_input *in);
/**************************** CMDS UTILS ********************************/
int		cmd_count_args(t_cmd *cmd);
void	mini_unsetenv(char *key, t_dlist **env);
void	mini_close_fds(t_read_input *in, t_cmd *cmd);
void	update_cmd_paths(t_read_input *in);
/*******************************EXECUTION********************************/
int		mini_select_execution(t_read_input *in);
void	ft_cmd_exec(t_read_input *in, t_cmd *cmd, int write_pipe);
/****************************** BUITINS *********************************/
int		cmd_is_builtin(t_cmd *cmd);
int		invalid_flags_standard(t_cmd *cmd);
int		mini_builtin_process(t_read_input *in, t_cmd *cmd, int type);
void	mini_echo(t_read_input *in, t_cmd *cmd);
void	mini_cd(t_read_input *in, t_cmd *cmd);
char	*cd_get_abspath(t_read_input *in, char *new_dir);
void	cd_print(int used_cdpath, char **s);
char	*get_current_pwd(t_read_input *in);
char	*delete_last_path(char *pwd);
char	*add_path_to_pwd(char *pwd, char *dir);
char	*check_pwd_empty(char *pwd);
void	mini_pwd(t_read_input *in, t_cmd *cmd);
void	mini_env(t_read_input *in, t_cmd *cmd);
void	mini_export(t_read_input *in, t_cmd *cmd);
void	mini_unset(t_read_input *in, t_cmd *cmd);
void	mini_exit(t_read_input *in, t_cmd *cmd);
/************************* FREE RESOURCES *******************************/
void	free_resources(t_read_input *in);
void	mini_destroy_and_exit(t_read_input *in);
/*************************** ERRORS  ************************************/
void	mini_errno_error(int status, char *cmd, char *arg);
void	mini_program_error(t_read_input *in);
void	mini_formatted_error(int status, char *cmd, char *arg, char *msg);
void	error_execve(t_read_input *in, t_cmd *cmd);
#endif
