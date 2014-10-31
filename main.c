#include "stdio.h"
#include "stdlib.h"
#include "sqlite3.h"

//声明一个数据库，名字叫db 
sqlite3 *db=NULL;
	 
void create_friend()
{
	char name[10];
	char phone[17];
	char sql[64];
	
	printf("\n请输入新联系人的 姓名、电话 ，以空格隔开：\n");
	scanf("%s %s", name, phone); 
	
	//得到需要执行的sql语句，存入变量中 
	sprintf(sql, "insert into info values('%s', '%s');", name, phone);
	//printf("%s\n", sql); //调试信息 
	
	int ret;
	ret = sqlite3_exec(db, sql, 0, 0, 0);  
    if(ret != SQLITE_OK){  
        printf("创建失败!  : %s\n\n", sqlite3_errmsg(db));  
    } else {  
        printf("创建成功!  ：%s %s\n\n", name, phone);  
    } 
}


void find_friend()
{
	char name[10];
	char sql[64];
	
	printf("\n请输入要查找联系人的姓名：");
	scanf("%s", name);

	//得到需要执行的sql语句，存入变量中
	sprintf(sql,"select * from info where name = '%s'", name);
	//printf("%s\n", sql); 
	
	int ret, row = 0, col = 0;
	//声明一个二维字符串指针，用于保存查询结果 
	char **set_of_information;
	//下面这个函数把查找的结果保存在 set_of_information 这个二维字符串向量中 
	//同时把查询结果的行数、列数分别保存在 row、col 变量中 
	ret = sqlite3_get_table(db, sql, &set_of_information, &row, &col, 0);  
    if(ret != SQLITE_OK){  
        printf("查找失败：%s\n",sqlite3_errmsg(db));  
        return;  
    }
    int index = 0, i, j;  
    if(row > 0) {  
        for(i = 0; i <= row; i++) {  
            for(j = 0; j < col; j++) 
                printf("%-11s", set_of_information[index++]);   
            printf("\n");  
        }  
    } else {  
        printf("查找失败：未找到此人！\n");  
    } 	 
}

void delete_friend()
{
	char name[10];
	char sql[64];
		
	printf("\n请输入要删除联系人的姓名：");
	scanf("%s", name);

	sprintf(sql, "delete from info where name='%s';", name);
	printf("%s\n", sql);
	
	int ret;
	ret = sqlite3_exec(db,sql,0,0,0);  
    if(ret != SQLITE_OK){  
        printf("删除失败：%s\n",sqlite3_errmsg(db));  
    } else{  
    	printf("删除成功!\n");  
    }  
}

void modify_friend()
{
	char name[10];
	char phone[12];
	printf("\n请输入新联系人的 姓名、电话 ，以空格隔开：\n");
	scanf("%s %s", name, phone);
	
	char sql[64];
	sprintf(sql, "update info set name='%s',phone='%s' where name='%s';",
		name, phone, name);
	printf("%s\n", sql);
	
	int ret;
	ret = sqlite3_exec(db, sql, 0, 0, 0);
	if(ret != SQLITE_OK){  
        printf("修改失败：%s\n", sqlite3_errmsg(db));  
    } else{  
    	printf("修改成功!\n");  
    }
}

void display_friend()
{
	char *sql = "select * from info;";
	int row, col, ret;
	char **set_of_information;
	
	ret = sqlite3_get_table(db, sql, &set_of_information, &row, &col, 0);  
    if(ret != SQLITE_OK) {  
        printf("数据库错误：%s\n", sqlite3_errmsg(db));
		return;  
    }  
  
    printf("\n共有 %d 个联系人。\n", row);  
    int index = 0, i, j;  
    for(i = 0; i <= row; i++) {  
        for(j = 0; j < col; j++)
            printf("%-11s", set_of_information[index++]);  
        printf("\n");  
    }  
} 

void print_navigation()
{
	printf("\n      ******************************\n");  
   	printf("      ***     1 创建联系人      ****\n");  
    printf("      ***     2 查找联系人      ****\n");  
    printf("      ***     3 删除联系人      ****\n"); 
	printf("      ***     4 修改联系人      ****\n"); 
	printf("      ***     5 显示联系人      ****\n"); 
    printf("      ***     0 退出            ****\n");  
    printf("      ******************************\n");
    printf("\n请输入您选择的操作：");
}

int main( void )  
{
	system("mode con cols=46 lines=40");
	 
    //数据库文件的名字 
    char *file = "contact.db";  

    //打开指定的数据库文件,如果不存在将创建一个同名的数据库文件
	//sqlite3_open()函数的返回值要是0的话表明执行正常，返回其他值的话表明执行
	//过程中有错误  
    if( sqlite3_open(file, &db) ) {  
        printf("打开数据库错误: %s\n", sqlite3_errmsg(db));  
        sqlite3_close(db);  
        return 1;  
    } else {
        printf("打开数据库 %s 成功！\n\n", file);
    }  
	
	//sql变量就是一个字符串，这个字符串有特殊的含义，它是一条sql语句，
	//可以给数据库拿去执行，在这里这条语句的意思是创建一个数据库表，
	//名字叫info，表里面记录两项内容，一项是name, 一项是 phone 
	char *sql = "create table if not exists info(name text, phone text);";
	int  ret;
	//errmsg是一个字符串指针，在这里表示如果要是数据库操作有错误的话则errmsg
	//会指向错误信息，便于打印出来调试  
	ret = sqlite3_exec(db, sql, 0, 0, 0); 
	if (ret != SQLITE_OK)
		printf("创建数据库表错误：%s\n",sqlite3_errmsg(db));
	printf("初始化数据库成功！\n\n"); 
    
    printf("*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\n");
    printf("*=*             联系人操作程序            *=*\n");   
    printf("*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\n\n");
    
    int choice;
    while (1) {
		print_navigation();
		 
	//input_choice: 
        scanf("%d", &choice);
        
		if (choice == 1) {
			create_friend();
		} else if (choice == 2) {
			find_friend();
		} else if (choice == 3) {
			delete_friend();
		} else if (choice == 4) {
			modify_friend();
		} else if (choice == 5) {
			display_friend();
		} else if (choice == 0) {
			printf("\n请确定是否退出程序、1（是）0（否）：");  
        	scanf("%d", &choice);  
			if (choice == 1)
				break;
		} else {
			printf("\n请选择正确的操作序号：");
			//goto input_choice; 
			continue;
		}
    }      
     
    sqlite3_close(db);//关闭数据库  
    return 0;  
}  
