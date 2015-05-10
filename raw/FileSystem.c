
# include "stdio.h"
# include "string.h"
# include <unistd.h> 
# include <stdlib.h> 
# define maxsize 1000
# define ShanNum 320
char opr[10];
char fname[maxsize];
int i_length;
int n_index;
struct Supiece{
  int Pn_inode;
  int Pn_fdata;
  int fn_piece;
  int fn_inode;
  int Fpnum[maxsize];
  int Finum[maxsize];
}sup;
struct Inode{
  int number;
  char name[maxsize];
  char dirname[maxsize];
  int length;
  int flags;
  char f_d;
  int ci;
}inode[maxsize];

struct data{
  int number;
  int flags;
  char Content[maxsize]; 
  int fd_number[maxsize];
}Data[maxsize];


void Filecreate(char F_fname[]); 
int ExistsFile(char E_fname[]);   //判断文件或目录是否存在，存在则返回1，不存在则返回0 
void Filedelete(char D_fname[]); 
void Fileopen(char O_fname[]); 
void Filels(char L_fname[]); 
void createdir(char fname[]); 
void deletedir(char fname[]); 
void LS_lfile(char fname[]); 

void doWhat(char opr[],char fname[])   //根据输入的命令进行相应的操作 
{
  if(!strcmp(opr,"create")){Filecreate(fname);}
  else if(!strcmp(opr,"delete")){Filedelete(fname);}
  else
    if(!strcmp(opr,"open")){Fileopen(fname);}
    else
      if(!strcmp(opr,"ls")){Filels(fname);}
      else 
        if(!strcmp(opr,"createdir")){createdir(fname);}
        else
          if(!strcmp(opr,"deletedir")){deletedir(fname);}
          else
            if(!strcmp(opr,"ls_l")){LS_lfile(fname);}
            else
            { printf("\t");printf("无此命令！\n");
              printf("\t");scanf("%s",opr);
              if(!strcmp(opr,"exit")){exit(1);}
              else 
              {  printf("\t");scanf("%s",fname);
                fflush(stdin);
                doWhat(opr,fname);
              }
            }
}
int ExistsFile(char E_fname[])  //判断文件或目录是否存在，存在则返回1，不存在则返回0 
{
  int i,f=0;
  for(i=1;i<=i_length;i++)
  {
    if(strcmp(inode[i].name,E_fname)==0&&inode[i].flags==1)
    {f=1;break;}
  }
  if(f==1)return 1;
  if(f==0)return 0;
}
int lookupPiece(int len)       //找到有效可占用的空闲物理块号，将相应物理块标志位置1，并返回首物理块号 
{
  int n,f,f1=0,i,j;
  n=len/512;
  for(i=1;i<=ShanNum-11;i++)
  {
    if(Data[i].flags==0)
    {
      for(j=i+1;j<i+n;j++)
      {
        if(Data[j].flags==1){f=0;break;}
        else f=1;
      }
      if(f==1)
      {
        for(j=i;j<i+n;j++){Data[j].flags=1;}     
        f1=1;
        return Data[i].number;break;
      }
    }
  }
  if(f1==0)return 0;
}
int lookupInode()         //找到可用 inode编号并返回 
{
  int i;
  for(i=1;i<=i_length;i++)
  {
    if(inode[i].flags==0)
    {return i;break;}
  }
}
void relaDir(char Dir[],int i_number)        //将文件添加到相关目录块的索引表中 
{
  int i,n,j,k;
  n=inode[i_number].length/512;
  for(i=1;i<=i_length;i++)
    if(!strcmp(inode[i].name,Dir))
    {
      for(j=1;j<=ShanNum-11;j++)
        if(Data[j].number==inode[i].number)
        {
          for(k=1;k<=n_index;k++)
            if(Data[j].fd_number[k]==-1)break;
          Data[j].fd_number[k]=i_number;
          break;
        }
      break;
    }

}
void Wback_Fpnum(int i_number)   //删除文件或目录时，释放其所占用的物理块以及所属目录块索引表中的记录 
{
  int i,n,j,k;
  n=inode[i_number].length/512;
  for(j=1;j<=ShanNum;j++)
    if(Data[j].number==inode[i_number].number)
    {
      for(i=j;i<j+n;i++)
        Data[i].flags=0;
    }
  for(i=1;i<=i_length;i++)
  {
    if(inode[i].f_d=='d')
    {  for(j=1;j<=ShanNum;j++)
      if(inode[i].number==Data[j].number)
      {
        for(k=1;k<=n_index;k++)
          if(Data[j].fd_number[k]==i_number)
          {
            Data[j].fd_number[k]=-1;
          }
      }
    }
  }
}

void Filecreate(char F_fname[])
{
  int temp,tlength,i_number,h;
  char Dir[maxsize];
  if(F_fname[0]!='#')
  {
    printf("\t");
    printf("您设定的文件大小为：");
    scanf("%d",&tlength);fflush(stdin);
    while(tlength%512!=0){printf("\t");printf("文件大小设定有误！请重新输入："); scanf("%d",&tlength);}fflush(stdin);
    h=lookupPiece(tlength);
    if(h)
    {
      i_number=lookupInode();
      inode[i_number].number=h;
      inode[i_number].length=tlength;
      strcpy(inode[i_number].name,F_fname);
      inode[i_number].flags=1;
      inode[i_number].ci=h/8;
      inode[i_number].f_d='f';
      printf("\t");
      printf("默认目录：1------指定目录：2\n");
      printf("\t");
      scanf("%d",&temp);fflush(stdin);
      if(temp==1){relaDir(".",i_number);printf("\t");printf("文件创建成功！\n");}
      else if(temp==2)
      {
        printf("\t");printf("您指定的目录是：");
        scanf("%s",Dir);fflush(stdin);
        if(ExistsFile(Dir))
        {relaDir(Dir,i_number);printf("\t");printf("文件创建成功！\n");}
        else { printf("\t");printf("目录不存在！文件默认存储到根目录下..\n");relaDir(".",i_number);}
      }
      printf("\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
      printf("\t"); scanf("%s",opr);
      if(!strcmp(opr,"exit")){exit(1);}
      else 
      {  printf("\t");scanf("%s",fname);
        fflush(stdin);
        doWhat(opr,fname);
      }
    }
    else
    {
      printf("\t");printf("文件大小超出磁盘空间.文件创建失败！\n");
      printf("\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
      printf("\t");
      scanf("%s",opr);
      if(!strcmp(opr,"exit")){exit(1);}
      else 
      {  printf("\t");scanf("%s",fname);
        fflush(stdin);
        doWhat(opr,fname);
      }
    }
  }
  else{
    printf("\t");
    printf("无效的文件名！\n");
    printf("\t");
    scanf("%s",opr);
    if(!strcmp(opr,"exit")){exit(1);}
    else 
    {  printf("\t");scanf("%s",fname);
      fflush(stdin);
      doWhat(opr,fname);
    }
  }
}
void Filedelete(char D_fname[])
{
  int i;
  if(ExistsFile(D_fname))
  {
    for(i=1;i<=i_length;i++)
    {
      if(!strcmp(inode[i].name,D_fname))
      {
        inode[i].flags=0;
        Wback_Fpnum(i);
        break;
      }
    }

    printf("\t");printf("文件删除成功!\n");
  }
  else {printf("\t");printf("文件不存在！\n");}
  printf("\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
  printf("\t");scanf("%s",opr);
  if(!strcmp(opr,"exit")){exit(1);}
  else 
  {   printf("\t");scanf("%s",fname);
    fflush(stdin);
    doWhat(opr,fname);
  }
}
void Fileopen(char O_fname[])
{
  int n,i,j;

  if(ExistsFile(O_fname))
  {
    printf("\t文件打开成功！\n\t您可以写文件，读文件或者关闭文件：\n");
    for(i=1;i<=i_length;i++)
    {
      if(!strcmp(inode[i].name,O_fname))
      {
        j=1;
        while(1){
          if(Data[j].number==inode[i].number)
          {n=j;break;}
          else j++;
        }
        while(1)
        {
          printf("\t");scanf("%s",opr);    
          printf("\t");scanf("%s",fname);
          if(!strcmp(opr,"read"))
          { printf("\t");printf("%s",Data[n].Content);printf("\n");}
          else if(!strcmp(opr,"write"))
          {
            printf("\t");scanf("%s",Data[n].Content);printf("\t");
            printf("写成功！\n");
          }
          else if(!strcmp(opr,"close"))
          {break;}
          else {
            printf("\t命令输入错误！请查看使用说明！\n");
          }
        }
        break;
      } 
    }
  }
  else {printf("\t");printf("文件不存在！\n");}
  printf("\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
  printf("\t"); scanf("%s",opr);
  if(!strcmp(opr,"exit")){exit(1);}
  else 
  {   printf("\t");scanf("%s",fname);
    fflush(stdin);
    doWhat(opr,fname);
  }
}
void Filels(char L_fname[])
{
  int n,i,j=1,k=1,f,n1,h;
  if(ExistsFile(L_fname))
  {
    for(i=1;i<=i_length;i++)
    {
      if(!strcmp(inode[i].name,L_fname))
      {
        h=inode[i].length/512;
        while(1){
          if(Data[j].number==inode[i].number)
          {n=j;break;}
          else j++;
        }
        f=1;
        printf("\t");
        for(k=1;k<=n_index;k++)
        {
          n1=Data[n].fd_number[k];
          if(n1!=-1&&inode[n1].flags!=0)
          {

            printf("%s\t\t",inode[n1].name);
            f=0;
          }

        }
        if(f==1){ printf("..");}
        printf("\n");
        break;
      }
    }
  }
  else {printf("\t");printf("目录不存在！\n");}
  printf("\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
  printf("\t");scanf("%s",opr);
  if(!strcmp(opr,"exit")){exit(1);}
  else 
  {   printf("\t");scanf("%s",fname);
    fflush(stdin);
    doWhat(opr,fname);
  } 
}
void LS_lfile(char fname[])
{
  int n,i,j=1,k=1,f,n1,h;
  if(ExistsFile(fname))
  {
    for(i=1;i<=i_length;i++)
    {
      if(!strcmp(inode[i].name,fname))
      {
        h=inode[i].length/512;
        while(1){
          if(Data[j].number==inode[i].number)
          {n=j;break;}
          else j++;
        }
        f=1;
        printf("\t名称\t\t大小\t磁道号\t物理块号\t类型\n");
        for(k=1;k<=h*30;k++)
        {
          n1=Data[n].fd_number[k];
          if(n1!=-1&&inode[n1].flags!=0)
          {

            printf("\t%s\t\t%dB\t%d\t%d\t\t%c\n",inode[n1].name,inode[n1].length,inode[n1].ci,inode[n1].number,inode[n1].f_d);
            f=0;
          }

        }
        if(f==1){ printf("..");}
        printf("\n");
        break;
      }
    }
  }
  else {printf("\t");printf("目录不存在！\n");}
  printf("\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
  printf("\t");scanf("%s",opr);
  if(!strcmp(opr,"exit")){exit(1);}
  else 
  {   printf("\t");scanf("%s",fname);
    fflush(stdin);
    doWhat(opr,fname);
  }  
}
void createdir(char fname[])
{ 
  int temp,tlength,i_number,h;
  char Dir[maxsize];
  if(fname[0]=='#')
  {
    printf("\t");printf("您设定的目录文件大小为：");
    scanf("%d",&tlength);fflush(stdin);
    while(tlength%512!=0){ printf("\t");printf("目录文件大小设定有误！请重新输入：");   printf("\t");scanf("%d",&tlength);}fflush(stdin);
    h=lookupPiece(tlength);
    if(h)
    {
      i_number=lookupInode();
      inode[i_number].number=h;
      inode[i_number].length=tlength;
      strcpy(inode[i_number].name,fname);
      inode[i_number].flags=1;
      inode[i_number].ci=h/8;
      inode[i_number].f_d='d';     

      printf("\t");
      printf("默认目录：1------指定目录：2\n");
      printf("\t");
      scanf("%d",&temp);fflush(stdin);
      if(temp==1){relaDir(".",i_number);printf("\t");printf("目录创建成功！\n");}
      else if(temp==2)
      {
        printf("\t");
        printf("您指定的目录是：");
        scanf("%s",Dir);fflush(stdin);
        if(ExistsFile(Dir))
        {relaDir(Dir,i_number);printf("\t");printf("目录创建成功！\n");}
        else { printf("\t");printf("目录不存在！文件默认存储到根目录下..\n");relaDir(".",i_number);}
      }
      printf("\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
      printf("\t");
      scanf("%s",opr);
      if(!strcmp(opr,"exit")){exit(1);}
      else 
        printf("\t");
      { scanf("%s",fname);
        fflush(stdin);
        doWhat(opr,fname);
      }
    }
    else
    {
      printf("\t");printf("目录文件大小超出磁盘空间.目录创建失败！\n");
      printf("\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
      printf("\t");scanf("%s",opr);
      if(!strcmp(opr,"exit")){exit(1);}
      else 
      {  printf("\t");scanf("%s",fname);
        fflush(stdin);
        doWhat(opr,fname);
      }
    }
  }
  else
  {
    printf("\t");
    printf("无效的目录名！\n");
    printf("\t");scanf("%s",opr);
    if(!strcmp(opr,"exit")){exit(1);}
    else 
    {  printf("\t");scanf("%s",fname);
      fflush(stdin);
      doWhat(opr,fname);
    }
  }
}
void re_deldir(char fname[])
{
  int i,j=1,k=1,n,temp;
  for(i=1;i<=i_length;i++)
  {
    if(inode[i].f_d=='d'&&strcmp(inode[i].name,fname)==0)
    {
      inode[i].flags=0; 
      while(1){
        if(Data[j].number==inode[i].number)
        {n=j;break;}
        else j++;
      }
      while(Data[n].fd_number[k]!=-1)
      {
        temp=Data[n].fd_number[k];
        if(inode[temp].f_d=='d'){re_deldir(inode[temp].name);}
        else{
          inode[temp].flags=0;
          Wback_Fpnum(temp);
        }
        k++;
      }
      Wback_Fpnum(i); 
    }  
  }
  return;     
}
void deletedir(char fname[])
{
  int i,j=1,k=1,n,temp;
  if(ExistsFile(fname))
  {
    for(i=1;i<=i_length;i++)
    {
      if(inode[i].f_d=='d'&&strcmp(inode[i].name,fname)==0)
      {
        inode[i].flags=0; Wback_Fpnum(i); 
        while(1){
          if(Data[j].number==inode[i].number)
          {n=j;break;}
          else j++;
        }
        while(Data[n].fd_number[k]!=-1)
        {
          temp=Data[n].fd_number[k];
          if(inode[temp].f_d=='d'){re_deldir(inode[temp].name);}
          else{
            inode[temp].flags=0;
            Wback_Fpnum(temp);
          }

          k++;
        }

      }
    }
    printf("\t目录删除成功！\n");
  }
  else {printf("\t");printf("目录不存在！\n");}
  printf("\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\n");
  printf("\t");scanf("%s",opr);
  if(!strcmp(opr,"exit")){exit(1);}
  else 
  {   printf("\t");scanf("%s",fname);
    fflush(stdin);
    doWhat(opr,fname);
  }
}
void InitSpace()
{
  int i,k=1,h=12,n,j;
  char temp2[10]=".";


  sup.Pn_inode=10;                               //inode表所占盘块数
  sup.Pn_fdata=ShanNum-sup.Pn_inode-1;           //文件数据所占盘块数
  sup.fn_piece=309;                              //主存中登记空闲盘块数
  sup.fn_inode=(512/16)*10;                      //主存中登记空闲inode数，其中一条inode记录占16个字节
  //超级块的其他文件管理控制信息在本系统中未用到，就不再写出

  n_index=512/8;                                 //目录块的索引表中一条索引记录占8个字节，则此目录块中可存放512/8条索引记录
  i_length=sup.fn_inode;
  for(i=1;i<=ShanNum-11;i++)                     //初始化物理块，即将其标志位置为0
    Data[i].flags=0;
  for(i=1;i<=i_length;i++)                       //初始化inode表，即将其标志位置为0
    inode[i].flags=0;

  for(i=1;i<=ShanNum-11;i++)                     //初始化目录块索引表
  {
    for(k=1;k<=n_index;k++)
      Data[i].fd_number[k]=-1;
  }
  for(i=1;i<=ShanNum-11;i++)                      //对数据块编号
  {
    Data[i].number=h;
    h++;
  }

  inode[1].length=2048;                            //初始化根目录区，长度为2048个字节
  strcpy(inode[1].name,temp2);                     //名称为“.”
  n=2048/512;
  for(j=1;j<1+n;j++)                                //将对应块标志位置1，表示已占用
    Data[j].flags=1;            

  inode[1].flags=1;                                  //设置对应根目录区德inode表相关信息
  inode[1].f_d='d';
  inode[1].number=Data[1].number;
  inode[1].ci=Data[1].number/8;	 
}
int main()
{

  printf("********************************************************************************\n");
  printf("\t\t\t欢迎使用Simple文件系统！\n");
  printf("********************************************************************************\n");
  printf("\t--------------------------------------------------------------\n");
  printf("\t|\t使用说明：\t\t\t\t\t     |\n\t|\t\t\t\t\t\t\t     |\n");
  printf("\t|\t\t创建文件：create<br>文件名\t\t     |\n");
  printf("\t|\t\t删除文件：delete<br>文件名\t\t     |\n");
  printf("\t|\t\t打开文件：open<br>文件名\t\t     |\n");
  printf("\t|\t\t读文件：  read<br>文件名\t\t     |\n");
  printf("\t|\t\t写文件：  write<br>文件名\t\t     |\n");
  printf("\t|\t\t关闭文件：close<br>文件名\t\t     |\n");
  printf("\t|\t\t查看文件：ls<br>目录名\t\t\t     |\n");
  printf("\t|\t\t查看文件详细信息：ls_l<br>目录名\t     |\n");
  printf("\t|\t\t创建目录：createdir<br>目录名\t\t     |\n");
  printf("\t|\t\t删除目录：deletedir<br>目录名\t\t     |\n");
  printf("\t|\t\t退出系统：exit<br>\t\t\t     |\n\t|\t\t\t\t\t\t\t     |\n");
  printf("\t|\t注：\t\t\t\t\t\t     |\n\t|\t\t1.目录文件形式为 #文件名\t\t     |\n");
  printf("\t|\t\t2.文件大小必须是512B的整数倍\t\t     |\n");
  printf("\t|\t\t3.当前目录默认为“.”\t\t\t     |\n");
  printf("\t--------------------------------------------------------------\n\n");
  printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");

  InitSpace();
  printf("\t你要做什么？\n");printf("\t");
  scanf("%s",opr);
  if(!strcmp(opr,"exit")){exit(1);}
  else 
  { printf("\t");
    scanf("%s",fname);
    fflush(stdin);
    doWhat(opr,fname);
  }
  /*getch();*/
  return 0;
}
/*1.程序通过调用一个递归函数re_deldir()完成多级目录的删除.*/
/*2.对文件以及目录的命名加强了限制：即普通文件不能以'#'开头，目录文件必须以'#'开头.*/
/*不足：虽然目录块的索引表可占有多个物理块，但在具体实现时，仅对一块进行了查询.在查找空闲块时，用了first_fit算法而不是磁盘调度算法*/
/*补充：对于物理空间分配情况的查看，可通过使用ls_l命令查看相应物理块号，结合块长和文件长度进行推算得知.*/
