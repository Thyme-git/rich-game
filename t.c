#ifndef _USERS_H_
#define _USERS_H_
#include"../utili/utili.h"
#include"../umap/uthash.h"
#include"stdlib.h"
#include"stdio.h"
#include"../io/in.h"
typedef struct 
{
    const char *name;
    int money;
    char c;
    int id;
    int stopnum;
    int wealnum;
    int pos;
    int credit;
    int tool[10];
    Color_enum outcolor;
}Player_t;

typedef enum _Role {
    ROLE_Q // 钱夫人
,   ROLE_A // 阿土伯
,   ROLE_S // 孙小美
,   ROLE_J // 金贝贝
} Role_enum;


Player_t *player[4];
void InitUsers()
{
    int uses[4]={0};
    printf("设置玩家的初始资金，范围为1500~50000\n");
    int sum=0;
    while(1)
    {
       sum=scanfnum();
        if(sum<1500||sum>50000)
        printf("输入错误，请重新输入初始资金");
        else  break;
    } 

    printf("请选择2~4个角色：1.钱夫人 2.阿土伯 3.孙小美 4.金贝贝，可自由排序");
    char c=getchar();
    while(c!='\n')
    {
        if((c-'0')<=4&&(c-'0')>=1)
        {
            if(uses[(c-'1')]==0)
            {uses[(c-'1')]++;
            c=getchar();}
            else 
            {
                c=getchar();
                continue;}
        }
        else 
        {   c=getchar();
            continue;
        }
    }

    for(int i=0;i<4;i++)
    {
    if(uses[i]==0)
    {
       continue;
    }
    else 
    {
      if(i==0)
      {
         Player_t *s=(Player_t *)malloc(sizeof(Player_t));
         s=(Player_t*)malloc(sizeof(Player_t));
         s->id=1;
         s->name="钱夫人";
         s->c='Q';
         s->credit=0;
         s->money=sum;
         s->outcolor=Red;
         s->pos=0;
         s->stopnum=0;
         s->wealnum=0;
         player[i]=s;
      }
      else if(i==1)
      {

        Player_t* s=(Player_t *)malloc(sizeof(Player_t));
         s=(Player_t*)malloc(sizeof(Player_t));
         s->name="阿土伯";
         s->c='A';
         s->credit=0;
         s->id=2;
         s->money=sum;
         s->outcolor=Green;
         s->pos=0;
         s->stopnum=0;
         s->wealnum=0;
         player[i]=s;
        //HASH_ADD_INT(hash,id,s);
      }
      else if(i==2)
      {
         Player_t* s=(Player_t *)malloc(sizeof(Player_t));
         s=(Player_t*)malloc(sizeof(Player_t));
         s->id=3;
         s->name="孙小美";
         s->c='S';
         s->credit=0;
         s->money=sum;
         s->outcolor=Blue;
         s->pos=0;
         s->stopnum=0;
         s->wealnum=0;
         player[i]=s;
        //HASH_ADD_INT(hash,id,s);
      }
      else if(i==3)
      {
         Player_t *s=(Player_t *)malloc(sizeof(Player_t));
         s=(Player_t*)malloc(sizeof(Player_t));
         s->c='J';
         s->name="金贝贝";
         s->credit=0;
         s->id=4;
         s->money=sum;
         s->outcolor=Yellow;
         s->pos=0;
         s->stopnum=0;
         s->wealnum=0;
         player[i]=s;
        //HASH_ADD_INT(hash,id,s);
      }
      }
      }
}
Player_t *findUser(int id)
{
    if(player[id]!=NULL)
    {
        return player[id];
    }
    return NULL;
}
int getUserstopnum(Player_t user)
{
    return user.stopnum;
}
int getUserpos(Player_t user)
{
    return user.pos;
}
int getUserwealnum(Player_t user)
{
    return user.wealnum;
}
int getUsercolor(Player_t user)
{
    return user.outcolor;
}
char getuserchar(Player_t user)
{
    return user.c;
}
void changeUserstopnum(Player_t *user,int num)
{
    user->stopnum=num;
}
void changeUserpos(Player_t *user,int num)
{
    if(user==NULL)
    {
        printf("用户未找到\n");
    }
    user->pos+=num;
}
void changeUserwealnum(Player_t *user,int wealnum)
{
    user->wealnum=wealnum;
}
void changeUsertool(Player_t *user,int tool)
{
    int i=0;
    for(i=0;i<10;i++)
    {
       if(user->tool[i]==0)
        user->tool[i]=tool;
    }
    if(i==10)
    {
        printf("你的道具太多，已经放不下\n");
    }
}
void Roll()
{
   int a=rand()%6+1;
   changeUserpos(findUser(2),a);
}
void Query()
{
    int i=0;
    printf("money:%d\n",findUser(2)->money);
    printf("credit:%d\n",findUser(2)->credit);
    printf("wealnum:%d\n",findUser(2)->wealnum);
    int i=0;
    printf("tool :");
    for( i=0;i<10;i++)
      {
        if(findUser(2)->tool[i]==0)
        continue;
        else printf(" %d",findUser(2)->tool[i]);
     }
      printf("\n");
}
void Robot()
{
    int i=0;
    int a=0;
    for( i=0;i<10;i++)
      {
        if( findUser(2)->tool[i]==2)
        {
         findUser(2)->tool[i]=0;
         a=findUser(2)->pos;
          for(int i=1;i<=10;i++)
         {
          if(a+i>=70)
          a=(a+i)%70;
          land[a+i].tool=0;
         }
          break;
        }
        else continue;
      }
}
void Sell(int a)
{
    a=a+findUser(2)->pos;
    if(land[a].owner==findUser(2)->c)
      {
       /*
       sell
       */
      }
     else 
      {
        printf("这里不是你的资产，请重新输入");
      }
}
void Bomb(int a)
{
    int i=0;
    a=a+findUser(2)->pos;
      for(i=0;i<10;i++)
      {
        if( findUser(2)->tool[i]==3)
        {
          findUser(2)->tool[i]=0;
          land[a].tool=3;
          break;
        }
      }
      if(i==10)
      {
        printf("你没有炸弹道具，请重新选择");
      }
     printf("\n");
}
#endif