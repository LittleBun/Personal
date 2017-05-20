#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>
#include <stdio.h>

int encrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext){
  EVP_CIPHER_CTX *ctx;
  int len;
  int plaintext_len;
  if(!(ctx = EVP_CIPHER_CTX_new())) {
    fprintf(stderr, "error1\n");
    abort();
  }
  if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv)){
    fprintf(stderr, "error2\n");
    abort();
  }
  if(1 != EVP_EncryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len)){
    fprintf(stderr, "error3\n");
    abort();
  }
  plaintext_len = len;
  if(1 != EVP_EncryptFinal_ex(ctx, plaintext + len, &len)) {
    //fprintf(stderr, "error4\n");
    ERR_print_errors_fp(stderr);
    abort();
  }
  plaintext_len += len;
  EVP_CIPHER_CTX_free(ctx);
  return plaintext_len;
}

int main(void){
  char plaintext[] = "This is a top secret.";
  char ciphertext[] = "8d20e5056a8d24d0462ce74e4904c1b513e10d1df4a2ef2ad4540fae1ca0aaf9";
   // printf("%d\n", strlen(ciphertext));
  unsigned char iv[16] = {0};
  unsigned char key[32];
  unsigned char buffer[1024];
  unsigned char encryptedtext[128];

  FILE *fp = fopen("English_Words_List.txt", "r");
  if(!fp){
    fprintf(stderr, "Cant open file\n");
    exit(1);
  }

  ERR_load_crypto_strings();
  OpenSSL_add_all_algorithms();
  OPENSSL_config(NULL);

  while(fscanf(fp, "%s", key) != EOF) {
    int l= strlen(key);
    if(l != 16){
      while(l<16){
        key[l] = ' ';
        l++;
      }
      key[16] = '\0';
    }
    int len=encrypt(plaintext, strlen((char *)plaintext), key, iv, buffer);
    char * pbuf=encryptedtext;
    int i;
    for(i=0; i<len; i++){
      pbuf += sprintf(pbuf, "%02x", buffer[i]);     
    }
    *(pbuf + 1) = '\0';
    //printf("%s\t%s\n", key, encryptedtext);
    if(strcmp(encryptedtext, ciphertext) == 0){
      //printf("lenth: %d\n", len);
      printf("-----------------Key found: %s\n", key);
      break;
    }
  }

  EVP_cleanup();
  ERR_free_strings();

  fclose(fp);
  return 0;
}
