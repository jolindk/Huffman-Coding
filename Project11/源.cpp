#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//-1表示没有双亲，0表示有双亲
typedef struct Htnode {//哈夫曼树的结点
	char ch;
	double weight;
	int lchild,rchild,parent;
}Htnode,*HuffmanT;
typedef struct Hfmcode {
	char* code;//编码
	char ch;//编码的字符
}Hfmcode,*Huffmcodef;
int FMin(HuffmanT& T,int i) {//寻找最权值小的结点，i从n开始
	int  k,count = 0;
	double min;
	while (T[count].parent != -1) {//找到没有双亲的结点
		count++;
	}
	min = T[count].weight;
	k = count;
	for (int j = count + 1;j < i;j++) {//历遍整个来找出最小的权值
		if (T[j].parent == -1 && T[j].weight < min) {//一定是没有双亲的
			min = T[j].weight;
			k = j;
		}
	}
	T[k].parent = 1;
	return k;
}
void select(HuffmanT& T, int i, int& p1, int& p2) {//选择最小的两个结点复制到p1，p2中
	int p;
	p1 = FMin(T, i);
	p2 = FMin(T, i);
	if (p1 > p2) {
		p = p1;
		p1 = p2;
		p2 = p;
	}
}
void InitHfmtree(HuffmanT &T,int n) {//哈夫曼树初始化
	int t;
	T = (Htnode*)malloc((2*n-1)*sizeof(Htnode));//由二叉树的性质得出时2n-1
	if (T == NULL) {
		printf("内存分配失败！");
		exit(1);
	}
	for (int i = 0;i < 2 * n - 1;i++) {
		T[i].ch=' ';
		T[i].parent = T[i].lchild = T[i].rchild = -1;//-1代表没有双亲，根节点
	}//所有结点赋值初始化,都是单个的结点，无双亲
	for (int i = 0;i <n;i++) {//输入每一个结点进入列表中
		printf("请输入第%d个字符", i + 1);
		t=getchar();//清除前面的/n使其能输入字符
		scanf_s("%c", &T[i].ch,10);
		printf("请输入权值（概率）");
		scanf_s("%lf", &T[i].weight);
	}//输入字符集
	int lnode, rnode;
	for (int i = n;i < 2 * n - 1;i++) {//两个最小的结点相加然后加入到哈夫曼树中
		select(T, i, lnode, rnode);
		T[i].lchild = lnode;
		T[i].rchild = rnode;
		T[lnode].parent =T[rnode].parent = i;
		T[i].weight = T[lnode].weight + T[rnode].weight;
	}
	printf("初始化哈夫曼树成功！\n");
	printf("哈夫曼树表示如下：\n");
	printf("序号\t字符\t权值\t左孩子\t右孩子\t双亲\n");
	for (int i = 0;i < 2 * n - 1;i++) {
		printf("%d\t%c\t%.2lf\t%d\t%d\t%d\n", i, T[i].ch, T[i].weight, T[i].lchild, T[i].rchild, T[i].parent);
	}
}
HuffmanT HuffmanFile(int& n) {
	FILE* fp;
	fp = fopen("hfmTree.txt", "r");
	if (fp == NULL) {
		printf("文件打开失败！请先建立哈夫曼树文件\n");
		return 0;
	}
	int line = 0;
	while (!feof(fp)) {//feof判断文件是否结束
		if (fgetc(fp) == '\n') {
			line++;
		}
	}
	//printf("%d\n", line);
	rewind(fp);//文件指针指向文件开头
	HuffmanT T;
	T = (Htnode*)malloc(line * sizeof(Htnode));
	if (T == NULL) {
		printf("内存分配失败！");
		exit(1);
	}
	int lchild, rchild, parent;
	for (int i = 0;i < line;i++) {
		int result = fscanf(fp, "%d\t%c\t%lf\t%d\t%d\t%d\n", &i, &T[i].ch, &T[i].weight, &lchild, &rchild, &parent);
		T[i].lchild = lchild;
		T[i].rchild = rchild;
		T[i].parent = parent;//输入的要是指针
		if (result != 6) {
			printf("读取文件失败！");
			exit(1);
		}

	}
	fclose(fp);
	n = (line+1)/2;
	return T;

}
void charsethuffmanEncoding(HuffmanT &T,Huffmcodef& H,int &n) {//建立哈夫曼编码表
	if (T == NULL) {
		T=HuffmanFile(n);
	}
	H = (Huffmcodef)malloc(n* sizeof(Hfmcode*));
	int c,f,start;
	char* cd = (char*)malloc(n * sizeof(char));
	cd[n-1]='\0';
	for (int i = 0;i < n;i++) {
		start = n-1;
		H[i].ch = T[i].ch;
		for(c=i,f=T[i].parent;f!=-1;c=f,f=T[f].parent){//从叶子逆向求编码
			cd[--start] = (T[f].lchild == c) ? '0' : '1';
		}
		H[i].code = (char*)malloc((n - start) * sizeof(char));
		for (int j = start;j < n;j++) {
			H[i].code[j - start] = cd[j];
		}
	}
	
	printf("哈夫曼编码表如下：\n");
	printf("字符\t编码\n");
	for (int i = 0;i < n;i++) {
		printf("%c\t%s\n", H[i].ch,H[i].code);
	}
	free(cd);
}
void Encode(Huffmcodef H,int n,FILE *fp) {//输入编码并创建编码文件
	char str[50] = {'\0'};
	errno_t err;
	printf("请输入要编译的内容：");
	int t = getchar();
	gets_s(str);
	if ((err = fopen_s(&fp, "CodeFile.txt", "w") != 0)) {
		printf("无法打开文件\n");
		exit(0);
	}
	else {
		printf("编码后：\n");
		for (int i = 0;i < strlen(str);i++) {
			for (int j = 0;j < n;j++) {
				if (str[i] == H[j].ch) {
					printf("%s", H[j].code);
					fprintf(fp, "%s", H[j].code);
				}
			}
		}
	}
	fclose(fp);
	printf("\n成功创建CodeFile.txt文件\n");

}
void Decode(HuffmanT T, int n) {
	FILE* fp2 = fopen("TextFile.txt", "w");
	FILE* fp1 = fopen("CodeFile.txt", "r");
	char str[1000] = { '\0' };
	int i = 0;
	if ((fp1 ==NULL)||(fp2==NULL)) {
		printf("无法打开文件\n");
		exit(0);
	}
	else {
		
		while (true) {
			char c = getc(fp1);
			if (c == EOF) {
				break;
			}
			str[i++] = c;
		}
		printf("CodeFile文件中编码为：");
		printf("%s", str);
		printf("\n翻译结果为：");
		int q = 2 * n - 2;//q一开始等于根节点
		for (int i = 0;i < strlen(str);i++) {//从根节点出发，0走左支，1走右支，走到叶子结点便得出字符
			if (str[i] == '0') {
				q = T[q].lchild;
			}
			else if (str[i] == '1') {
				q = T[q].rchild;
			}
			if (T[q].lchild == -1 && T[q].rchild == -1) {//左右节点都为空表示叶子结点
				printf("%c", T[q].ch);
				fprintf(fp2, "%c", T[q].ch);
				q = 2 * n - 2;//要重新从根节点开始寻找！！！！！！！
			}

		}
		printf("\n");
	}
	fclose(fp1);
	fclose(fp2);
}
int main() {
	int n=-1;
	HuffmanT T = {0};
	Huffmcodef H;
	FILE* fp=0;
	errno_t err;
	int choice=-1;
	while(choice!=0){
		printf("===========================================================\n");
		printf("                     哈夫曼编译码器系统\n");
		printf("-----------------------------------------------------------\n");
		printf("1、初始化，建立哈夫曼树。\n");
		printf("2、编码。\n");
		printf("3、译码。\n");
		printf("输入0退出程序。\n");
		printf("===========================================================\n");
		printf("请输入指令（0、1、2、3）：");
		scanf_s("%d", &choice);
		switch (choice)
		{case 1:
			printf("请输入字符集大小：");
			scanf_s("%d", &n);
			if (n > 1) {
				InitHfmtree(T, n);
				if ((err = fopen_s(&fp, "hfmTree.txt", "w") != 0)) {
					printf("无法打开文件");
					exit(0);
				}
				for (int i = 0;i < 2 * n - 1;i++) {
					fprintf(fp, "%d\t%c\t%.2lf\t%d\t%d\t%d\n", i, T[i].ch, T[i].weight, T[i].lchild, T[i].rchild, T[i].parent);
				}
				fclose(fp);
				printf("成功创建hfmTree.txt文件！\n");
				break;
			}
			else {
				printf("请输入正确的字符集大小！\n");
				break;
			}
		case 2:
				charsethuffmanEncoding(T, H,n);
				Encode(H,n,fp);
				break;
		case 3:
			if (n == 0) {
				printf("请先进行初始化设置！\n");
				break;
			}
			else {
				Decode(T, n);
				break;
			}
		case 0:
			break;
		default:
			printf("请输入正确的功能！\n");
			break;
		}
			
			
		}
	return 0;
}