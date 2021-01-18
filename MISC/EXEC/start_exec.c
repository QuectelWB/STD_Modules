#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


#define EXECV



int main(int argc, char *argv[])
{
//以NULL结尾的字符串数组的指针，适合包含v的exec函数参数
char *arg[] = {"./quectel-CM", "-s cmnet", NULL};


 	printf( "------------PID %d------------\n" ,getpid());
#ifdef EXECL


/**
* 创建子进程并调用函数execl
* execl 中希望接收以逗号分隔的参数列表，并以NULL指针为结束标志
*/
	if( fork() == 0 )
	{
	// in clild
		printf( "1------------execl------------\n" );

		if( execl( "./quectel-CM", "quectel-CM","-s cmnet", NULL ) == -1 )
		{
			perror( "execl error " );
			exit(1);
		}
	}
#endif

#ifdef EXECV
/**
*创建子进程并调用函数execv
*execv中希望接收一个以NULL结尾的字符串数组的指针
*/
	if( fork() == 0 )
	{
// in child
		printf("2------------execv------------\n");
		printf("2------------execv--ppid = %d----------\n",getppid());
		if( execv( "./quectel-CM",arg) < 0)
		{
			perror("execv error ");
			exit(1);
		}
	}

#endif

#if 0
/**
*创建子进程并调用 execlp
*execlp中
*l希望接收以逗号分隔的参数列表，列表以NULL指针作为结束标志
*p是一个以NULL结尾的字符串数组指针，函数可以DOS的PATH变量查找子程序文件
*/
if( fork() == 0 )
{
// in clhild
printf("3------------execlp------------\n");
if( execlp( "ls", "ls", "-a", NULL ) < 0 )
{
perror( "execlp error " );
exit(1);
}
}

/**
*创建子里程并调用execvp
*v 望接收到一个以NULL结尾的字符串数组的指针
*p 是一个以NULL结尾的字符串数组指针，函数可以DOS的PATH变量查找子程序文件
*/
if( fork() == 0 )
{
printf("4------------execvp------------\n");
if( execvp( "ls", arg ) < 0 )
{
perror( "execvp error " );
exit( 1 );
}
}

/**
*创建子进程并调用execle
*l 希望接收以逗号分隔的参数列表，列表以NULL指针作为结束标志
*e 函数传递指定参数envp，允许改变子进程的环境，无后缀e时，子进程使用当前程序的环境
*/
if( fork() == 0 )
{
printf("5------------execle------------\n");
if( execle("/bin/ls", "ls", "-a", NULL, NULL) == -1 )
{
perror("execle error ");
exit(1);
}
}

/**
*创建子进程并调用execve
* v 希望接收到一个以NULL结尾的字符串数组的指针
* e 函数传递指定参数envp，允许改变子进程的环境，无后缀e时，子进程使用当前程序的环境
*/
if( fork() == 0 )
{
printf("6------------execve-----------\n");
if( execve( "/bin/ls", arg, NULL ) == 0)
{
perror("execve error ");
exit(1);
}
}
#endif

return EXIT_SUCCESS;
}


