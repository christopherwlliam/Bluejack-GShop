#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct game{
	char title[26];
	char genre[10];
	int stock;
	game* left;
	game* right;
	int height;
}*root;

game *createGame(char* title, char* genre, int stock){
	game* newGame = (game*)malloc(sizeof(game));
	strcpy(newGame->title, title);
	strcpy(newGame->genre, genre);
	newGame->stock = stock;
	newGame->left = NULL;
	newGame->right = NULL;
	newGame->height = 1;
	
	return newGame;
}

int getHeight(game* curr){
	if(curr == NULL){
		return 0;
	}
	return curr->height;
}

int max(int a, int b){
	return a > b? a : b;
}

int balanceFactor(game* curr){
	if(curr == NULL){
		return 0;
	}
	
	return getHeight(curr->left) - getHeight(curr->right);
}

game* leftRotate(game* unbalanceNode){
	game* pivot = unbalanceNode->right;
	game* pivotChild = pivot->left;
	
	pivot->left = unbalanceNode;
	unbalanceNode->right = pivotChild;
	
	unbalanceNode->height = 1 + max(getHeight(unbalanceNode->left), getHeight(unbalanceNode->right));
	pivot->height = 1 + max(getHeight(pivot->left), getHeight(pivot->right));
	
	return pivot;
}

game* rightRotate(game* unbalanceNode){
	game *pivot = unbalanceNode->left;
	game *pivotChild = pivot->right;
	
	pivot->right = unbalanceNode;
	unbalanceNode->left = pivotChild;
	
	unbalanceNode->height = 1 + max(getHeight(unbalanceNode->left), getHeight(unbalanceNode->right));
	pivot->height = 1 + max(getHeight(pivot->left), getHeight(pivot->right));
	
	return pivot;
}

game* balancing(game* curr){
	if(curr == NULL){
		return curr;
	}
	curr->height = 1 + max(getHeight(curr->left), getHeight(curr->right));
	int BF = balanceFactor(curr);
	
	// balancing

	if(BF > 1 && balanceFactor(curr->left) >= 0){
		return rightRotate(curr);
	}
	else if(BF > 1 && balanceFactor(curr->left) < 0){
		curr->left = leftRotate(curr->left);
		return rightRotate(curr);
	}
	else if(BF < -1 && balanceFactor(curr->right) < 0){
		return leftRotate(curr);
	}
	else if(BF < -1 && balanceFactor(curr->right) >= 0){
		curr->right = rightRotate(curr->right);
		return leftRotate(curr);
	}
	return curr;
}

game* insert(game* curr, game* newGame){
	if(curr == NULL){
		return newGame;
	}else if(strcmp(newGame->title, curr->title) < 0){
		curr->left =insert(curr->left, newGame);
	}else if(strcmp(newGame->title, curr->title) > 0){
		curr->right =insert(curr->right, newGame);
	}
}

void print(game* curr){
	if(root == NULL){
		printf("Warehouse is empty...\n");getchar();getchar();
		return;
	}
	if(curr == NULL){
		return;
	}
	print(curr->left);
	printf("%s\t%s\t\t%d\n", curr->title, curr->genre, curr->stock);
	print(curr->right);
}

game* deleteStock(game* curr, char* title){
	if(curr == NULL) return curr;
	else if(strcmp(title, curr->title)< 0) curr->left = deleteStock(curr->left,title);
	else if(strcmp(title, curr->title)> 0) curr->right = deleteStock(curr->right,title);
	else {
		if(!curr->left && !curr->right){
			free(curr);
			curr = NULL;
		}
		else if(curr->left || curr->right){
			game* temp;
			if(curr->left) temp = curr->left;
			else temp = curr->right;
			
			*curr = *temp;
			free(temp);
			temp = NULL;
		}
		else{
			game* temp = curr->left;
			while(temp->right) temp = temp->right;
			
			strcpy(curr->title, temp->title);
			strcpy(curr->genre, temp->genre);
			curr->stock = temp->stock;
			
			curr->left = deleteStock(curr->left,temp->title);
		}
	}
	return balancing(curr);
}

game* search(game* curr, char* title){
	if(curr == NULL){
		return NULL;
	}else if(strcmp(title, curr->title) < 0){
		return search(curr->left, title);
	}else if(strcmp(title, curr->title) > 0){
		return search(curr->right, title);
	}else{
		return curr;
	}
}

void insertGame(){
	game* searchUnik;
	char title[26];
	char genre[10];
	int stock;
	do{
		printf("Input Game title[5-25][unique] : ");
		getchar(); scanf("%[^\n]",title);	
		searchUnik = search(root,title);
	}while((strlen(title) >= 25 || strlen(title) <= 5) || searchUnik != NULL);
	
	do{
		printf("Input game Type[Action|RPG|Adventure|Card Game] : ");
		getchar(); scanf("%[^\n]", genre);
	}while(strcmp(genre,"Action") != 0 && strcmp(genre,"RPG") != 0 && strcmp(genre,"Adventure") != 0 && strcmp(genre,"Card Game") != 0);\
	
	do{
		printf("Input Game Stock [>= 1]: ");
		scanf("%d", &stock);
	}while(stock < 1);
	
	root = insert(root, createGame(title,genre,stock));
	
	printf("Insert Success"); getchar(); getchar();
}

void updateGame(){
	char title[26];
	char genre[10];
	int stock;
	print(root);
	if(root == NULL){
		return;
	}
	printf("Input game title:");getchar();
	scanf("%[^\n]", title);
	game* searchTitle = search(root, title);
	if(searchTitle == NULL){
		printf("Data Not Found\n\n");
		printf("Press enter to continue\n...."); getchar();getchar();
		return;
	}else{
		printf("Current Stock : %d\n", searchTitle->stock);
	}
	
	do{
		printf("Input update Type [add | remove ] : ");
		getchar(); scanf("%[^\n]", genre);
	}while(strcmp(genre,"add") != 0 && strcmp(genre,"remove") && strcmp(genre,"ADD") != 0 && strcmp(genre,"REMOVE"));
	
	if(strcmp(genre,"remove") == 0 || strcmp(genre,"REMOVE") == 0){
		do{
			printf("Input stock to remove [1-50] : ");
			scanf("%d", &stock);
		}while(stock < 1 || stock > 50);
	
		if(stock == searchTitle->stock){
			root = deleteStock(searchTitle, title);	
		}else{
			searchTitle->stock -= stock;	
		} 
		
	}else{
		do{
			printf("Input stock to add [1-50] : ");
			scanf("%d", &stock);
		}while(stock < 1 || stock > 50);
		
		searchTitle->stock += stock;
	}
	printf("Data Updated succesfully..."); getchar();getchar();
}

int main(){
	int choose;
	while(true){
		do{
		system("cls");
		printf("Bluejack GShop\n");
		printf("==============\n");
		printf("1. Insert Game\n");
		printf("2. View Game\n");
		printf("3. Update Stock\n");
		printf("4. Exit\n");
		printf(">> ");
		scanf("%d",&choose);
	}while(choose > 5 || choose < 1);
		switch(choose){
			case 1:
				insertGame();
				break;
			case 2:
				printf("|Game Title | Game Genre | Game Stock |\n");
				print(root);
				printf("Press enter to continue"); getchar(); getchar();
				break;
			case 3:
				updateGame();
				break;
			case 4:
				return 0;
				break;
		}
	}
	
}
