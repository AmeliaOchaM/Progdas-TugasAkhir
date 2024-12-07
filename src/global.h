// Di file header global, misalnya global.h  
#ifndef GLOBAL_H  
#define GLOBAL_H  

class User;  // Forward declaration  

// Deklarasi global yang dapat diakses di semua file  
extern User* currentUser;  
extern int nextUserId;  

#endif