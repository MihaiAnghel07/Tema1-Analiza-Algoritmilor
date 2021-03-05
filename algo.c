#include <stdio.h> 
#include <string.h> 
#include <time.h>
#include <stdlib.h>

  
// d -> numarul de caractere a alfabetului input
#define d 256
#define NR_FISIERE 28  //numarul testelor de input
  
/* pat -> pattern 
    txt -> text 
    q -> numar prim
*/
int RK(char pat[], char txt[], int q) 
{ 
    int M = strlen(pat); 
    int N = strlen(txt); 
    int i, j; 
    int p = 0; // valoare hash pentru pattern
    int t = 0; // valoare hash pentru txt 
    int h = 1; 
  
    
    for (i = 0; i < M - 1; i++) 
        h = (h * d) % q; 
  
    // Se calculeaza valoarea hash a pattern-ului si a 
    // primei "ferestre" a textului
    //fereastra = posibilul pattern gasit in text
    for (i = 0; i < M; i++) { 
        p = (d * p + pat[i]) % q; 
        t = (d * t + txt[i]) % q; 
    } 
  
    // se muta pattern-ul pozitie cu pozitie
    for (i = 0; i <= N - M; i++) { 
  
        // se compara valorile hash a pattern-ului si a "ferestrei" 
        // daca sunt egale se verifica si caractere pentru a nu aparea 
        // conflicte
        if (p == t) { 
            
            for (j = 0; j < M; j++) { 
                if (txt[i + j] != pat[j]) 
                    break; 
            } 
  
            //s-a gasit si se intoarce pozitia
            if (j == M) 
                return i;
        } 
  
        
        // se calculeaza valoarea hash pentru urmatoarea fereastra
        if (i < N - M) { 
            t = (d * (t - txt[i] * h) + txt[i + M]) % q; 
  
            // daca e negativa, o convertim la pozitiv 
            if (t < 0) 
                t = (t + q); 
        } 
    }

    return -1; 
} 
  

// compune lps[] pentru pat[0..M-1] 
void computeLPSArray(char* pat, int M, int* lps) 
{ 
   
    int len = 0; 
  
    lps[0] = 0;  
  
 
    int i = 1; 
    while (i < M) { 
        if (pat[i] == pat[len]) { 
            len++; 
            lps[i] = len; 
            i++; 
        } 
        else{

            if (len != 0) { 
                len = lps[len - 1]; 
            } 
            else 
            { 
                lps[i] = 0; 
                i++; 
            } 
        } 
    } 
} 
  

int KMPSearch(char* pat, char* txt) 
{ 
    int M = strlen(pat); 
    int N = strlen(txt); 
  
    int lps[M]; 
  
    // preproceseaza pattern-ul  
    computeLPSArray(pat, M, lps); 
  
    int i = 0; 
    int j = 0;  
    while (i < N) { 
        if (pat[j] == txt[i]) { 
            j++; 
            i++; 
        } 
  
        if (j == M) { 
            return i - j;
            j = lps[j - 1]; 
        } 
  
        else if (i < N && pat[j] != txt[i]) { 
                if (j != 0) 
                    j = lps[j - 1]; 
                else
                    i = i + 1; 
                } 
    }

    return -1; 
} 


int apply_RK(char *pat, char *txt, int q, FILE *out)
{

    int i = 0, k, l;
        int pas = strlen(pat);
        int len = strlen(pat);
        char found = 0;

    if(RK(pat, txt, q) >= 0){       //daca se gaseste tot sirul "pat" in    
        fputs(pat, out);         //txt, se afiseaza
        fputs("\n", out);
        return 1;
    }else{
            
        char *str = calloc(strlen(pat), sizeof(char));
        pas--;
        found = 0;
        while(pas > 0 && !found){    //se formeaza substringuri in ordine
            k = pas - 1;             //descrescatoare a lungimii
            l = 0;

            while(k < len){
                int cnt = 0;
                for (i = l; i <= k; i++, cnt++) {
                    str[cnt] = pat[i];
                }

                str[cnt] = '\0';
                if(RK(str, txt, q) >= 0){   //se cauta substringul in
                    fputs(str, out);        //"txt" folosind  RK
                    fputs("\n", out);
                    found = 1;          //variabila care ajuta la oprire
                }                       //prelucrarii dupa ce s-au gasit 
                                    //toate secventele de lungime maxima
                k++;
                l++;
            }
            
            pas--;
        }

        free(str);
    }

    if(found)
        return 1;

    return 0;

}


int apply_KMP(char *pat, char *txt, FILE *out)
{   

    char found = 0;
    int i = 0, k, l;
    int pas = strlen(pat);
    int len = strlen(pat);

    if(KMPSearch(pat, txt) >= 0){   //daca se gaseste tot sirul "pat" in 
        fputs(pat, out);            //txt, se afiseaza
        fputs("\n", out);
        return 1;
    }else{
            
        char *str = calloc(strlen(pat), sizeof(char));
        pas--;
        found = 0;
        while(pas > 0 && !found){   //se formeaza substringuri in ordine
            k = pas - 1;            //descrescatoare a lungimii
            l = 0;

            while(k < len){
                int cnt = 0;
                for (i = l; i <= k; i++, cnt++) {
                str[cnt] = pat[i];
                }

                str[cnt] = '\0';
                if(KMPSearch(str, txt) >= 0){ //se cauta substringul in
                    fputs(str, out);          //"txt" folosind  KMP
                    fputs("\n", out);
                    found = 1;          //variabila care ajuta la oprire
                }                       //prelucrarii dupa ce s-au gasit 
                                    //toate secventele de lungime maxima
                k++;
                l++;
            }
            
            pas--;
        }

        free(str);
    }

    if(found)
        return 1;

    return 0;
}


int main() 
{ 
    
    int q = 101; // Numar prim folosit in cazul Rabin - Karp
    clock_t t;
    FILE *in; 
    FILE *in2;
    FILE *out; 
    char path_in[15] = "in/test";
    char path_out[15] = "out/test";
    int cnt = 0, cnt2 = 0;
    
                
    while(cnt <= NR_FISIERE){
        
        char *txt = calloc(100000, sizeof(char)); 
        char *pat = calloc(100000, sizeof(char));
        char s[3];
        char s2[3];
        
        //se face extensia primului fisier de input 
        if(cnt < 10){       
            s[0] = cnt + '0';
            s[1] = '\0';
        } else {
            s[0] = ((cnt/10)%10) + '0';
            s[1] = (cnt%10) + '0'; 
            s[2] = '\0';
        }

        //se creeaza calea catre fisierul 1 de input
        strcat(path_in, s);    
        strcat(path_in, ".in");
        in = fopen(path_in, "r");
        cnt++;
        

        //se face extensia celui de-al doilea fisier de input 
        if(cnt < 10){       
            s[0] = cnt + '0';       
            s[1] = '\0';
        } else {
            s[0] = ((cnt/10)%10) + '0';
            s[1] = (cnt%10) + '0'; 
            s[2] = '\0';
        }


        //se creeaza calea catre fisierul 2 de input
        if(cnt < 10){       
            path_in[strlen(path_in) - 4] = s[0];
        } else {
            path_in[strlen(path_in) - 4] = s[1];
        }
        

        in2 = fopen(path_in, "r");
        cnt++;
        path_in[strlen(path_in) - strlen(s) - 3] = '\0';


        //se face extensia fisierului de output 
        if(cnt2 < 10){      
            s2[0] = cnt2 + '0';
            s2[1] = '\0';
        } else {
            s2[0] = ((cnt2/10)%10) + '0';
            s2[1] = (cnt2%10) + '0'; 
            s2[2] = '\0';
        }

        //se creeaza calea catre fisierul de output
        strcat(path_out, s2);       
        strcat(path_out, ".out");
        cnt2++;
        out = fopen(path_out, "w");


        //se calculeaza dimensiunea fisierelor de intrare, daca cel putin unul
        //este gol, nu se mai trece prin prelucrare 
        fseek(in, 0L, SEEK_END);
        int sizeof_file1 = ftell(in);
        fseek(in, 0L, SEEK_SET);

        fseek(in2, 0L, SEEK_END);
        int sizeof_file2 = ftell(in2);
        fseek(in2, 0L, SEEK_SET);
    

        if(sizeof_file1 == 0 || sizeof_file2 == 0){
            fputs("Nu se poate afla subsirul comun de lungime maxima! Cel putin unul dintre siruri este NULL! \n",out);
            goto tail;
        }


        //se face citirea datelor din fisiere
        if(sizeof_file1 != 0 && sizeof_file2 != 0){
            char aux[100000];
            while(fgets(aux, sizeof(aux), in)){
                strcat(pat, aux);
            }

            pat[strlen(pat) - 1] = '\0';
            //printf("%s\n",pat);
            while(fgets(aux, sizeof(aux), in2)){
                strcat(txt, aux);
            }

            txt[strlen(txt) - 1] = '\0';
            //printf("%s\n",txt);   
        }


        //se pune sirul de lungime mai mica in "pat"
        if(strlen(pat) > strlen(txt)){
            char aux[100000];
            strcpy(aux, pat);
            strcpy(pat, txt);
            strcpy(txt, aux);
        }


        char found = 0;
        

        fputs("----KMP Solution----\n", out);
        t = clock();
        found = apply_KMP(pat, txt, out);
        t = clock() - t;
        double time_taken = ((double)t)/CLOCKS_PER_SEC;
        if(!found){
            fputs("Nu s-a gasit nicio secventa comuna!\n", out);
        }
        fprintf (out, "----Timp: [%-10.10f]----\n", time_taken);



        fputs("\n", out);
        fputs("----RK Solution----\n", out);
        t = clock();
        found = apply_RK(pat, txt, q, out);
        t = clock() - t;
        time_taken = ((double)t)/CLOCKS_PER_SEC;
        if(!found){
            fputs("Nu s-a gasit nicio secventa comuna!\n", out);
        }
        fprintf (out, "----Timp: [%-10.10f]----\n", time_taken);


tail:
        path_out[strlen(path_out) - strlen(s2) - 4] = '\0';
        free(txt);
        free(pat);
        fclose(in);
        fclose(in2);
    }


    return 0; 
} 