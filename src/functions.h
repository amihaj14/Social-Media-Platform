// functions.h

//Alexander Mihajlovski 20422017
//Dec 1 2024
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

struct User;
struct friendNode;
typedef struct User User;
typedef struct Post Post;

typedef struct friendNode{
    struct User* friend;
    struct friendNode* left;
    struct friendNode* right;
    int height;
}friendNode;

typedef struct User{
    int userId;
    char email[50];
    char name[50];
    struct friendNode* friends;
    struct User *left;
    struct User *right;
    int height;

    int postCount;
    Post* posts[100];
} User;

typedef struct Post{
    int likes;
    int postId;
    char postContent[256];
    User* user;
    User** likedUsers[10000];

}Post;

typedef struct Message{
    int messageId;
    char messageContent[256];
    User* sender;
    User* receiver;
} Message;

typedef struct Chat{
    Message* messages[50];
    int messageCount;
    User* user1;
    User* user2;
}Chat;

void testingParser(int arg1, char *arg2);

//Balancing functions
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
#endif
