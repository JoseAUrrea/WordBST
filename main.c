/* COP 3502C Assignment 5
This program is written by Jose Urrea */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leak_detector_c.h"

typedef struct info
{
  char data[21];
  int count; 
  int depth;
} info;

typedef struct treeNode
{
  struct info *stats;
  struct treeNode *left, *right;
}treeNode;



FILE *inFile, *outFile;

treeNode* insertNode(treeNode *root, char *word);
void arrayTree(treeNode *root, info *array, int *reference);
treeNode* search(treeNode *root, char *word, int depth);
int totalNodes(treeNode *root);
void merge(info arr[], int left, int mid, int right);
void mergeSort(info *arr, int left, int right);
int compareTo(info *a, info *b);
void freeTree(treeNode *root);
void freeArray(info *arr);


int main(void) {
  atexit (report_mem_leak); 
  int wordCount;
  inFile = fopen("in.txt", "r");
  outFile = fopen("out.txt", "w");
  fscanf(inFile,"%d", &wordCount);
  
  int action;
  char word[21];
  int total = 0;
  int reference = 0;

  treeNode *root;
  root = NULL;
  if(wordCount > 0)
  {
    for(int i=0; i<wordCount; i++)
    {
      fscanf(inFile," %d %s", &action, word);
      
      if(action == 1)
      {
      root = insertNode(root, word);
      }
      else if(action == 2)
      {
        search(root, word, 0);
      }
      else
        printf("Invalid action number\n");
    }
    fclose(inFile);

    total = totalNodes(root);

    info *array = (info*)malloc(total*sizeof(info));

    arrayTree(root, array, &reference);

    mergeSort(array, 0, total-1);
    for(int i=0; i<total; i++)
    {
      printf("%s %d\n", array[i].data, array[i].count);
      fprintf(outFile, "%s %d\n", array[i].data, array[i].count);
    }

    fclose(outFile);
    freeTree(root);
    freeArray(array);
  }
  else 
    printf("No words to enter\n");
  return 0;
}

treeNode* insertNode(treeNode *root, char word[])
{
  if(root == NULL)
  {
    root = (treeNode*)malloc(sizeof(treeNode));
    root->stats = malloc(sizeof(info));
    root->stats->count = 1;
    strcpy(root->stats->data, word);
    root->left = NULL;
    root->right = NULL;
  }
  
  else if(strcmp(word, root->stats->data)<0)
  {
    root->left = insertNode(root->left, word);
  }
  else if(strcmp(word, root->stats->data)>0)
  {
    root->right = insertNode(root->right, word);
  }
  else
  {
    root->stats->count ++;
  }

  return root;
}

void arrayTree(treeNode *root, info *array, int *reference)
{
  if(root!=NULL)
  {
    arrayTree(root->left, array, reference);
    
    strcpy(array[(*reference)].data, root->stats->data);
    array[(*reference)].count = root->stats->count;
    (*reference)++;

    arrayTree(root->right, array, reference);
  }
}

treeNode* search(treeNode *root, char *word, int depth)
{
  if(root!=NULL)
  {
    if(strcmp(root->stats->data, word) == 0)
    {
      root->stats->depth = depth;
      printf("%d", root->stats->count);
      fprintf(outFile, "%d", root->stats->count);
      printf(" %d\n",root->stats->depth);
      fprintf(outFile, " %d\n", root->stats->depth);
      return root;
    }
    if(strcmp(word, root->stats->data)<0)
    {
      return search(root->left, word, depth+1);
      root->stats->depth = depth;
    }
    else
    {
      return search(root->right, word, depth+1);
      root->stats->depth = depth;
    }
  }
  else 
    printf("-1 -1\n");
    fprintf(outFile, "-1 -1\n");
  
  return NULL;
}

int totalNodes(treeNode *root)
{
  int a=1; 
  if(root==NULL)
  {
    return 0;
  }
  else
  {
    a += totalNodes(root->left);
    a += totalNodes(root->right);
    return a;
  }
}

void merge(info arr[], int left, int mid, int right)
{
  int leftsize = mid - left +1;
  int rightsize = right-mid;

  info *L = malloc(leftsize *sizeof(info));

  info *R = malloc(rightsize *sizeof(info));

  for (int i=0; i<leftsize; i++)
  {
      L[i].count = arr[left+i].count;
      strcpy(L[i].data, arr[left+i].data);
  }

  for(int j=0; j<rightsize; j++)
  {
    R[j].count = arr[mid+1+j].count;
    strcpy(R[j].data, arr[mid+1+j].data);
  }

  int i =0;
  int j =0;
  int k = left;

  while(i<leftsize && j<rightsize)
  {
    if(compareTo(&R[j], &L[i]) == 1)
    {
      arr[k] = R[j];
      k++;
      j++;
    }
    else
    {
      arr[k] = L[i];
      k++;
      i++;
    }
  }
  while(i<leftsize)
  {
    arr[k] = L[i];
    k++;
    i++;
  }
  while(j<rightsize)
  {
    arr[k] = R[j];
    k++;
    j++;
  }

  free(L);
  free(R);
}

void mergeSort(info *arr, int left, int right)
{
  if(right>left)
  {
    int mid = (left+right)/2;

    mergeSort(arr, left, mid);

    mergeSort(arr, mid+1, right);

    merge (arr, left, mid, right);
  }
}

int compareTo(info *a, info *b)
{
  if(a->count > b->count)
    return 1;
  else if( a->count < b->count)
    return -1;
  else
    if(a->data < b->data)
      return 1;
    else
      return -1;
}

void freeTree(treeNode *root){
	while(1){		
		if(root == NULL){
			break;
		}
		else if(root->left != NULL){
			freeTree(root->left);
      root->left = NULL;
		}
		else if(root->right != NULL){
			freeTree(root->right);
      root->right = NULL;
		}
		else{ 
      free(root->stats);
      free(root);
			return;
		}
	}
}

void freeArray(info *arr)
{
  free(arr);
}