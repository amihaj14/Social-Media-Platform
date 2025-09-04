//Alexander Mihajlovski 20422017
//Dec 1 2024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

// Utility function to test parser
void testingParser(int arg1, char *arg2) {
    printf("The parser was called with arguments: %d and %s\n", arg1, arg2);
}
int userIDCounter = 0;
User* userRoot = NULL;
int messageIDCounter = 0;
Chat* chatList[100];
int chatCount = 0;
int postIDCounter = 0;

//Balancing Functions
int userBSTHeight(User* node);
int max(int a, int b);
int friendBSTHeight(friendNode* node);
User* rightRotateUser(User* y);
User* leftRotateUser(User* x);
int balanceUser(User* node);
friendNode* rightRotateFriend(friendNode* y);
friendNode* leftRotateFriend(friendNode* x);
int balanceFriend(friendNode* node);

//User Creation and insertion in BST
User* create_user(const char* name, const char* email); // int user_id is auto-generated to be unique
User* insertUser(User* node, User* newUser);

//Adding/Deleting friends
void add_friend(User* user1, User* user2); // users user1 and user2 are now friends
friendNode* createFriendNode(User* friend);
int areFriends(friendNode* node, User* user);
friendNode* insertFriendNode(friendNode* node, User* friend);
void delete_friend(User* user1, User* user2); // users user1 and user2 are no longer friends
friendNode* deleteFriendNode(friendNode* node, User* friend);

//Deleting Users
void delete_user(User* user); // user is deleted
User* deleteUserNode(User* node, int user_id);

//Printing Users names in alphabetical order
void collectUserBST(User* root, User** userArray, int* index);
int userNameComparison(const void* a, const void* b);
void print_users(); // prints all user names in ascending order

//Changing user profile
void change_user_name(User* user, char* new_name);
void change_user_email(User* user, char* new_email);

//Printing users friends
int friendNameComparison(const void* a, const void* b);
int collectFriendBST(friendNode* root, friendNode** friendArray, int* index);
void print_friends(User* user); // prints user's friends in ascending order

//Name/Email search
User* nameSearch(User* root, const char* name);
User* search_user_by_name(const char* name);
User* emailSearch(User* root, const char* email);
User* search_user_by_email(const char* email);

//Mutual Friends
User** mutual_friends(User* user1, User* user2); // returns an array of pointers to the mutual friends
void print_mutual_friends(User** friends); // prints mutual friends' user names in acsending order

//Messaging
Message* create_message(User* sender, User* receiver, const char* content); // int message_id is auto-generated to be unique
void print_message(Message* message);
void display_chat(User* user1, User* user2); // print messages in FIFO
Chat* getChat(User* user1, User* user2);

//Posting
Post* new_post(User* user, const char* content); // post id is auto-generated to be unique
void add_like(Post* post, User* user); // user is the individual who liked the post
void display_feed(User* user1);
void collectPosts(friendNode* node, Post** feedPosts, int* feedCount, int maxPosts);
int compareFeeds(const void* a, const void* b);

// WRITE FUNCTIONS BELOW

User* create_user(const char* name, const char* email){
    User* newUser = (User*)malloc(sizeof(User)); //Creating a new user variable and allocating memory for it.
    //checking to see if the memory allocation was done correctly.
    if(newUser == NULL){
        printf("Memory allocation failed.\n");
        return NULL;
    }

    newUser -> userId = userIDCounter++; //Increments the user id everytime a newUser is created.
    strncpy(newUser->name, name, sizeof(newUser->name)-1);//Copies the name the user entered into the newUsers name field.
    newUser ->name[sizeof(newUser->name)-1]= '\0';//Adds the null character to the end of the name string.
    strncpy(newUser->email, email, sizeof(newUser->email)-1);//Copies the email the user entered into the newUsers email field.
    newUser->email[sizeof(newUser->email)-1]='\0';//Adds the null character to the end of the email string.

    newUser -> left = NULL, newUser->right = NULL;//Sets the left and right child pointers of the user to null.
    newUser -> height = 1;//Sets the height of the users node to 1.
    newUser -> friends = NULL;//Initializing the users friends to be empty.
    newUser -> postCount = 0;//Initializing the users post count as 0 to start.

    userRoot = insertUser(userRoot, newUser);//Puts the user into the user AVL tree.

    return newUser;//Returns the pointer to the new user.
}

int max(int a, int b){
    return (a>b) ? a:b;//Returns a if a greater than b and returns b if b greater than a.
}

int userBSTHeight(User* node){
    if(node == NULL) return 0;//If the node is empty its height is 0.
    return node ->height;//Returns the height of the node.
}

int friendBSTHeight(friendNode* node){
    if(node == NULL) return 0;//If the node is empty its height is 0.
    return node ->height;//Returns the height of the node.
}

User* rightRotateUser(User* y){
    User* x = y->left;//Setting x to be the left child of y.
    User* T2 = x->right;//Setting T2 to be the right subtree of x.
    x->right = y;//Makes y the right child of x.
    y->left = T2;//Attaches T2 as the left child of y.
    //Updating the height of the nodes that got rotated.
    y->height = max(userBSTHeight(y->left), userBSTHeight(y->right))+1;
    x->height = max(userBSTHeight(x->left), userBSTHeight(x->right))+1;
    return x;//Returns the new root of the subtree.
}

User* leftRotateUser(User* x){
    User* y = x->right;//Setting y to be the right child of x.
    User* T2 = y->left;//Setting T2 as the left subtree of y.
    y->left = x;//Makes x the left child of y.
    x->right = T2;//Attaches T2 as the right child of x.
    //Updating the height of the nodes that got rotated.
    x->height = max(userBSTHeight(x->left),userBSTHeight(x->right))+1;
    y->height = max(userBSTHeight(y->left), userBSTHeight(y->right))+1;
    return y;//Returns the new root of the subtree.
}

int balanceUser(User* node){
    if(node == NULL) return 0;//If the node is empty, the balance factor is 0.
    return(userBSTHeight(node ->left) - userBSTHeight(node->right));//Returns the difference in height of the left and right subtrees.
}

friendNode* rightRotateFriend(friendNode* y){
    friendNode* x = y->left;//Setting x to be the left child of y.
    friendNode* T2 = x->right;//Setting T2 to be the right subtree of x.
    x->right = y;//Makes y the right child of x.
    y->left = T2;//Attaches T2 as the left child of y.
    //Updating the height of the nodes that got rotated.
    y->height = max(friendBSTHeight(y->left), friendBSTHeight(y->right))+1;
    x->height = max(friendBSTHeight(x->left), friendBSTHeight(x->right))+1;
    return x;//Returns the new root of the subtree.
}

friendNode* leftRotateFriend(friendNode* x){
    friendNode* y = x->right;//Setting y to be the right child of x.
    friendNode* T2 = y->left;//Setting T2 as the left subtree of y.
    y->left = x;//Makes x the left child of y.
    x->right = T2;//Attaches T2 as the right child of x.
    //Updating the height of the nodes that got rotated.
    x->height = max(friendBSTHeight(x->left),friendBSTHeight(x->right))+1;
    y->height = max(friendBSTHeight(y->left), friendBSTHeight(y->right))+1;
    return y;//Returns the new root of the subtree
}

int balanceFriend(friendNode* node){
    if(node == NULL) return 0;//If the node is empty, the balance factor is 0.
    return(friendBSTHeight(node ->left) - friendBSTHeight(node->right));//Returns the difference in height of the left and right subtrees.
}

//User BST
User* insertUser(User* node, User* newUser) {
    if (node == NULL) return newUser;//Base case of the recursive function, if the current node is empty returns the new user as a node.

    if (newUser->userId < node->userId)//If the new users id is smaller than the current node then go to the left subtree.
        node->left = insertUser(node->left, newUser);//Recursive call in the left subtree.
    else if (newUser->userId > node->userId)//If the new user id is greater than the current node then go to the right subtree.
        node->right = insertUser(node->right, newUser);//Recursive call in the right subtree.
    else
        return node; //Returns the current node unchanged if a duplicate id comes up.
        //Updates the height of the current node.
    node->height = 1 + max(userBSTHeight(node->left), userBSTHeight(node->right));
    int balance = balanceUser(node);//Calculates the balance factor to check if its unbalanced.

    if (balance > 1 && newUser->userId < node->left->userId){//New user inserted in the left subtree of the left child.
        return rightRotateUser(node);//Performs a right rotation.
    }if (balance < -1 && newUser->userId > node->right->userId){//New user inserted in the right subtree of the right child.
        return leftRotateUser(node);//Performs a left rotation.
    }
    if (balance > 1 && newUser->userId > node->left->userId) {//New user inserted in the right subtree of the left child.
        node->left = leftRotateUser(node->left);//Performs a left rotation on the left child.
        return rightRotateUser(node);//Performs a right rotation on the current node.
    }
    if (balance < -1 && newUser->userId < node->right->userId) {//New user inserted in the left subtree of the right child
        node->right = rightRotateUser(node->right);//Performs a right rotation on the right child.
        return leftRotateUser(node);//Performs a left rotation on the current node.
    }
    return node;//Returns the updated node.
}


friendNode* createFriendNode(User* friend) {
    if (friend == NULL) {//If the friend field is empty.
        printf("Cannot create a friend node.\n");
        return NULL;
    }

    friendNode* newNode = (friendNode*)malloc(sizeof(friendNode));//Allocating memory for the new friend node.
    if (newNode == NULL) {//If the memory allocation failed this will run.
        printf("Memory allocation failed.\n");
        return NULL;
    }

    newNode->friend = friend;//Assigns the friend to the new node
    newNode->left = NULL;//Sets the left child of the node to empty.
    newNode->right = NULL;//Sets the right child of the node to empty.
    newNode->height = 1;//Initializes the height at 1.

    return newNode;//Returns the created friendNode.
}


int areFriends(friendNode* node, User* user) {
    if (node == NULL || user == NULL) return 0; //The users are not friends.
    if (node->friend == user) return 1; //The two are friends.
    if (strcmp(user->name, node->friend->name) < 0){//If the users name is less than the current friends name it will go through the left subtree.
        return areFriends(node->left, user);
    }
    return areFriends(node->right, user);//Searches the right subtree to find the friend.
}


friendNode* insertFriendNode(friendNode* node, User* friend) {
    if (friend == NULL) return node;//If the friend is empty it will return the current node.
    if (node == NULL) return createFriendNode(friend);//If the current node is empty it creates and returns a new one.

    if (strcmp(friend->name, node->friend->name) < 0){//If the friends name is less than the users name it will search the left subtree.
        node->left = insertFriendNode(node->left, friend);//Recursively goes through the left subtree.
    }else if (strcmp(friend->name, node->friend->name) > 0){//If the friends name is greather than the users name it will search the right subtree.
        node->right = insertFriendNode(node->right, friend);//Recursively goes through the right subtree.
    }else{
        return node;//If there is a duplicate the original node is returned.
    }

    node->height = 1 + max(friendBSTHeight(node->left), friendBSTHeight(node->right));//Update the height and balance the tree.

    int balance = balanceFriend(node);//Calculating the balance factor of the current node.

    if (balance > 1 && strcmp(friend->name, node->left->friend->name) < 0){
        return rightRotateFriend(node);//Performs a right rotation.
    }if (balance < -1 && strcmp(friend->name, node->right->friend->name) > 0){
        return leftRotateFriend(node);//Performs a left rotation.
    }if (balance > 1 && strcmp(friend->name, node->left->friend->name) > 0) {
        node->left = leftRotateFriend(node->left);//Perfoms a left rotation on the left subtree.
        return rightRotateFriend(node);//Performs a right rotation.
    }
    if (balance < -1 && strcmp(friend->name, node->right->friend->name) < 0) {
        node->right = rightRotateFriend(node->right);//Performs a right rotation on the right subtree.
        return leftRotateFriend(node);//Performs a left rotation.
    }
    return node;//Returns the root of the subtree.
}


void add_friend(User* user1, User* user2) {
    if (user1 == NULL || user2 == NULL) {//Checking to see if either user is empty.
        printf("One or both users are NULL.\n");
        return;
    }

    if (user1 == user2) {//Checks to see if both users are the same.
        printf("A user cannot be friends with themselves.\n");
        return;
    }

    if (areFriends(user1->friends, user2)) {//Checking to see if the users are already friends.
        printf("%s and %s are already friends.\n", user1->name, user2->name);
        return;
    }

    user1->friends = insertFriendNode(user1->friends, user2);//Adds the second user to the first users friends.
    user2->friends = insertFriendNode(user2->friends, user1);//Adds the first user to the second users friends.
}


friendNode* deleteFriendNode(friendNode* node, User* friend) {
    if (node == NULL) return NULL;//Base Case, if the tree/subtree is empty return NULL.

    //Traversing the tree recursively to find the friend to be deleted.
    if (strcmp(friend->name, node->friend->name) < 0){
        node->left = deleteFriendNode(node->left, friend);
    }else if (strcmp(friend->name, node->friend->name) > 0){
        node->right = deleteFriendNode(node->right, friend);
    }else {//The case where the node has one or no children, if one child exists it replaces the node with the child.
        if (node->left == NULL) {
            friendNode* temp = node->right;
            free(node);
            return temp;
        } else if (node->right == NULL) {
            friendNode* temp = node->left;
            free(node);
            return temp;
        }

        //Replacing the node being deleted with the smallest node in the right subtree.
        friendNode* temp = node->right;
        while (temp->left != NULL) temp = temp->left;
        node->friend = temp->friend;
        node->right = deleteFriendNode(node->right, temp->friend);
    }

    //Updating the height and balances the tree after deletion.
    node->height = 1 + max(friendBSTHeight(node->left), friendBSTHeight(node->right));
    int balance = balanceFriend(node);

    if (balance > 1 && balanceFriend(node->left) >= 0)
        return rightRotateFriend(node);
    if (balance > 1 && balanceFriend(node->left) < 0) {
        node->left = leftRotateFriend(node->left);
        return rightRotateFriend(node);
    }
    if (balance < -1 && balanceFriend(node->right) <= 0)
        return leftRotateFriend(node);
    if (balance < -1 && balanceFriend(node->right) > 0) {
        node->right = rightRotateFriend(node->right);
        return leftRotateFriend(node);
    }

    return node;//Returns the new root node.
}


void delete_friend(User* user1, User* user2) {
    if (user1 == NULL || user2 == NULL) {//Checking if the users are empty.
        printf("One or both users are NULL.\n");
        return;
    }

    if (!areFriends(user1->friends, user2)) {//Checking to see if the users are friends to begin with.
        printf("%s and %s are not friends.\n", user1->name, user2->name);
        return;
    }
    //Deleting the users from their friend trees.
    user1->friends = deleteFriendNode(user1->friends, user2);
    user2->friends = deleteFriendNode(user2->friends, user1);
}


User* deleteUserNode(User* node, int user_id){
    if(node == NULL) return node;//Base case, if the node is empty it will return the current node.
    //Traversing the tree recursively based on user Id to find the user to be deleted.
    if(user_id < node->userId){
        node -> left = deleteUserNode(node ->left, user_id);
    }else if(user_id > node->userId){
        node -> right = deleteUserNode(node->right, user_id);
    }else{//Handling cases where the node has one or no children, if one child exists it replaces the node with the child.
        if(node ->left == NULL){
            User* temp = node->right;
            free(node);
            return temp;
        }else if(node->right == NULL){
            User* temp = node->left;
            free(node);
            return temp;
        }
        //Replacing the node being deleted with the smallest node in the right subtree.
        User* temp = node->right;
        while(temp && temp->left !=NULL){
            temp = temp -> left;
        }

        //Copying the data of the successor node to the current node.
        node->userId = temp->userId;
        strncpy(node->name, temp->name, sizeof(node->name));
        strncpy(node->email,temp->email,sizeof(node->email));

        node->right = deleteUserNode(node->right, temp->userId);//Deletes the successor from the right subtree.
    }

    //Updating the height and rebalancing the tree.
    node -> height = 1 + max(userBSTHeight(node ->left), userBSTHeight(node ->right));
    int balance = balanceUser(node);

    if (balance > 1 && balanceUser(node->left) >= 0) {
        return rightRotateUser(node);
    }
    if (balance > 1 && balanceUser(node->left) < 0) {
        node->left = leftRotateUser(node->left);
        return rightRotateUser(node);
    }
    if (balance < -1 && balanceUser(node->right) <= 0) {
        return leftRotateUser(node);
    }
    if (balance < -1 && balanceUser(node->right) > 0) {
        node->right = rightRotateUser(node->right);
        return leftRotateUser(node);
    }
    return node;//Returns the new root of the subtree.
}

void delete_user(User* user){
    if(userRoot == NULL){//Checking to see if the user tree is empty.
        printf("The user BST is empty");
        return;
    }

    userRoot = deleteUserNode(userRoot,user->userId);//Calling the delete helper function to delete the desired user.
}

void print_users(){
    if(userRoot == NULL){//Checking to see if the user tree is empty.
        printf("No users found in the BST.\n");
        return;
    }

    int userCount = 0;//The number of users in the user BST.
    User* tempArray[10000];//Temporary array to store the user names when properly ordering them.
    collectUserBST(userRoot, tempArray,&userCount);//Calls the function that will collect user names from the user tree.

    qsort(tempArray, userCount, sizeof(User*), userNameComparison);//Sorts the names in the tempArray of the users in ascending/alphabetical order.

    //Printing out each users name from the user tree.
    for(int i =0; i<userCount; i++){
        printf("%s", tempArray[i]->name);

        if(i<userCount -1){
            printf(",");
        }
    }
    printf("\n");
}

void collectUserBST(User* root, User** userArray, int* index){
    if(root != NULL){//Base Case, will run until the tree is fully collected.
        collectUserBST(root->left, userArray, index);//Goes through the left side of the tree collecting the user names in the array.
        userArray[*index]=root;
        (*index)++;
        collectUserBST(root->right,userArray, index);//Does the same with the right side of the tree.
    }
}

int userNameComparison(const void* a, const void* b){
    User* user1 = *(User**)a;
    User* user2 = *(User**)b;

    return strcmp(user1->name, user2->name);//Comparing the names alphabetically and returns, >0 if user1 comes after user2, <0 if user1 comes before user2, and 0 if the names are identical.
}

void change_user_name(User* user, char* new_name){
    if(user == NULL){//Checking if the user exists.
        printf("User not found. \n");
    }
    strncpy(user->name, new_name, sizeof(user->name)-1);//Changes the users name.
    user->name[sizeof(user->name)-1] = '\0';//Adds a null character to the end.
}

void change_user_email(User* user, char* new_email){
    if(user == NULL){//Checking if the user exists.
        printf("User not found. \n");
    }
    strncpy(user->email, new_email, sizeof(user->email)-1);//Changes the users email.
    user->email[sizeof(user->email)-1] = '\0';//Adds a null character to the end.
}

void print_friends(User* user) {
    if (user == NULL || user->friends == NULL) {//Checking to see if the user exists, or if he has any friends.
        printf("%s has no friends.\n", user->name);
        return;
    }

    friendNode* friendArray[10000];//Array that stores a users friends, max value of 10000 in case they are friends with everyone.
    int friendCount = 0;
    collectFriendBST(user->friends, friendArray, &friendCount);//Calling the friend collection function.

    qsort(friendArray, friendCount, sizeof(friendNode*), friendNameComparison);//Sorts the names in the friendArray in alphabetical order.

    //Printing out the friends names from the friendArray.
    for (int i = 0; i < friendCount; i++) {
        printf("%s", friendArray[i]->friend->name);
        if (i < friendCount - 1) printf(",");
    }
    printf("\n");
}

int friendNameComparison(const void* a, const void* b){
    friendNode* friend1 = *(friendNode**)a;
    friendNode* friend2 = *(friendNode**)b;
    return strcmp(friend1->friend->name, friend2->friend->name);//Comparing the names alphabetically and returns, >0 if friend1 comes after friend2, <0 if friend1 comes before friend2, and 0 if the names are identical.
}

int collectFriendBST(friendNode* root, friendNode** friendArray, int* index){

    if (root == NULL || friendArray == NULL || index == NULL) {//Base case, recursive function runs until the root is empty, the friend array is empty, or the index is.
        return 0;
    }

    collectFriendBST(root->left, friendArray, index);//Goes through the left side of the tree collecting the user names in the array.
    friendArray[*index]=root;
    (*index)++;
    collectFriendBST(root->right,friendArray, index);//Does the same with the right side of the tree.

    return *index;//Returns a pointer to the index.
}

User* search_user_by_name(const char* name){
    if(userRoot == NULL){//Checking if the user tree is empty.
        printf("The user BST is empty. \n");
        return NULL;
    }

    return(nameSearch(userRoot, name));//Calls name searching function to search for user.
}

User* nameSearch(User* root, const char* name){
    if(root == NULL){//Base case, recursively runs until it reaches the bottom of the user tree.
        return NULL;
    }

    if(strcmp(root->name,name) == 0){//Checking if the root has the same name as the one being searched for.
        return root;
    }
    //Searching the left side of the tree.
    User* leftResult = nameSearch(root->left,name);
    if(leftResult != NULL){
        return leftResult;
    }else{
        return nameSearch(root->right,name);//Searches right tree.
    }
}

User* search_user_by_email(const char* email){
    if(userRoot == NULL){//Checks if the user tree is empty.
        printf("The user BST is empty. \n");
        return NULL;
    }

    return(emailSearch(userRoot,email));//Calls the search function.
}

User* emailSearch(User* root, const char* email){
    if(root == NULL){//Base case, recursively calls until it has gone through the tree.
        return NULL;
    }

    if(strcmp(root->email,email) == 0){//Checks if the root node has the same email as the one being searched for.
        return root;
    }
    //Searches left side of tree.
    User* leftResult = emailSearch(root->left,email);
    if(leftResult != NULL){
        return leftResult;
    }else{
        return emailSearch(root->right,email);//Searches through the right side of the tree.
    }
}


User** mutual_friends(User* user1, User* user2) {
    if (user1 == NULL || user2 == NULL || user1->friends == NULL || user2->friends == NULL){//Checks if the users are valid and if they have friends.
        return NULL;
    }

    friendNode* user1FriendArray[10000];//The friends from the first users friend tree.
    friendNode* user2FriendArray[10000];//The friend from the second users friend tree.
    int user1FriendCount = 0, user2FriendCount = 0;//Initializing the friend counts of each user at 0.

    //Collecting the friends from each user and placing them in their respective arrays.
    collectFriendBST(user1->friends, user1FriendArray, &user1FriendCount);
    collectFriendBST(user2->friends, user2FriendArray, &user2FriendCount);

    //Allocating memory for the mutualFriends pointer array.
    User** mutualFriends = malloc(sizeof(User*) * (user1FriendCount + 1));
    if (mutualFriends == NULL) {
        printf("Memory allocation failed for mutual friends.\n");
        return NULL;
    }
    //Comparing the friends between the two users friend arrays.
    int mutualCount = 0;
    for (int i = 0; i < user1FriendCount; i++) {
        for (int j = 0; j < user2FriendCount; j++) {
            if (user1FriendArray[i]->friend == user2FriendArray[j]->friend) {
                //Check if this friend is already in the mutualFriends array.
                int alreadyAdded = 0;
                for (int k = 0; k < mutualCount; k++) {
                    if (mutualFriends[k] == user1FriendArray[i]->friend) {
                        alreadyAdded = 1;
                        break;
                    }
                }
                if (!alreadyAdded) {
                    mutualFriends[mutualCount++] = user1FriendArray[i]->friend;
                }
            }
        }
    }

    mutualFriends[mutualCount] = NULL;//Sets the end of the mutualFriends array with a NULL pointer.

    //Check if there are no mutual friends.
    if (mutualCount == 0) {
        free(mutualFriends);
        return NULL;
    }
    return mutualFriends;//Returns the mutualFriends array.
}


void print_mutual_friends(User** friends) {
    if (friends == NULL || friends[0] == NULL || friends[1] == NULL){//Checks if the users are empty/valid.
        printf("One or both users are NULL.\n");
        return;
    }

    //Setting each user to an index of the pointer array friends.
    User* user1 = friends[0];
    User* user2 = friends[1];

    User** mutualFriendsArray = mutual_friends(user1, user2);//Creates a mutual friends array and calls the mutual_friends function to populate it.

    if (mutualFriendsArray == NULL) {//Checks if the users have any mutual friends.
        printf("No mutual friends between %s and %s.\n", user1->name, user2->name);
        return;
    }
    //Printing out their mutual friends.
    for (int i = 0; mutualFriendsArray[i] != NULL; i++) {
        printf("%s", mutualFriendsArray[i]->name);
        if (mutualFriendsArray[i + 1] != NULL) {
            printf(",");
        }
    }
    printf("\n");

    free(mutualFriendsArray);//Freeing the memory in the array.
}


Message* create_message(User* sender, User* receiver, const char* content){
    if(!areFriends(receiver -> friends, sender)){//Checking if the two users are friends.
        printf("The two users are not friends.\n");
        return NULL;
    }
    //Allocating memory for a new message.
    Message* newMessage = (Message*)malloc(sizeof(Message));
    if(newMessage == NULL){
    printf("Memory allocation failed.\n");
    return NULL;
    }

    //Setting values to struct fields.
    newMessage -> messageId = messageIDCounter++;
    newMessage ->sender = sender;
    newMessage->receiver = receiver;

    //Copying the message into the content field.
    strncpy(newMessage->messageContent, content, sizeof(newMessage->messageContent) - 1);
    newMessage->messageContent[sizeof(newMessage->messageContent) - 1] = '\0';

    //Getting the chat between the two friends.
    Chat* chat = getChat(sender, receiver);
    if(chat == NULL){
        printf("Failed to create or retrieve chat.\n");
        free(newMessage);
        return NULL;
    }
    //Checking to see if the chat is over the message limit, if not it adds the new chat, if so it removes the oldest and adds the newest.
    if(chat->messageCount < 50){
        chat->messages[chat->messageCount++] = newMessage;
    }else{
        free(chat->messages[0]);
        for(int i = 1; i < 50; i++){
            chat->messages[i-1] = chat->messages[i];
        }
        chat->messages[49] = newMessage;
    }

    return newMessage;//Returns the new message.
}

Chat* getChat(User* user1, User* user2){
    //Checking to see if there is an existing chat between the two users and returns it.
    for (int i = 0; i < chatCount; i++) {
        if ((chatList[i]->user1 == user1 && chatList[i]->user2 == user2) || (chatList[i]->user1 == user2 && chatList[i]->user2 == user1)) {
            return chatList[i];
        }
    }

    //If there is no chat a new one is created.
    if (chatCount < 100) {
        Chat* newChat = (Chat*)malloc(sizeof(Chat));
        if(newChat == NULL){
            printf("Memory allocation failed.\n");
        }
        newChat->user1 = user1;
        newChat->user2 = user2;
        newChat->messageCount = 0;
        chatList[chatCount++] = newChat;
        return newChat;
    }

    return NULL;//If the chat has reached its limit return NULL.
}

void print_message(Message* message){
    if(message == NULL){//Checking if an empty message was given.
        printf("Message is empty\n");
        return;
    }
    printf("[%s:]%s", message->sender->name,message -> messageContent);//Prints out the message content.
}

void display_chat(User* user1, User* user2){
    if(user1==NULL||user2==NULL){//Checks if either user is empty.
        printf("Invalid users\n");
        return;
    }if(!areFriends(user1->friends,user2)){//Checks to see if the users are friends.
        printf("The users are not friends\n");
        return;
    }


    Chat* currentChat = getChat(user1, user2);//Gets the chat between the two users.
    if(currentChat == NULL || currentChat->messageCount == 0){//Checking if their chat is empty
        printf("No messages between %s and %s\n", user1->name, user2->name);
        return;
    }
    //Prints out the messages/chat between the two users.
    for(int i = 0; i < currentChat->messageCount; i++){
        print_message(currentChat->messages[i]);
        if(i < currentChat->messageCount -1){
            printf(",");
        }
    }
    printf("\n");
}

Post* new_post(User* user, const char* content){
    //Checking if the users is empty, the content is empty, and if the length of the content is over the limit of 256 characters.
    if(user == NULL){
        printf("Invalid user.\n");
        return NULL;
    }if(content == NULL){
        printf("Content is empty.\n");
        return NULL;
    }if(strlen(content) > 255){
        printf("Content exceeds maximum length for a post.\n");
        return NULL;
    }
    //Allocating memory for a new post.
    Post* newPost = (Post*)malloc(sizeof(Post));
    if(newPost == NULL){
        printf("Memory allocation failed.\n");
        return NULL;
    }
    //Setting values to each post field.
    newPost->postId = postIDCounter++;
    newPost->user = user;
    newPost->likes = 0;

    //Copying the posts content into its respective field.
    strncpy(newPost->postContent, content, sizeof(newPost->postContent)-1);
    newPost->postContent[sizeof(newPost->postContent) - 1] = '\0';
    //Adding the post to the user if they have less than the max amount of posts.
    if (user->postCount < 100) {
        user->posts[user->postCount++] = newPost;
    } else {
        printf("%s has reached the maximum number of posts.\n", user->name);
        free(newPost);//Freeing the posts memory.
        return NULL;
    }

    return newPost;//Returning the new post.
}


void add_like(Post* post, User* user){
    //Checks if the post and user are empty.
    if(post == NULL){
        printf("Invalid post.\n");
    }if(user == NULL){
        printf("Invalid user.\n");
    }

    //Checking if the user has already liked the post.
    for(int i = 0; i < post->likes; i++){
        if(post->likedUsers[i] == user){
            return;
        }
    }

    //Adding a like to the post and adding the user to the list of people who have liked that post.
    if(post->likes < 10000){
        post->likedUsers[post->likes++] = user;
        post->likes++;
    }
}

void display_feed(User* user) {
    if (user == NULL) {//Checking if the user is empty.
        printf("Invalid user.\n");
        return;
    }


    Post* feedPosts[20];//An array to hold the current user feed.
    int feedCount = 0;//A counter variable.

    //Adding user posts to the feed.
    for (int i = 0; i < user->postCount; i++) {
        if (feedCount < 20) {
            feedPosts[feedCount++] = user->posts[i];
        }
    }

    //Collecting all friends posts and adding them to the users feed.
    collectPosts(user->friends, feedPosts, &feedCount, 20);

    //Sorting the feed by likes and how recently it was posted.
    qsort(feedPosts, feedCount, sizeof(Post*), compareFeeds);

    int displayCount = (feedCount > 20) ? 20 : feedCount;//If the feed count is greater than 20 then the value of display count will be 20 for the 20 most recent posts, otherwise it will be set to whatever value feed count is at.
    //Printing out the users feed, 20 most recent posts.
    for (int i = displayCount-1; i >=0; i--) {
        printf("[%s:]%s", feedPosts[i]->user->name, feedPosts[i]->postContent);
        if(i > 0){
            printf(",");
        }
    }
    printf("\n");
}


int compareFeeds(const void* a, const void* b) {
    const Post* post1 = (const Post*)a;
    const Post* post2 = (const Post*)b;

    //Comparing the posts by likes.
    if (post1->likes != post2->likes) {
        return post2->likes - post1->likes;
    }

    //Comparing the posts by post id to see which one is most recent.
    return post1->postId - post2->postId;
}


void collectPosts(friendNode* node, Post** feedPosts, int* feedCount, int maxPosts) {
    if (node == NULL || *feedCount >= maxPosts){//Checking if the node is empty, or if the feed count is greater than the max posts.
        return;
    }

    //Traversing through the left subtree.
    collectPosts(node->left, feedPosts, feedCount, maxPosts);

    //Adding the friends posts.
    User* friend = node->friend;
    for (int i = 0; i < friend->postCount && *feedCount < maxPosts; i++){
        feedPosts[*feedCount] = friend->posts[i];
        (*feedCount)++;
    }

    //Traversing through the right subtree.
    collectPosts(node->right, feedPosts, feedCount, maxPosts);
}