#include<stdio.h>
#include<string.h>
#include<stdlib.h>
void init();
void getsym();
/* 符号 */
enum symbol
{
	nul,ident,number,plus,minus,times,slash,oddsym,eql,neq,
	lss,leq,gtr,geq,lparen,rparen,comma,semicolon,period,becomes,
	beginsym,endsym,ifsym,thensym,whilesym,writesym,readsym,dosym,callsym,
	constsym,varsym,procsym,programsym,
};
#define norw 14           /*关键字个数*/
#define al 10            //符号的最大的长度 
#define nmax 10         //number的最大位数
char word [norw][al];  //保留字13个
char ch;               /* 获取字符的缓冲区 */
enum symbol sym;         /* 当前的符号 */
enum symbol wsym[norw];    /* 保留字对应的符号值 */
enum symbol ssym[256];    //单字符的符号值
FILE *fin,*fout;
int main()
{
	printf("请输入结果要保存的文件:\n");    //词法分析之后的结果要输出到的文件
	char outname[30],inname[30];
	scanf("%s",outname);
	if((fout=fopen(outname,"w"))==NULL)
	{
		printf("不能打开文件!");
		exit(0);
	}
	printf("请输入需要词法分析的源程序:\n");     //需要词法分析的源程序
	scanf("%s",inname);
	if((fin=fopen(inname,"r"))==NULL)
	{
		printf("不能打开文件!\n");
		exit(0);
	}
	ch=fgetc(fin);
	while(ch!=EOF) //EOF实际是-1，用来表示文本文件的结束 
	{
		getsym();
	}
}
//读取源文件
void getsym()
{
	long  m;
    char id[al+1],a[al+1];
	int  i,k;
	init();
	if(ch==' '||ch==10||ch==9)         /*忽略空格、换行和TAP*/
	{
		ch=fgetc(fin);
	}
	else
	{
		if(ch>='a'&&ch<='z'||ch>='A'&&ch<='Z')   /*名字或保留字以a..z开头*/
		{
			k=0;
		    do                                    /*搜索当前符号是否为保留字*/
			{
				if(k<al)
				{
					a[k]=ch;
			       	k++;
				}
				ch=fgetc(fin);
			}while ((ch>='a'&&ch<='z'||ch>='0'&&ch<='9')||(ch>='A'&&ch<='Z'||ch>='0'&&ch<='9'));
			a[k]=0;
		    strcpy(id,a);
		    //将识别出来的字符和已定义的标示符作比较， 判断是否是关键字
		    for(i=0;i<norw;i++)
			{
				if(strcmp(id,word[i])==0)
				{
					sym=wsym[i];
                    printf("%s       %ssym\n",id,id);
                    fprintf(fout,"%s       %ssym\n",id,id);
					break;
				}
				else
				{
					if(i==(norw-1))
					{
						sym=ident;
					    printf("%s          ident\n",id);
					    fprintf(fout,"%s          ident\n",id);
					}
				}
			}
		}
		else
		{
			if(ch>='0'&&ch<='9')   /*检测是否为数字：以0..9开头*/
			{
				k=0;
			    m=0;
			    sym=number;
			    do
				{
					m=10*m+ch-'0';
				    k++;
				    ch=getc(fin);
				}while(ch>='0'&&ch<='9');     /*获取数字的值*/
				if(k>=nmax)
				{
					printf("     error!数值太大！     \n");
					//exit(0);
				}
				printf("%d        number\n",m);
				fprintf(fout,"%d        number\n",m);
			}
	       	else
			{
				if(ch==':')              /*检测赋值符号*/
				{
		           	ch=getc(fin);
					if(ch=='=')
					{
						sym=becomes;
					    printf(":=         becomes\n");
						fprintf(fout,":=         becomes\n");
					    ch=fgetc(fin);
					}
					else
					{
						sym=nul;
					    printf(":            nul\n");                     /*不能识别的符号*/
						fprintf(fout,":            nul\n");
					}
				}
				else
				{
					if(ch=='<')                                          /*检测小于或小于等于符号*/
					{
						ch=fgetc(fin);
					    if(ch=='=')
						{
							sym=leq;
					       	printf("<=            lep\n");
							fprintf(fout,"<=            lep\n");
					        ch=fgetc(fin);
						}
				        else
						{
							sym=lss;
					      	printf("<            lss\n");
							fprintf(fout,"<            lss\n");
						}
					}
				    else
					{
						if(ch=='>')                         /*检测大于或大于等于符号*/
						{
							ch=fgetc(fin);
							if(ch=='=')
							{
								sym=geq;
						       	printf(">=            geq\n");
								fprintf(fout,">=            geq\n");
					           	ch=fgetc(fin);
							}
							else
							{
								sym=gtr;
							    printf(">            gtr\n");
						        fprintf(fout,">            gtr\n");
							}
						}
						else     /*当符号不满足上述条件时，全部按照单字符符号处理*/
						{
							sym=ssym[ch];
							if(ch=='+'){printf("+           plus\n");fprintf(fout,"+           plus\n");ch=fgetc(fin);}
							else
							if(ch=='-'){printf("-          minus\n");fprintf(fout,"-          minus\n");ch=fgetc(fin);}
							else
							if(ch=='*'){printf("*          times\n");fprintf(fout,"*          times\n");ch=fgetc(fin);}
							else
							if(ch=='/'){printf("/          slash\n");fprintf(fout,"/          slash\n");ch=fgetc(fin);}
							else
							if(ch=='('){printf("(         lparen\n");fprintf(fout,"(         lparen\n");ch=fgetc(fin);}
							else
							if(ch==')'){printf(")         rparen\n");fprintf(fout,")         rparen\n");ch=fgetc(fin);}
							else
							if(ch=='='){printf("=            eql\n");fprintf(fout,"=            eql\n");ch=fgetc(fin);}
							else
							if(ch==','){printf(",          comma\n");fprintf(fout,",          comma\n");ch=fgetc(fin);}
							else
							if(ch=='#'){printf("#            neq\n");fprintf(fout,"#            neq\n");ch=fgetc(fin);}
							else
							if(ch=='.'){printf(".         period\n");fprintf(fout,".         period\n");ch=fgetc(fin);}
							else
							if(ch==';'){printf(";       semicoln\n");fprintf(fout,";       semicoln\n");ch=fgetc(fin);}
							else{printf("%c            nul\n",ch);fprintf(fout,"%c            nul\n",ch);ch=fgetc(fin);}
						}
					}
				}
			}
		}
	}
}
//对关键字等实现初始化
void init()
{
	/*设置单字符符号*/
	int i;
	for(i=0;i<=255;i++)
	{
		ssym[i]=nul;
	}
	ssym['+']=plus;
	ssym['-']=minus;
	ssym['*']=times;
	ssym['/']=slash;
	ssym['(']=lparen;
	ssym[')']=rparen;
	ssym['=']=eql;
	ssym[',']=comma;
	ssym['.']=period;
	ssym['#']=neq;
	ssym[';']=semicolon;
 /*设置保留字名字，按照字母表顺序，便于折半查找*/
	strcpy(&(word[0][0]),"begin");
	strcpy(&(word[1][0]),"call");
	strcpy(&(word[2][0]),"const");
	strcpy(&(word[3][0]),"do");
	strcpy(&(word[4][0]),"end");
	strcpy(&(word[5][0]),"if");
	strcpy(&(word[6][0]),"odd");
	strcpy(&(word[7][0]),"procedure");
	strcpy(&(word[8][0]),"read");
	strcpy(&(word[9][0]),"program");
	strcpy(&(word[10][0]),"var");
	strcpy(&(word[11][0]),"whlie");
	strcpy(&(word[12][0]),"write");
	strcpy(&(word[13][0]),"then");
/*设置保留字符号*/
	wsym[0]=beginsym;
	wsym[1]=callsym;
	wsym[2]=constsym;
	wsym[3]=dosym;
	wsym[4]=endsym;
	wsym[5]=ifsym;
	wsym[6]=oddsym;
	wsym[7]=procsym;
	wsym[8]=readsym;
	wsym[9]=programsym;
	wsym[10]=varsym;
	wsym[11]=whilesym;
	wsym[12]=writesym;
	wsym[13]=thensym;
}
