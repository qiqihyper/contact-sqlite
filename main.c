#include "stdio.h"
#include "stdlib.h"
#include "sqlite3.h"

//����һ�����ݿ⣬���ֽ�db 
sqlite3 *db=NULL;
	 
void create_friend()
{
	char name[10];
	char phone[17];
	char sql[64];
	
	printf("\n����������ϵ�˵� �������绰 ���Կո������\n");
	scanf("%s %s", name, phone); 
	
	//�õ���Ҫִ�е�sql��䣬��������� 
	sprintf(sql, "insert into info values('%s', '%s');", name, phone);
	//printf("%s\n", sql); //������Ϣ 
	
	int ret;
	ret = sqlite3_exec(db, sql, 0, 0, 0);  
    if(ret != SQLITE_OK){  
        printf("����ʧ��!  : %s\n\n", sqlite3_errmsg(db));  
    } else {  
        printf("�����ɹ�!  ��%s %s\n\n", name, phone);  
    } 
}


void find_friend()
{
	char name[10];
	char sql[64];
	
	printf("\n������Ҫ������ϵ�˵�������");
	scanf("%s", name);

	//�õ���Ҫִ�е�sql��䣬���������
	sprintf(sql,"select * from info where name = '%s'", name);
	//printf("%s\n", sql); 
	
	int ret, row = 0, col = 0;
	//����һ����ά�ַ���ָ�룬���ڱ����ѯ��� 
	char **set_of_information;
	//������������Ѳ��ҵĽ�������� set_of_information �����ά�ַ��������� 
	//ͬʱ�Ѳ�ѯ����������������ֱ𱣴��� row��col ������ 
	ret = sqlite3_get_table(db, sql, &set_of_information, &row, &col, 0);  
    if(ret != SQLITE_OK){  
        printf("����ʧ�ܣ�%s\n",sqlite3_errmsg(db));  
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
        printf("����ʧ�ܣ�δ�ҵ����ˣ�\n");  
    } 	 
}

void delete_friend()
{
	char name[10];
	char sql[64];
		
	printf("\n������Ҫɾ����ϵ�˵�������");
	scanf("%s", name);

	sprintf(sql, "delete from info where name='%s';", name);
	printf("%s\n", sql);
	
	int ret;
	ret = sqlite3_exec(db,sql,0,0,0);  
    if(ret != SQLITE_OK){  
        printf("ɾ��ʧ�ܣ�%s\n",sqlite3_errmsg(db));  
    } else{  
    	printf("ɾ���ɹ�!\n");  
    }  
}

void modify_friend()
{
	char name[10];
	char phone[12];
	printf("\n����������ϵ�˵� �������绰 ���Կո������\n");
	scanf("%s %s", name, phone);
	
	char sql[64];
	sprintf(sql, "update info set name='%s',phone='%s' where name='%s';",
		name, phone, name);
	printf("%s\n", sql);
	
	int ret;
	ret = sqlite3_exec(db, sql, 0, 0, 0);
	if(ret != SQLITE_OK){  
        printf("�޸�ʧ�ܣ�%s\n", sqlite3_errmsg(db));  
    } else{  
    	printf("�޸ĳɹ�!\n");  
    }
}

void display_friend()
{
	char *sql = "select * from info;";
	int row, col, ret;
	char **set_of_information;
	
	ret = sqlite3_get_table(db, sql, &set_of_information, &row, &col, 0);  
    if(ret != SQLITE_OK) {  
        printf("���ݿ����%s\n", sqlite3_errmsg(db));
		return;  
    }  
  
    printf("\n���� %d ����ϵ�ˡ�\n", row);  
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
   	printf("      ***     1 ������ϵ��      ****\n");  
    printf("      ***     2 ������ϵ��      ****\n");  
    printf("      ***     3 ɾ����ϵ��      ****\n"); 
	printf("      ***     4 �޸���ϵ��      ****\n"); 
	printf("      ***     5 ��ʾ��ϵ��      ****\n"); 
    printf("      ***     0 �˳�            ****\n");  
    printf("      ******************************\n");
    printf("\n��������ѡ��Ĳ�����");
}

int main( void )  
{
	system("mode con cols=46 lines=40");
	 
    //���ݿ��ļ������� 
    char *file = "contact.db";  

    //��ָ�������ݿ��ļ�,��������ڽ�����һ��ͬ�������ݿ��ļ�
	//sqlite3_open()�����ķ���ֵҪ��0�Ļ�����ִ����������������ֵ�Ļ�����ִ��
	//�������д���  
    if( sqlite3_open(file, &db) ) {  
        printf("�����ݿ����: %s\n", sqlite3_errmsg(db));  
        sqlite3_close(db);  
        return 1;  
    } else {
        printf("�����ݿ� %s �ɹ���\n\n", file);
    }  
	
	//sql��������һ���ַ���������ַ���������ĺ��壬����һ��sql��䣬
	//���Ը����ݿ���ȥִ�У�����������������˼�Ǵ���һ�����ݿ��
	//���ֽ�info���������¼�������ݣ�һ����name, һ���� phone 
	char *sql = "create table if not exists info(name text, phone text);";
	int  ret;
	//errmsg��һ���ַ���ָ�룬�������ʾ���Ҫ�����ݿ�����д���Ļ���errmsg
	//��ָ�������Ϣ�����ڴ�ӡ��������  
	ret = sqlite3_exec(db, sql, 0, 0, 0); 
	if (ret != SQLITE_OK)
		printf("�������ݿ�����%s\n",sqlite3_errmsg(db));
	printf("��ʼ�����ݿ�ɹ���\n\n"); 
    
    printf("*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*\n");
    printf("*=*             ��ϵ�˲�������            *=*\n");   
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
			printf("\n��ȷ���Ƿ��˳�����1���ǣ�0���񣩣�");  
        	scanf("%d", &choice);  
			if (choice == 1)
				break;
		} else {
			printf("\n��ѡ����ȷ�Ĳ�����ţ�");
			//goto input_choice; 
			continue;
		}
    }      
     
    sqlite3_close(db);//�ر����ݿ�  
    return 0;  
}  
