#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//-1��ʾû��˫�ף�0��ʾ��˫��
typedef struct Htnode {//���������Ľ��
	char ch;
	double weight;
	int lchild,rchild,parent;
}Htnode,*HuffmanT;
typedef struct Hfmcode {
	char* code;//����
	char ch;//������ַ�
}Hfmcode,*Huffmcodef;
int FMin(HuffmanT& T,int i) {//Ѱ����ȨֵС�Ľ�㣬i��n��ʼ
	int  k,count = 0;
	double min;
	while (T[count].parent != -1) {//�ҵ�û��˫�׵Ľ��
		count++;
	}
	min = T[count].weight;
	k = count;
	for (int j = count + 1;j < i;j++) {//�����������ҳ���С��Ȩֵ
		if (T[j].parent == -1 && T[j].weight < min) {//һ����û��˫�׵�
			min = T[j].weight;
			k = j;
		}
	}
	T[k].parent = 1;
	return k;
}
void select(HuffmanT& T, int i, int& p1, int& p2) {//ѡ����С��������㸴�Ƶ�p1��p2��
	int p;
	p1 = FMin(T, i);
	p2 = FMin(T, i);
	if (p1 > p2) {
		p = p1;
		p1 = p2;
		p2 = p;
	}
}
void InitHfmtree(HuffmanT &T,int n) {//����������ʼ��
	int t;
	T = (Htnode*)malloc((2*n-1)*sizeof(Htnode));//�ɶ����������ʵó�ʱ2n-1
	if (T == NULL) {
		printf("�ڴ����ʧ�ܣ�");
		exit(1);
	}
	for (int i = 0;i < 2 * n - 1;i++) {
		T[i].ch=' ';
		T[i].parent = T[i].lchild = T[i].rchild = -1;//-1����û��˫�ף����ڵ�
	}//���н�㸳ֵ��ʼ��,���ǵ����Ľ�㣬��˫��
	for (int i = 0;i <n;i++) {//����ÿһ���������б���
		printf("�������%d���ַ�", i + 1);
		t=getchar();//���ǰ���/nʹ���������ַ�
		scanf_s("%c", &T[i].ch,10);
		printf("������Ȩֵ�����ʣ�");
		scanf_s("%lf", &T[i].weight);
	}//�����ַ���
	int lnode, rnode;
	for (int i = n;i < 2 * n - 1;i++) {//������С�Ľ�����Ȼ����뵽����������
		select(T, i, lnode, rnode);
		T[i].lchild = lnode;
		T[i].rchild = rnode;
		T[lnode].parent =T[rnode].parent = i;
		T[i].weight = T[lnode].weight + T[rnode].weight;
	}
	printf("��ʼ�����������ɹ���\n");
	printf("����������ʾ���£�\n");
	printf("���\t�ַ�\tȨֵ\t����\t�Һ���\t˫��\n");
	for (int i = 0;i < 2 * n - 1;i++) {
		printf("%d\t%c\t%.2lf\t%d\t%d\t%d\n", i, T[i].ch, T[i].weight, T[i].lchild, T[i].rchild, T[i].parent);
	}
}
HuffmanT HuffmanFile(int& n) {
	FILE* fp;
	fp = fopen("hfmTree.txt", "r");
	if (fp == NULL) {
		printf("�ļ���ʧ�ܣ����Ƚ������������ļ�\n");
		return 0;
	}
	int line = 0;
	while (!feof(fp)) {//feof�ж��ļ��Ƿ����
		if (fgetc(fp) == '\n') {
			line++;
		}
	}
	//printf("%d\n", line);
	rewind(fp);//�ļ�ָ��ָ���ļ���ͷ
	HuffmanT T;
	T = (Htnode*)malloc(line * sizeof(Htnode));
	if (T == NULL) {
		printf("�ڴ����ʧ�ܣ�");
		exit(1);
	}
	int lchild, rchild, parent;
	for (int i = 0;i < line;i++) {
		int result = fscanf(fp, "%d\t%c\t%lf\t%d\t%d\t%d\n", &i, &T[i].ch, &T[i].weight, &lchild, &rchild, &parent);
		T[i].lchild = lchild;
		T[i].rchild = rchild;
		T[i].parent = parent;//�����Ҫ��ָ��
		if (result != 6) {
			printf("��ȡ�ļ�ʧ�ܣ�");
			exit(1);
		}

	}
	fclose(fp);
	n = (line+1)/2;
	return T;

}
void charsethuffmanEncoding(HuffmanT &T,Huffmcodef& H,int &n) {//���������������
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
		for(c=i,f=T[i].parent;f!=-1;c=f,f=T[f].parent){//��Ҷ�����������
			cd[--start] = (T[f].lchild == c) ? '0' : '1';
		}
		H[i].code = (char*)malloc((n - start) * sizeof(char));
		for (int j = start;j < n;j++) {
			H[i].code[j - start] = cd[j];
		}
	}
	
	printf("��������������£�\n");
	printf("�ַ�\t����\n");
	for (int i = 0;i < n;i++) {
		printf("%c\t%s\n", H[i].ch,H[i].code);
	}
	free(cd);
}
void Encode(Huffmcodef H,int n,FILE *fp) {//������벢���������ļ�
	char str[50] = {'\0'};
	errno_t err;
	printf("������Ҫ��������ݣ�");
	int t = getchar();
	gets_s(str);
	if ((err = fopen_s(&fp, "CodeFile.txt", "w") != 0)) {
		printf("�޷����ļ�\n");
		exit(0);
	}
	else {
		printf("�����\n");
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
	printf("\n�ɹ�����CodeFile.txt�ļ�\n");

}
void Decode(HuffmanT T, int n) {
	FILE* fp2 = fopen("TextFile.txt", "w");
	FILE* fp1 = fopen("CodeFile.txt", "r");
	char str[1000] = { '\0' };
	int i = 0;
	if ((fp1 ==NULL)||(fp2==NULL)) {
		printf("�޷����ļ�\n");
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
		printf("CodeFile�ļ��б���Ϊ��");
		printf("%s", str);
		printf("\n������Ϊ��");
		int q = 2 * n - 2;//qһ��ʼ���ڸ��ڵ�
		for (int i = 0;i < strlen(str);i++) {//�Ӹ��ڵ������0����֧��1����֧���ߵ�Ҷ�ӽ���ó��ַ�
			if (str[i] == '0') {
				q = T[q].lchild;
			}
			else if (str[i] == '1') {
				q = T[q].rchild;
			}
			if (T[q].lchild == -1 && T[q].rchild == -1) {//���ҽڵ㶼Ϊ�ձ�ʾҶ�ӽ��
				printf("%c", T[q].ch);
				fprintf(fp2, "%c", T[q].ch);
				q = 2 * n - 2;//Ҫ���´Ӹ��ڵ㿪ʼѰ�ң�������������
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
		printf("                     ��������������ϵͳ\n");
		printf("-----------------------------------------------------------\n");
		printf("1����ʼ������������������\n");
		printf("2�����롣\n");
		printf("3�����롣\n");
		printf("����0�˳�����\n");
		printf("===========================================================\n");
		printf("������ָ�0��1��2��3����");
		scanf_s("%d", &choice);
		switch (choice)
		{case 1:
			printf("�������ַ�����С��");
			scanf_s("%d", &n);
			if (n > 1) {
				InitHfmtree(T, n);
				if ((err = fopen_s(&fp, "hfmTree.txt", "w") != 0)) {
					printf("�޷����ļ�");
					exit(0);
				}
				for (int i = 0;i < 2 * n - 1;i++) {
					fprintf(fp, "%d\t%c\t%.2lf\t%d\t%d\t%d\n", i, T[i].ch, T[i].weight, T[i].lchild, T[i].rchild, T[i].parent);
				}
				fclose(fp);
				printf("�ɹ�����hfmTree.txt�ļ���\n");
				break;
			}
			else {
				printf("��������ȷ���ַ�����С��\n");
				break;
			}
		case 2:
				charsethuffmanEncoding(T, H,n);
				Encode(H,n,fp);
				break;
		case 3:
			if (n == 0) {
				printf("���Ƚ��г�ʼ�����ã�\n");
				break;
			}
			else {
				Decode(T, n);
				break;
			}
		case 0:
			break;
		default:
			printf("��������ȷ�Ĺ��ܣ�\n");
			break;
		}
			
			
		}
	return 0;
}