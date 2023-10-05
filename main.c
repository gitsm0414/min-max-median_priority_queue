#include <stdio.h>
#include <stdlib.h>

#define MAX 30
int totalSize = 0; //current total data size (global)


typedef struct node_t_{
    int value;
    int size; //size of subtree whose root is this node

    struct node_t_* left;
    struct node_t_* right;

}node_t;


node_t* currentRoot = NULL; //current root pointer (global)


int getHeight(node_t* node){
    if(node==NULL) return 0;

    int leftDepth = getHeight(node->left);
    int rightDepth = getHeight(node->right);

    if(leftDepth > rightDepth) return leftDepth+1;
    else return rightDepth+1;
}

int getBalanceFactor(node_t* node){
    return getHeight(node->left) - getHeight(node->right);
}


node_t* rightRotation(node_t* z){
    node_t* y = z->left;

    z->left = y->right;
    y->right = z;

    z->size = 1 + (z->left ? z->left->size:0) + (z->right ? z->right->size:0);
    y->size = 1 + (y->left ? y->left->size:0) + (y->right ? y->right->size:0);

    return y;
}

node_t* leftRotation(node_t* z){
    node_t* y = z->right;

    z->right = y->left;
    y->left = z;

    z->size = 1 + (z->left ? z->left->size:0) + (z->right ? z->right->size:0);
    y->size = 1 + (y->left ? y->left->size:0) + (y->right ? y->right->size:0);

    return y;
}


node_t* LL(node_t* z){
    return rightRotation(z);
}


node_t* RR(node_t* z){
    return leftRotation(z);
}


node_t* LR(node_t* z){
    z->left = leftRotation(z->left);
    return rightRotation(z);
}


node_t* RL(node_t* z){
    z->right = rightRotation(z->right);
    return leftRotation(z);
}


node_t* treeBalancing(node_t* node){ // adjusting subtree's formation whose root is 'node'
    int balanceFactor = getBalanceFactor(node);

    if(balanceFactor >= 2){
        balanceFactor = getBalanceFactor(node->left);
        if(balanceFactor >= 1){
            node = LL(node);
        }
        else{
            node = LR(node);
        }
    }
    else if(balanceFactor <= -2){
        balanceFactor = getBalanceFactor(node->right);
        if(balanceFactor <= -1){
            node = RR(node);
        }
        else{
            node = RL(node);
        }
    }

    return node;
}


node_t* insertNode(node_t* node, int value){
    if(node==NULL){
        node = (node_t*)malloc(sizeof(node_t));
        node->left = NULL;
        node->right = NULL;
        node->size = 1;
        node->value = value;

        totalSize++;

        return node;
    }

    if(value < node->value){
        node->left = insertNode(node->left, value);
        node->size=node->size + 1;
        node = treeBalancing(node);

    }
    else if(value > node->value){
        node->right = insertNode(node->right, value);
        node->size=node->size + 1;
        node = treeBalancing(node);
    }

    return node;
}


node_t* rightMostNode(node_t* node){
    if(node->right==NULL) return node;

    return rightMostNode(node->right);
}

node_t* leftMostNode(node_t* node){
    if(node->left==NULL) return node;

    return leftMostNode(node->left);
}


node_t* deleteNode(node_t* node, int value){
    if(node==NULL) return node;

    if(value < node->value){
        node->left = deleteNode(node->left, value);
    }
    else if(value > node->value){
        node->right = deleteNode(node->right,value);
    }
    else{
        if(node->left==NULL && node->right==NULL){
            free(node);
            totalSize--;
            return NULL;
        }

        if(node->left==NULL){
            node_t* temp = node->right;
            free(node);
            totalSize--;
            return temp;
        }
        else if(node->right==NULL){
            node_t* temp = node->left;
            free(node);
            totalSize--;
            return temp;
        }

        //if exact delete node has two children
        node_t* temp = rightMostNode(node->left);

        //copy
        node->value = temp->value;

        //deleting original
        node->left = deleteNode(node->left, temp->value);
    }

    node->size = 1 + (node->right ? node->right->size : 0) + (node->left ? node->left->size : 0);

    return treeBalancing(node);
}


//return n-th smallest node in subtree( whose root is 'node')
node_t* Nsmallest(node_t* node, int leftOrder){
    if(node==NULL){
        return NULL;
    }
    if(totalSize==1){
        return node;
    }


    int leftSize = node->left ? node->left->size : 0;


    if(leftSize+1==leftOrder) return node;

    else if(leftSize+1 > leftOrder){
        return Nsmallest(node->left, leftOrder);
    }
    else{
        return Nsmallest(node->right, leftOrder - leftSize-1);
    }
}


node_t* find_min(node_t* root){

    return leftMostNode(root);
}


node_t* find_max(node_t* root){

    return rightMostNode(root);
}


node_t* find_median(node_t* root){
    int order=(totalSize+1)/2;

    return Nsmallest(root, order);
}


int* delete_min(node_t* root){
    node_t* minNode = leftMostNode(root);
    if(minNode==NULL) return NULL;

    int* minPointer = &(minNode->value);

    currentRoot = deleteNode(root, *minPointer);

    return minPointer;
}


int* delete_max(node_t* root){
    node_t* maxNode = rightMostNode(root);
    if(maxNode==NULL) return NULL;

    int* maxPointer = &(maxNode->value);

    currentRoot = deleteNode(root, *maxPointer);

    return maxPointer;
}


int* delete_median(node_t* root){
    node_t* medianNode = Nsmallest(root, (totalSize+1)/2);
    if(medianNode==NULL) return NULL;

    int* medianPointer = &(medianNode->value);

    currentRoot = deleteNode(root, *medianPointer);

    return medianPointer;
}


void freeTree(node_t* root){
    if(root==NULL) return;

    freeTree(root->left);
    freeTree(root->right);

    free(root);
}


int main()
{
    int operationNum;
    int insertValue;
    char inputString[MAX];
    node_t* temp;


    scanf("%d", &operationNum);
    getchar(); //reading '\n'

    for(int i=0; i<operationNum; i++){
        fgets(inputString, MAX, stdin);

        if(inputString[0]=='I'){
            sscanf(inputString + 2, "%d", &insertValue);
            currentRoot = insertNode(currentRoot, insertValue);

        }
        if(inputString[0]=='F'){
            if(inputString[2]=='M'){
                temp = find_min(currentRoot);
                if(temp==NULL) continue;
                printf("%d\n", temp->value);

            }else if(inputString[2]=='X'){
                temp = find_max(currentRoot);
                if(temp==NULL) continue;
                printf("%d\n", temp->value);

            }else if(inputString[2]=='E'){
                temp = find_median(currentRoot);
                if(temp==NULL) continue;
                printf("%d\n", temp->value);
            }

        }
        if(inputString[0]=='D'){
            if(inputString[2]=='M'){
                delete_min(currentRoot);

            }else if(inputString[2]=='X'){
                delete_max(currentRoot);

            }else if(inputString[2]=='E'){
                delete_median(currentRoot);

            }
        }

    }

    freeTree(currentRoot);

    return 0;
}
