//v2:可以正确计算所有,效率较低,没有使用共享内存

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <Windows.h>
#include <conio.h>

int myLU_analysis(double *vU,int *colU,int *rowU,int nnzU,double *vL,int *rowL,int *colL,int nnzL,int N,int *father,int* fatherNUM,int *level,int *levelNUM,int *nnz){
	int *l=(int *)malloc(N*sizeof(int));
	for(int i=0;i<N;i++)
		l[i]=0;
	for(int i=0;i<N;i++){
		//int flag=0;
		for(int j=rowU[i]+1;j<rowU[i+1];j++){
			if(l[i]+1>l[colU[j]]){
				l[colU[j]]=l[i]+1;
				fatherNUM[colU[j]]=1;
				father[colU[j]*N+fatherNUM[colU[j]]-1]=i;
			}
			if((l[i]+1==l[colU[j]])&&(father[colU[j]*N+fatherNUM[colU[j]]-1]!=i)){
				fatherNUM[colU[j]]++;
				father[colU[j]*N+fatherNUM[colU[j]]-1]=i;
			}
			//if((l[i]+1==l[colU[j]])&&(father[colU[j]]!=i)){
			//	l[i]=l[colU[j]];
			//	father[i]=father[colU[j]];
			//	l[colU[j]]++;
			//	father[colU[j]]=i;
			//	i--;
			//	flag=1;
			//}
		}
		//if(flag) 
		//	continue;
		for(int j=colL[i]+1;j<colL[i+1];j++){
			if(l[i]+1>l[rowL[j]]){
				l[rowL[j]]=l[i]+1;
				fatherNUM[rowL[j]]=1;
				father[rowL[j]*N+fatherNUM[rowL[j]]-1]=i;
			}
			if((l[i]+1==l[rowL[j]])&&(father[rowL[j]*N+fatherNUM[rowL[j]]-1]!=i)){
				fatherNUM[rowL[j]]++;
				father[rowL[j]*N+fatherNUM[rowL[j]]-1]=i;
			}
			//if((l[i]+1==l[rowL[j]])&&(father[rowL[j]]!=i)){
			//	l[i]=l[rowL[j]];
			//	father[i]=father[rowL[j]];
			//	l[rowL[j]]++;
			//	father[rowL[j]]=i;
			//	i--;
			//	flag=1;
			//}
		}//建立节点依赖关系
		//if(flag) 
		//	continue;
		for(int j=rowU[i]+1;j<rowU[i+1];j++){//产生新的注入元，以0填入
			int nj=colU[j];//欲更新的元素纵坐标
			for(int k=colL[i]+1;k<colL[i+1];k++){
				int ni=rowL[k];//欲更新的元素横坐标
				if(ni>nj){//在L里找nj列非零元
					if(ni>rowL[colL[nj+1]-1]){
						int x=colL[nj+1];
						nnzL++;
						for(int y=nnzL-1;y>x;y--){
							rowL[y]=rowL[y-1];
							vL[y]=vL[y-1];
						}
						rowL[x]=ni;
						vL[x]=0;
						for(int y=nj+1;y<N+1;y++)
							colL[y]++;
					}//在该列最后插入新注入元
					else
						for(int x=colL[nj];x<colL[nj+1]-1;x++)
							if((ni>rowL[x])&&(ni<rowL[x+1])){
								nnzL++;
								for(int y=nnzL-1;y>x+1;y--){
									rowL[y]=rowL[y-1];
									vL[y]=vL[y-1];
								}
								rowL[x+1]=ni;
								vL[x+1]=0;
								for(int y=nj+1;y<N+1;y++)
									colL[y]++;
								break;
							}//在中间插入新注入元
				}
				if(ni<nj){//在U里找ni行非零元
					if(nj>colU[rowU[ni+1]-1]){
						int x=rowU[ni+1];
						nnzU++;
						for(int y=nnzU-1;y>x;y--){
							colU[y]=colU[y-1];
							vU[y]=vU[y-1];
						}
						colU[x]=nj;
						vU[x]=0;
						for(int y=ni+1;y<N+1;y++)
							rowU[y]++;
					}//在该行最后插入新注入元
					else
						for(int x=rowU[ni];x<rowU[ni+1]-1;x++)
							if((nj>colU[x])&&(nj<colU[x+1])){
								nnzU++;
								for(int y=nnzU-1;y>x+1;y--){
									colU[y]=colU[y-1];
									vU[y]=vU[y-1];
								}
								colU[x+1]=nj;
								vU[x+1]=0;
								for(int y=ni+1;y<N+1;y++)
									rowU[y]++;
								break;
							}//在中间插入新注入元
				}	
			}
		}
		//for(int j=0;j<nnzU;j++)
		//	printf("%f ",vU[j]);
		//printf("\n");
		//for(int j=0;j<nnzU;j++)
		//	printf("%d ",colU[j]);
		//printf("\n");
		//for(int j=0;j<N;j++)
		//	printf("%d ",rowU[j]);
		//printf("\n");
	}
	//for(int i=0;i<N;i++)
	//	printf("%d ",l[i]);
	//printf("\n");
	int leveldepth=0;
	for(int i=0;i<N;i++){
		levelNUM[l[i]]++;
		if (l[i]>leveldepth)
			leveldepth=l[i];
	}
	int *start=(int *)malloc(N*sizeof(int));
	start[0]=0;
	for(int i=1;i<=leveldepth;i++)
		start[i]=start[i-1]+levelNUM[i-1];
	int *num=(int *)malloc(N*sizeof(int));
	for(int i=0;i<N;i++)
		num[i]=0;
	for(int i=0;i<N;i++){
		int j=start[l[i]]+num[l[i]];
		level[j]=i;
		num[l[i]]++;
	}
	nnz[0]=nnzU;
	nnz[1]=nnzL;
	free(start);
	free(l);
	return leveldepth;
}

void myLU_solve(double *vU,int *colU,int *rowU,double *vL,int *rowL,int *colL,int N){
	
	for(int node=0;node<N;node++){
		int rows=rowU[node+1]-rowU[node]-1;
	    int cols=colL[node+1]-colL[node]-1;
		for(int i=0;i<cols;i++){
			int ori=colL[node]+1+i;
			vL[ori]=vL[ori]/vU[rowU[node]];
			int ni=rowL[ori];
			for(int j=0;j<rows;j++){
				int orj=rowU[node]+1+j;
				int nj=colU[orj];
				double val=-vU[orj]*vL[ori];
				if(ni>nj){
					for(int x=colL[nj]+1;x<colL[nj+1];x++)
						if (rowL[x]==ni){
							vL[x]+=val;
							break;
						}
				}
				else{
					for(int x=rowU[ni];x<rowU[ni+1];x++)
						if (colU[x]==nj){
							vU[x]+=val;
							break;
						}
				}
			}
		}
	}
}
int main()
{
	double t=0;
	int iteration=100;
	for(int ite=0;ite<iteration;ite++){
    FILE *in;
	if((in=fopen("C:\\home\\wmx\\myLU\\input\\J39.txt","rt+"))==NULL){
		printf("Cannot open file strike any key exit!");
		getchar();
		exit(1);
	}
	int N,nnzU,nnzL;
	fscanf(in,"%d",&N);
	fscanf(in,"%d",&nnzU);
	double *vU=(double *)malloc(5*nnzU*sizeof(double));
	int *colU=(int *)malloc(5*nnzU*sizeof(int));
	int *rowU=(int *)malloc((N+1)*sizeof(int));
	for(int i=0;i<nnzU;i++)
		fscanf(in,"%lf",&vU[i]);
	for(int i=0;i<nnzU;i++)
		fscanf(in,"%d",&colU[i]);
	for(int i=0;i<N+1;i++)
		fscanf(in,"%d",&rowU[i]);

	fscanf(in,"%d",&nnzL);
	double *vL=(double *)malloc(5*nnzL*sizeof(double));
	int *rowL=(int *)malloc(5*nnzL*sizeof(int));
	int *colL=(int *)malloc((N+1)*sizeof(int));
	for(int i=0;i<nnzL;i++)
		fscanf(in,"%lf",&vL[i]);
	for(int i=0;i<nnzL;i++)
		fscanf(in,"%d",&rowL[i]);
	for(int i=0;i<N+1;i++)
		fscanf(in,"%d",&colL[i]);
	fclose(in);
	int *level=(int *)malloc(N*sizeof(int));
	int *levelNUM=(int *)malloc(N*sizeof(int));
	for(int i=0;i<N;i++)
		levelNUM[i]=0;
	int *nnz=(int *)malloc(2*sizeof(int));
	int *father=(int *)malloc(N*N*sizeof(int));
	for(int i=0;i<N*N;i++)
		father[i]=-1;
	int *fatherNUM=(int *)malloc(N*sizeof(int));
	for(int i=0;i<N;i++)
		fatherNUM[i]=0;
	int leveldepth=myLU_analysis(vU,colU,rowU,nnzU,vL,rowL,colL,nnzL,N,father,fatherNUM,level,levelNUM,nnz);
	nnzU=nnz[0];
	nnzL=nnz[1];


	//int *threadsNeed=(int *)malloc(N*sizeof(int));
	//int *blocksNeed=(int *)malloc(N*sizeof(int));
	//int *cntblocks=(int *)malloc(N*sizeof(int));
	//cntblocks[0]=0;
	//int threadsperblock=256,totalblocks=0;
	//for(int i=0;i<N;i++){
	//	int rows=rowU[i+1]-rowU[i]-1;
	//	int cols=colL[i+1]-colL[i]-1;
	//	threadsNeed[i]=rows*cols;
	//	blocksNeed[i]=threadsNeed[i]/threadsperblock+1;
	//	totalblocks+=blocksNeed[i];
	//	if(i>0)
	//		cntblocks[i]=cntblocks[i-1]+blocksNeed[i-1];
	//}

	//int *block2node=(int *)malloc(totalblocks*sizeof(int));
	//for(int i=0;i<N;i++){
	//	for(int j=cntblocks[i];j<cntblocks[i]+blocksNeed[i];j++)
	//		block2node[j]=i;
	//}
	//for(int i=0;i<totalblocks;i++)
	//	printf("%d ",block2node[i]);
	//int *flag=(int *)malloc(N*sizeof(int));//全局标记位
	//for(int i=0;i<N;i++)
	//	flag[i]=0;
	
	LARGE_INTEGER t1,t2,tc;

	
	QueryPerformanceFrequency(&tc);
	QueryPerformanceCounter(&t1);
	myLU_solve(vU,colU,rowU,vL,rowL,colL,N);
	QueryPerformanceCounter(&t2);
	t=t+(t2.QuadPart - t1.QuadPart)*1000.0/tc.QuadPart;
	free(vU);
	free(rowU);
	free(colU);
	free(vL);
	free(rowL);
	free(colL);
	free(level);
	free(levelNUM);
	free(nnz);
	free(father);
	free(fatherNUM);
	}
	printf("time for solving: %f\n",t/iteration);
	//FILE *outfp;
	//outfp=fopen("C:\\home\\wmx\\myLU\\outputCPU\\J39LU.txt","wt+");
	//fprintf(outfp,"%d\n",N);
	//fprintf(outfp,"%d\n",nnzU);
	//for(int i=0;i<nnzU;i++)
	//	fprintf(outfp,"%f ",vU[i]);
	//fprintf(outfp,"\n");
	//for(int i=0;i<nnzU;i++)
	//	fprintf(outfp,"%d ",colU[i]);
	//fprintf(outfp,"\n");
	//for(int i=0;i<(N+1);i++)
	//	fprintf(outfp,"%d ",rowU[i]);
	//fprintf(outfp,"\n");
	//fprintf(outfp,"%d\n",nnzL);
	//for(int i=0;i<nnzL;i++)
	//	fprintf(outfp,"%f ",vL[i]);
	//fprintf(outfp,"\n");
	//for(int i=0;i<nnzL;i++)
	//	fprintf(outfp,"%d ",rowL[i]);
	//fprintf(outfp,"\n");
	//for(int i=0;i<(N+1);i++)
	//	fprintf(outfp,"%d ",colL[i]);
	//fprintf(outfp,"\n");
	//fclose(outfp);
    return 0;
}



