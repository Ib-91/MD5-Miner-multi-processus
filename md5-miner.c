// ldd: -lcrypto
#include <openssl/md5.h> //pour le hachage
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // Pour le processus 
#include <signal.h>	//Pour le kill


// = {0} => all the array is reset to zero (only works for zero!)
char hash[1 + 2 * MD5_DIGEST_LENGTH] = {0};

// Calcule le hash MD5 de la chaîne 'str'
char *md5hash(char *str) {
    unsigned char md5[MD5_DIGEST_LENGTH] = {0};
    MD5((unsigned char *)str, strlen(str), md5);
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        sprintf(hash + 2 * i, "%02x", md5[i]);
    }
    return hash;
}

int length(int num) {
    int len = 0;
    if (num == 0) {
        return 1;
    }
    while (num > 0) {
        num /= 10;
        len++;
    }
    return len;
}


int zeros (char* s, int n) {//chaine commence par n fois '0'
    for (int i = 0; i < n; i++) {
        if (s[i] != '0') {
           return 0;}
    }
    return 1;
}


void bruteforce (int first, int step, int zero){
  int pid;
  char* reptext;//représentation textuelle
  char* hach;
  char* fichier_nom;
  FILE* fichier;
  
  while(1){
    
    reptext= (char*)malloc((length(first)+1)* sizeof(char));
    sprintf(reptext, "%d",first);//Convertir l'entier en chaine
    printf("reptext= ' %s ' \n",reptext);
    hach=md5hash(reptext);
    printf("hachage de %s = '%s' \n",reptext, hach);
    
    if (zeros(hach, zero)==1){
		
      	pid=getpid();
      	printf("pid= %d\n",pid);
        fichier_nom=(char*)malloc(((5+(length(pid)))+1) * sizeof(char));
      	
      	sprintf(fichier_nom, "found.%d", pid);//converti pid en chaine  	
      	printf("%s \n",fichier_nom);
      	
      	fichier=fopen(fichier_nom,"w");//w pour ouvrir en écriture
        fprintf(fichier, "%s\n", hach);//Ecrire la valeur dans le fichier
      	fclose(fichier);

      	free(reptext);
      	free(fichier_nom);
      	exit(0);
    }else{
   first+=step;
    }
   }
}



int main(void) {
  	pid_t p;
    pid_t child[10];
  	int i=0;
  	while(i<10){
          p = fork();
        if (p == 0) {  //si on est dans un processus enfant
			bruteforce(i, 10, 6);
          	exit(0);
        }
      	child[i]=p;//enregistre le pid de l'enfant
        i++;
    }
    pid_t fini_pid = waitpid(-1, NULL, 0); //attendre qu'un enfant finisse
    printf("1er enfant qui termine PID = %d\n", fini_pid);

  	//tuer les autres enfant
    int j=0;
  	while(j<10) {
        if (child[j] != fini_pid) {
           	printf("On tue l'enfant avec le pid= %d\n", child[j]);
            kill(child[j], SIGKILL);
        }
      j++;
    }
  
  	char* fichier_nom[255];
  	sprintf(fichier_nom, "found.%d",fini_pid);
    FILE* fichier = fopen(fichier_nom, "r");//r pour read
    if (fichier){
    int val;
    fscanf(fichier, "%d", &val);  //lit la valeur de l'entier
    printf("L'enfant %d a trouvé le résultat '%d' \n", fini_pid, val);
    fclose(fichier);
    remove("found.PID");
    }
  return 0;
}