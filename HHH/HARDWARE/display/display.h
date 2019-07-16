#ifndef __DISPLAY_H
#define __DISPLAY_H
#include "sys.h"
#include "OLED.h"
typedef struct
{
		unsigned char current;
		unsigned chaup;//�ϼ�
		unsigned chadown;//�¼�
		unsigned chaenter;//ȷ�ϼ�
		
   void (*current_operation)();
} key_table;

/*********************************************
 * ����һ���ṹ��
 * ��Ž����־λ
*/
typedef struct
{
    u8 Interface_Mark;     //����״̬
    u8 Task_Mark;          //����״̬
    u8 Run_Task;           //��ʼ��������
} Mark;
Mark Mark_Sign;//״̬��־λ

/*********************************************
 * ����һ��ö��
 * ��Ž������
*/
enum
{
    Main_Interface = 0x10, /****������*****/
    Menu_Interface = 0x20, /****�˵�����***/
    Task_Interface = 0x30, /****�������***/
};
/*******************************************/

void Main_Interface_APP();//��ʾ������
void Menu_Interface_APP();//��ʾ�˵�����
void Function_Menu_APP();//��ʾ���ܽ���

#endif