#include <stdio.h>
#include <stdlib.h>
#include <math.h>
void main(){
	FILE *fp1,*fp2;
	if((fp1=fopen("C:\\home\\wmx\\myLU\\outputCPU\\J118LU.txt","rt+"))==NULL){
		printf("Cannot open file strike any key exit!");
		getchar();
		exit(1);
	}
	if((fp2=fopen("C:\\home\\wmx\\myLU\\output\\J118LU_DAG.txt","rt+"))==NULL){
		printf("Cannot open file strike any key exit!");
		getchar();
		exit(1);
	}
	int N,nnzU,nnzL;
	double v1,v2;
	int a1,a2;
	fscanf(fp1,"%d\n",&N);
	fscanf(fp1,"%d\n",&nnzU);
	fscanf(fp2,"%d\n",&N);
	fscanf(fp2,"%d\n",&nnzU);
	for(int i=0;i<nnzU;i++){
		fscanf(fp1,"%lf",&v1);
		fscanf(fp2,"%lf",&v2);
		if(abs(v1-v2)>1e-4){
			printf("U:%d %f %f\n",i,v1,v2);
		}
	}
	for(int i=0;i<nnzU;i++){
		fscanf(fp1,"%d",&a1);
		fscanf(fp2,"%d",&a2);
		if(a1!=a2){
			printf("%d %f %f\n",i,a1,a2);
			fclose(fp1);
	        fclose(fp2);
			return;
		}
	}
	for(int i=0;i<(N+1);i++){
		fscanf(fp1,"%d",&a1);
		fscanf(fp2,"%d",&a2);
		if(a1!=a2){
			printf("%d %f %f\n",i,a1,a2);
			fclose(fp1);
	        fclose(fp2);
			return;
		}
	}
	fscanf(fp1,"%d\n",&nnzL);
	fscanf(fp2,"%d\n",&nnzL);
	for(int i=0;i<nnzL;i++){
		fscanf(fp1,"%lf",&v1);
		fscanf(fp2,"%lf",&v2);
		if(abs(v1-v2)>1e-4){
			printf("L:%d %f %f\n",i,v1,v2);
		}
	}
	for(int i=0;i<nnzL;i++){
		fscanf(fp1,"%d",&a1);
		fscanf(fp2,"%d",&a2);
		if(a1!=a2){
			printf("%d %f %f\n",i,a1,a2);
			fclose(fp1);
	        fclose(fp2);
			return;
		}
	}
	for(int i=0;i<(N+1);i++){
		fscanf(fp1,"%d",&a1);
		fscanf(fp2,"%d",&a2);
		if(a1!=a2){
			printf("%d %f %f\n",i,a1,a2);
			fclose(fp1);
	        fclose(fp2);
			return;
		}
	}
	fclose(fp1);
	fclose(fp2);
	return;
}