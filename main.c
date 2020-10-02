#include <stdlib.h>
#include <stdio.h>
#include <string.h>


//struttura del nodo dell'albero, va messa fuori dal main e prima delle dichiarazioni delle funzioni affinché le funzioni abbiano il tipo nodo_rb già dichiarato
typedef struct nodo{
  char id_ent[40]; //nome dell'entità
  char key[40]; //chiave dell'entità
  struct nodo* parent; //puntatore al padre
  struct nodo* left; //puntatore al sottoalbero sx
  struct nodo* right; //puntatore al sottoalbero dx
  char color; //colore: 0 se nero, 1 se rosso
} nodo_rb;


//struttura del nodo della lista di monitoraggio delle entità
 typedef struct nodolista{
  char id_ent[40];
  struct nodolista* next;
  char color;
} listanodi;


//struttura del nodo della lista delle relazioni. La lista mi serve per monitorare le relazioni.
typedef struct relazione{
  char id_rel[40];
  char id_radice[40];
  nodo_rb* radice;
  int contatore;
  char flag_stesso;
  int flag_volta;

  struct relazione* next;
} nodolistarelazioni;

//struttura del nodo della lista dei massimi per il comando report.
typedef struct massimo{
  char id_rel[40];
  char id_dest[40];
  int massimo;

  struct massimo* next;
} nodomassimi;


//dichiaro a NULL il puntatore dell'inizio della lista di monitoraggio delle entità.
listanodi* first = NULL;

//dichiaro a NULL il puntatore dell'inizio della lista di monitoraggio delle relazioni
nodolistarelazioni* first_rel = NULL;

//dichiaro a NULL il puntatore dell'inizio della lista di monitoraggio dei massimi
nodomassimi* first_max = NULL;




//dichiarazioni delle funzioni

//inizializzazione nodo NIL
void InitNil();
//creazione nodo
void NuovoNodo(char *id_ent, listanodi* nodolista);

//inserimento nodo
void InserisciNodo(nodolistarelazioni* albero, nodo_rb *z);
//fixup dell'inserimento per gli alberi RB
void InsertFixup(nodolistarelazioni* albero, nodo_rb *z);

//eliminazione nodo
void EliminaNodo(nodolistarelazioni* albero, nodo_rb *z);
//fixup per l'eliminazione degli alberi RB
void DeleteFixup(nodolistarelazioni* albero, nodo_rb *x);

//rotazione a sinistra
void LeftRotate(nodolistarelazioni* albero, nodo_rb *x);
//rotazione a destra
void RightRotate(nodolistarelazioni* albero, nodo_rb *x);

//ricerca successore
nodo_rb *Successor(nodo_rb *x);
//ricerca minimo
nodo_rb* Minimum(nodo_rb *x);
//ricerca massimo
nodo_rb* Maximum(nodo_rb *x);


//ricerca nodo per id_ent, mi dice se esiste o no
listanodi* EsistePerId(listanodi* first, char* id_ent);
//creazione di una lista per una ricerca successiva
void AggiungiAllaLista(listanodi** first, char* id_ent, listanodi* nodolista);
//eliminazione di un nodo dalla lista di monitoraggio
void TogliDallaLista(listanodi** first, char* id_ent);


//Creazione di una relazione
nodolistarelazioni* NuovaRelazione(char* id_rel, char* id_dest);
//Creazione di un nuovo massimo per la relazione da inserire in report
nodomassimi* NuovoMassimo(nodomassimi* massimo, char* id_rel);
//Aggiunta di una relazione alla lista di monitoraggio delle relazioni
void AggiungiRelAllaLista(nodolistarelazioni** first_rel, nodolistarelazioni* nodo, char* id_rel, char* id_destinatario);
//Aggiunta di un massimo alla lista di monitoraggio dei massimi
void AggiungiMaxAllaLista(nodomassimi** first_max, nodomassimi* first_max2, nodomassimi* massimo, char* id_rel);

//Inserisco un nodo nella relazione
void InserisciNodoNellaRel(nodolistarelazioni* nodo, char* id_ent, char* id_dest, char* id_rel);

//controllo se esiste la relazione
int EsisteSoloLaRel(nodolistarelazioni* first_rel, char* id_rel, char* id_radice);
//controllo se esiste già la relazione tra queste due entità
int EsisteGia(nodolistarelazioni* first_rel, char* id_ent, char* id_dest, char* id_rel);
//elimino la relazione tra due entità
void EliminaRelTraDueEnt(nodolistarelazioni** first_rel, nodolistarelazioni* first_rel2, listanodi* first,  nodomassimi* first_max, char* id_ent, char* id_dest, char* id_rel);
//elimino tutte le relazioni di un'entità
void EliminoTutteLeSueRel(listanodi* first, nodolistarelazioni** first_rel, nodomassimi* first_max, char* id_ent);
//stampa l'elenco dei massimi per il comando report
void StampaMassimi(nodomassimi* first_max, nodomassimi* m);

void StampaAlbero(nodo_rb* x);
//trovo la relazione in base al nome
nodolistarelazioni* TrovaRelazione(nodolistarelazioni* first_rel, char* id_rel, char* id_radice);
//cambia il destinatario principale di un massimo
void CambiaDestinatarioPrincipale(nodolistarelazioni* first_rel, char* id_rel, char* id_dest);

nodo_rb *pNIL;
listanodi *pNIL2;


int main(){


//inizializzo il nodo NIL
InitNil();


while(1){
  char str[100];                    //buffer dove inserisco tutto quanto

  char comando[7];                  //buffer del comando
  char id_ent[40], id_dest[40];     //id_ent è id_ent quando il comando è su una sola entità, diventa id_sorgente quando definisco una relazione tra due entitò
  char id_rel[40];                  //id della relazione



  fgets(str, 100, stdin);
  sscanf(str, "%s \"%25[^\"]\"  \"%25[^\"]\"  \"%25[^\"]\" ", comando, id_ent, id_dest, id_rel);

  //valutazione del comando inserito
  if (strcmp(comando, "addent") == 0){

    //condizione di addent: se aggiungo una entità già monitorata, non deve succedere nulla.
    if(EsistePerId(first, id_ent) != NULL)
       continue;


    //creo il nodo per la lista di monitoraggio delle liste
    listanodi* nodolista;
    nodolista = malloc(sizeof(listanodi));


    NuovoNodo(id_ent, nodolista);
    AggiungiAllaLista(&first, id_ent, nodolista);

    continue;  //non break, devo poter dare un altro comando, break è solo dopo end

  }

  if (strcmp(comando, "delent") == 0){

    if(EsistePerId(first, id_ent) == NULL)
       continue;

    //elimina tutte le relazioni di cui "id_ent" fa parte (sia come origine, che come destinazione)
    //prima di toglierlo dalla lista altrimenti obv errore
    EliminoTutteLeSueRel(first, &first_rel, first_max, id_ent);

    TogliDallaLista(&first, id_ent);

    continue;

  }

  if (strcmp(comando, "addrel") == 0){

     //Se la relazione tra "id_orig" e "id_dest" già esiste, o se almeno una delle entità non è monitorata, non fa nulla.
     if(EsistePerId(first, id_ent) == NULL || EsistePerId(first, id_dest) == NULL || EsisteGia(first_rel, id_ent, id_dest, id_rel) == 1)
        continue;


     //dichiaro la variabile di tipo struct relazione per controllare se la relazione esiste già oppure no, per capire se devo crearla oppure no
     nodolistarelazioni* trovarelazione;
     trovarelazione = malloc(sizeof(nodolistarelazioni));


     //faccio il check sulle relazioni, vedo se ne esiste già una con id_rel e id_dest dati in input
     for(trovarelazione=first_rel;  trovarelazione!=NULL  &&  (strcmp(trovarelazione->id_rel, id_rel) != 0  ||  strcmp(trovarelazione->id_radice, id_dest) != 0)  ; trovarelazione=trovarelazione->next);
     if(trovarelazione != NULL){

       //Caso in cui la relazione già esiste, non devo crearla nuovamente perciò mi basta solo aggiungere un nodo all'albero con
       //l'entità destinazione come radice

        InserisciNodoNellaRel(trovarelazione, id_ent, id_dest, id_rel);
        continue;
      }

      //se sono arrivato qui vuol dire che la relazione non l'ho trovata e perciò devo crearla
      nodolistarelazioni* relazione;
      relazione = malloc(sizeof(nodolistarelazioni));

      relazione = NuovaRelazione(id_rel, id_dest);

      AggiungiRelAllaLista(&first_rel, relazione, id_rel, id_dest);

      InserisciNodoNellaRel(relazione, id_ent, id_dest, id_rel);
      continue;
    }

  if (strcmp(comando, "delrel") == 0){

    //Se non c'è relazione "id_rel" tra "id_orig" e "id_dest" (con "id_dest" come ricevente), non fa nulla
    if(EsisteGia(first_rel, id_ent, id_dest, id_rel) == 0)
      continue;

    EliminaRelTraDueEnt(&first_rel, first_rel, first, first_max, id_ent, id_dest, id_rel);
    continue;
  }

  if (strcmp(comando, "report") == 0){

    //se non ci sono relazioni tra le entità, l'output è none(senza virgolette)
    if(first_max==NULL){
      fputs("none\n", stdout);
      continue;
    }

   //flag mi serve per vedere se ci sta almeno una relazione che stampa almeno un'entità
   //se flag rimane 0 stampo none
   char flag = '0';

   nodomassimi* m;
   for(m=first_max; m!=NULL; m=m->next){
     if(m->massimo > 0)
       flag = '1';
     StampaMassimi(first_max, m);
   }

   fputs("\n", stdout);

   if(flag == '0'){
     fputs("none\n", stdout);
     continue;
   }





   continue;
  }

  if (strcmp(comando, "end") == 0){
  break;
  }

} //fine ciclo while

}//fine main

//funzioni degli alberi RB

//creazione nodo
void NuovoNodo(char *id_ent, listanodi* nodolista){

  nodolista->color = '0';
  strcpy(nodolista->id_ent, id_ent);

}

//inizializzazione nodo NIL
void InitNil(){

  pNIL = malloc(sizeof(nodo_rb));
  pNIL2 = malloc(sizeof(listanodi));
  pNIL->color = '0';
  pNIL->parent = pNIL;
  pNIL->left = pNIL;
  pNIL->right = pNIL;

  strcpy(pNIL2->id_ent, "pNIL");

  AggiungiAllaLista(&first, pNIL->id_ent, pNIL2);
  //printf("Il nodo si chiama %s e ha chiave %d.\n", pNIL->id_ent, pNIL->key);
}


//inserimento nodo
void InserisciNodo(nodolistarelazioni* albero, nodo_rb *z){

  nodo_rb *x;
  nodo_rb *y;

  y = pNIL;
  x = albero->radice;

  while(x != pNIL){             //blocco di codice in cui scendo in fondo per inserire il nodo
    y = x;
    if (strcmp(z->id_ent, x->id_ent) < 0)
      x = x->left;               //obv se devo inserirlo ed è più piccolo vado a sx
    else
      x = x->right;              //se devo inserirlo ed è più grande vado a dx
    }                            //tutto questo finché x è diverso da NIL, per arrivare in fondo

    z->parent = y;

    if (y == pNIL)
      albero->radice = z;              //l'albero è vuoto
    else if (strcmp(z->id_ent, y->id_ent) < 0)
      y->left = z;
    else //if (z->id_ent > y->id_ent)
      y->right = z;

    z->left = pNIL;
    z->right = pNIL;
    z->color = '1'; //è rosso

    InsertFixup(albero, z);
}

//fixup dell'inserimento per gli alberi RB
void InsertFixup(nodolistarelazioni* albero, nodo_rb *z){

  nodo_rb* x;
  nodo_rb* y;


  if (z == albero->radice)
    albero->radice->color = '0'; //nero

  else{
    x = z->parent;                     //x è il padre di z

    if (x->color == '1'){                  //se x è rosso

      if(x == x->parent->left){           //BLOCCONE IF (OPZIONE 1)
         y = x->parent->right;           //se x è figlio sx y è figlio dx, fratello di x

         if (y->color == '1'){
            x->color = '0';
            y->color = '0';
            x->parent->color = '1';
            InsertFixup(albero, x->parent);
          }
        else{
          if (z == x->right){
           z = x;
           LeftRotate(albero, z);
           x = z->parent;
        }
        x->color = '0';
        x->parent->color = '1';
        RightRotate(albero, x->parent);
      }
    }

    else{ //cioé if(x.parent,right)   //BLOCCONE ELSE (OPZIONE 2) //else relativo all'if di riga 396
      y = x->parent->left;
      if(y->color == '1'){
         x->color = '0';
         y->color = '0';
         x->parent->color = '1';
         InsertFixup(albero, x->parent);
      }
      else{
       if (z == x->left){
         z = x;
         RightRotate(albero, z);
         x = z->parent;
       }
       x->color = '0';
       x->parent->color = '1';
       LeftRotate(albero, x->parent);
      }
  }
 }
}
}


//eliminazione nodo
void EliminaNodo(nodolistarelazioni* albero, nodo_rb *z){

  nodo_rb* x;
  nodo_rb* y;

  if (z->left == pNIL || z->right == pNIL)      //vedo se ha due sottoalberi, se li ha prendo il successore
    y = z;                                  //altrimenti y (cioé il nodo da eliminare) è lo stesso z
  else
    y = Successor(z);

  if (y->left != pNIL)
    x = y->left;
  else
    x = y->right;

  if (x!= pNIL)
    x->parent = y->parent;           //qui inizia la sostituzione di y col suo sottoalbero

  if (y->parent == pNIL)
    albero->radice = x;            //se il padre di y è nullo, la radice prende il valore di x;
  else if (y == y->parent->left)
    y->parent->left = x;        //se y è il figlio sx di suo padre, il figlio sx di suo padre prende il valore di x
  else
    y->parent->right = x;       //se y è il figlio dx di suo padre, il figlio dx di suo padre prende il valore di x

  if (y != z)
    strcpy(z->id_ent, y->id_ent);                 //z->id_ent = y->id_ent;          //nel caso con due sottoalberi, la chiave di z è sostituita con quella di y

  if (y->color == '0')
    DeleteFixup(albero, x);
}

//fixup per l'eliminazione degli alberi RB
void DeleteFixup(nodolistarelazioni* albero, nodo_rb *x){

  nodo_rb* w;

  if (x->color == '1' || x->parent == pNIL)
    x->color = '0';

  else if (x == x->parent->left){     //BLOCCONE OPZIONE 1
    w = x->parent->right;
    if (w->color == '1'){
      w->color = '0';
      x->parent->color = '1';
      LeftRotate(albero, x->parent);
      w = x->parent->right;
    }
    if (w->left->color == '0' && w->right->color == '0'){
      w->color = '1';
      DeleteFixup(albero, x->parent);
    }
    else{
      if (w->right->color == '0'){
        w->left->color = '0';
        w->color = '1';
        RightRotate(albero, w);
        w = x->parent->right;
      }
      w->color = x->parent->color;
      x->parent->color = '0';
      w->right->color = '0';
      LeftRotate(albero, x->parent);
    }
  } //relativo all'else if di riga 483

  else{                       //BLOCCONE OPZIONE 2
    w = x->parent->left;
    if (w->color == '1'){
      w->color = '0';
      x->parent->color = '1';
      RightRotate(albero, x->parent);
      w = x->parent->left;
    }
    if (w->right->color == '0' && w->left->color == '0'){
      w->color = '1';
      DeleteFixup(albero, x->parent);
    }
    else{
      if (w->left->color == '0'){
        w->right->color = '0';
        w->color = '1';
        LeftRotate(albero, w);
        w = x->parent->left;
      }
    w->color = x->parent->color;
    x->parent->color = '0';
    w->left->color = '0';
    RightRotate(albero, x->parent);
  }
}
}


//rotazione a sinistra
void LeftRotate(nodolistarelazioni* albero, nodo_rb *x){

  nodo_rb* y;


  y = x->right;
  x->right = y->left;

  if (y->left != pNIL)
    y->left->parent = x;
  y->parent = x->parent;

  if (x->parent == pNIL)
    albero->radice = y;
  else if (x == x->parent->left)
    x->parent->left = y;
  else
    x->parent->right = y;

  y->left = x;
  x->parent = y;
}

//rotazione a destra
void RightRotate(nodolistarelazioni* albero, nodo_rb *x){

  nodo_rb* y;

  y = x->left;
  x->left = y->right;

  if (y->right != pNIL)
    y->right->parent = x;
  y->parent = x->parent;

  if (x->parent == pNIL)
    albero->radice = y;
  else if (x == x->parent->right)
    x->parent->right = y;
  else
    x->parent->left = y;

  y->right = x;
  x->parent = y;
}


//ricerca successore
nodo_rb *Successor(nodo_rb *x){

  nodo_rb* y;

  if (x->right != pNIL)
    return Minimum(x->right);
  y = x->parent;
  while(y != pNIL && x == y->right){
    x = y;
    y = y->parent;
  }
  return y;
}

//ricerca minimo
nodo_rb* Minimum(nodo_rb *x){

  while( x->left != pNIL)
    x = x->left;

  return x;
}

//ricerca massimo
nodo_rb* Maximum(nodo_rb *x){

  while(x->right != pNIL)
    x = x->right;

  return x;
}

//attraversamento dell'albero
int RicercaSeEsistePerNome(nodo_rb* x, char* nome){

  //x è la radice
  if(x != pNIL){
    if(strcmp(x->id_ent, nome) == 0)
       return 1;
    if(RicercaSeEsistePerNome(x->left, nome) == 1)
       return 1;
    if(RicercaSeEsistePerNome(x->right, nome) == 1)
       return 1;
  }
  return 0;
  }

//ricerca per nome e restituzione
nodo_rb* RestituisciPerNome(nodo_rb* x, char* nome){
  //x è la radice
    if(strcmp(nome, x->id_ent) == 0 || x == pNIL)
       return x;

    if(strcmp(nome, x->id_ent) < 0)
       return RestituisciPerNome(x->left, nome);

    else
       return RestituisciPerNome(x->right, nome);
  }

void StampaAlbero(nodo_rb* x){
  if(x != pNIL){
       StampaAlbero(x->left);
       printf("%s\n", x->id_ent);
       StampaAlbero(x->right);
  }
}

//funzioni per la lista di monitoraggio

//aggiunta di un nodo alla lista di monitoraggio
void AggiungiAllaLista(listanodi** first, char* id_ent, listanodi* nodolista){

    nodolista->next = *first;
    *first = nodolista;
}

//ricerca nodo per id_ent
listanodi* EsistePerId(listanodi* first, char* id_ent){

  listanodi* p;
  for(p=first; p!=NULL && strcmp(p->id_ent, id_ent) != 0; p=p->next);
  return p;
}

//eliminazione di un nodo dalla lista di monitoraggio
void TogliDallaLista(listanodi** first, char* id_ent){

  listanodi *cur, *prev;
  for(cur=*first, prev=NULL;   cur!=NULL && strcmp(cur->id_ent, id_ent) != 0;   prev=cur, cur=cur->next); //ciclo finché non esco. Quando esco sono nella condizione che volevo.

  if(cur == NULL)
     return;                      //non trova nulla, cur scorre e arriva fino a NULL
  if(prev == NULL)
     *first = (*first)->next;     //è da eliminare il primo nodo
  else
     prev->next = cur->next;
  free(cur);
  return;
}


//funzioni per la lista di monitoraggio degli alberi

//Creazione di una relazione
nodolistarelazioni* NuovaRelazione(char* id_rel, char* id_dest){

  nodolistarelazioni* relazione;
  relazione = malloc(sizeof(nodolistarelazioni));

  //assegno alla relazione il nome dato
  strcpy(relazione->id_rel, id_rel);

  //assegno alla radice il nome del destinatario dato
  strcpy(relazione->id_radice, id_dest);

  //inizializzo i due flag
  relazione->flag_stesso = '0';
  relazione->flag_volta = 0;

  //scorro la lista dei nodi per trovare il nodo con id_ent da inserire e ne creo una copia, ma del tipo che si può inserire nell'albero
  listanodi* p;
  for(p=first;   p!=NULL && strcmp(p->id_ent, id_dest)!= 0   ; p=p->next);

  nodo_rb* q;
  q = malloc(sizeof(nodo_rb));

  q->color = p->color;
  relazione->radice = q;

  strcpy(q->id_ent, p->id_ent);
  strcpy(relazione->radice->id_ent, p->id_ent);


  relazione->contatore = 0;
  //lo inizializzo a 0 perché la relazione è appena stata creata, non c'è nessun seguace


  nodomassimi* massimo;

  for(massimo=first_max; massimo!=NULL; massimo = massimo->next){
    if( strcmp(massimo->id_rel, id_rel) == 0)
      return relazione;
  }
  //questo for fa il controllo sui massimi. se ce ne sta già un altro con lo stesso nome non devo ricrearlo!
  //questo perché c'è un massimo per ogni relazione


  //il massimo non c'è già: lo devo creare.
  massimo = malloc(sizeof(nodomassimi));

  massimo = NuovoMassimo(massimo, id_rel);
  AggiungiMaxAllaLista(&first_max, first_max, massimo, id_rel);

  return relazione;
}

//Creazione di un nuovo massimo per la relazione da inserire in report
nodomassimi* NuovoMassimo(nodomassimi* nodo, char* id_rel){


  nodo->massimo = 0;
  strcpy(nodo->id_rel, id_rel);
  //nodo->id_dest = id_dest;     id_dest non ci va, varia in base alla id_dest che è destinataria di più relazioni

  return nodo;
}

//Aggiunta di una relazione alla lista di monitoraggio delle relazioni
void AggiungiRelAllaLista(nodolistarelazioni** first_rel, nodolistarelazioni* relazione, char* id_rel, char* id_destinatario){

  //devo inserire la relazione in ordine alfabetico
  nodolistarelazioni* cur, *prev;
  for(cur=*first_rel, prev=NULL;    cur!=NULL   &&   strcmp(id_destinatario, cur->id_radice) >= 0   ; prev=cur, cur=cur->next); //ricerca della posizione di inserimento

    if(prev==NULL){ //siamo in prima posizione, la lista è vuota
      relazione->next = *first_rel;
      *first_rel = relazione;
      return;
    }
    else{ //posizione centrale o coda, aggiorno i collegamenti
      prev->next = relazione;
      relazione->next = cur;
      return;
    }


}

//Aggiunta di un massimo alla lista di monitoraggio dei massimi
void AggiungiMaxAllaLista(nodomassimi** first_max, nodomassimi* first_max2, nodomassimi* massimo, char* id_rel){

  //devo inserire il massimo in ordine alfabetico
  nodomassimi* cur, *prev;

  //ricerca della posizione di inserimento
  for(cur=*first_max, prev=NULL;    cur!=NULL   &&   strcmp(id_rel, cur->id_rel) >= 0   ; prev=cur, cur=cur->next);

    if(prev==NULL){
      //siamo in prima posizione, la lista è vuota
      massimo->next = *first_max;
      *first_max = massimo;
      return;
    }
    else{
      //posizione centrale o coda, aggiorno i collegamenti
      prev->next = massimo;
      massimo->next = cur;
      return;
    }
}

//Inserisco un nodo nella relazione

void InserisciNodoNellaRel(nodolistarelazioni* albero, char* id_ent, char* id_dest, char* id_rel){

  if(strcmp(id_ent, id_dest) == 0)
    albero->flag_stesso = '1';

  albero->flag_volta++;

  if(albero->flag_volta < 2){
    albero->radice->parent = pNIL;
    albero->radice->left = pNIL;
    albero->radice->right = pNIL;
  }

  //devo inserire come nodo quello che ha come nome id_ent, così lo cerco nella lista di monitoraggio delle entità.
  listanodi* p;
  for(p=first; p!=NULL && strcmp(p->id_ent, id_ent) != 0 ; p=p->next);

  //ne creo una copia per non farmi sminchiare tutto
  nodo_rb* s;
  s = malloc(sizeof(nodo_rb));


  strcpy(s->id_ent, p->id_ent);
  s->color = p->color;

  InserisciNodo(albero, s);

  int flag_max = 0;

  nodolistarelazioni* q;
  nodomassimi* m;
  //voglio puntare alla relazione con id_rel e id_dest che dico io, il ciclo lo faccio solo una volta, tanto è solo una la relazione che cerco, poi è sistemata su q
  for(q=first_rel; q!=NULL && (strcmp(q->id_rel, id_rel) != 0 || strcmp(q->id_radice, id_dest) != 0);  q=q->next);
                                                                  //qui devo incrementare il contatore, perché, dato che ho aggiunto un nodo nella relazione,
                                                                  //so che c'è un nodo in più che "segue" il destinatario nell'albero
  q->contatore = q->contatore + 1;
  for(m=first_max; m!=NULL && strcmp(q->id_rel, m->id_rel) != 0; m=m->next);                               //scorro la lista dei massimi, quando trovo il nodo con struttura dei massimi
  if(q->contatore > m->massimo){                                                                           //aggiorno il massimo se ne sono in condizione
    m->massimo = q->contatore;        //è per tenere traccia del massimo per report
    flag_max++;
    if(strcmp(q->id_radice, m->id_dest) < 0 && flag_max > 1){
       strcpy(m->id_dest, q->id_radice);
       return;
    }
    if(flag_max > 1)
       return;
    strcpy(m->id_dest, q->id_radice);       //id_ent deve essere l'entità che la struttura massimo tiene, perché
                                            //la struttura massimo deve tenere in memoria qual è l'entità che è più destinataria
  }
  else if(strcmp(q->id_rel, m->id_rel) == 0 && q->contatore == m->massimo){
      if(strcmp(q->id_radice, m->id_dest) < 0)
           strcpy(m->id_dest, q->id_radice);
        }

   }

//controllo se esiste la relazione
int EsisteSoloLaRel(nodolistarelazioni* first_rel, char* id_rel, char* id_radice){

  nodolistarelazioni* p;
  for(p=first_rel; p!=NULL; p=p->next){
    if(strcmp(p->id_rel, id_rel) == 0   &&  strcmp(p->id_radice, id_radice) == 0)
      return 1;
  }
  return 0;
}

//controllo se esiste già la relazione tra queste due entità
int EsisteGia(nodolistarelazioni* first_rel, char* id_ent, char* id_dest, char* id_rel){

  nodolistarelazioni* p;
  for(p=first_rel; p!=NULL && (strcmp(p->id_radice, id_dest) != 0 || strcmp(p->id_rel, id_rel) != 0); p=p->next);

     //if(strcmp(p->id_radice, id_dest) == 0  &&  strcmp(p->id_rel, id_rel) == 0){
     if(p!=NULL){
       //prima controllo se si parla di stessa entità sia come origine che come desitinatario e nel caso controllo il flag
       if(strcmp(id_ent, id_dest) == 0){
         if(p->flag_stesso == '1')
           return 1;
         else if(p->flag_stesso == '0')
           return 0;
       }

       //nel caso in cui l'entità origine e destinatario siano diverse, vedo se se esiste nell'albero del destinatario un nodo con l'entità dell'origine
       if(RicercaSeEsistePerNome(p->radice, id_ent) == 1)
         return 1;
    }
    else
      return 0;
  }


//elimino la relazione tra due entità
void EliminaRelTraDueEnt(nodolistarelazioni** first_rel, nodolistarelazioni* first_rel2, listanodi* first, nodomassimi* first_max, char* id_ent, char* id_dest, char* id_rel){

  nodolistarelazioni *cur, *prev, *cur2;

  char flag = '0';
  char flag_massimo = '0';

  nodomassimi* m;

  //trovo il massimo con l'id_rel che cerco, e lo fisso su m. Ha l'id_ent e il massimo della relazione che mi servono nei confronti
  for(m=first_max;  m!=NULL  &&  strcmp(m->id_rel, id_rel) != 0  ; m=m->next);

  //scorro le relazioni e vado a vedere quante relazioni con lo stesso id_rel del massimo ci sono
  //(che però devono avere il contatore uguale al massimo, perché voglio vedere quante ne sono stampate con report),
  //in modo tale da agire in modo diverso se sono solo una o più
  for(cur=first_rel2;  cur!=NULL ; cur=cur->next){

    if(strcmp(cur->id_rel, m->id_rel) == 0){

      if(cur->contatore == m->massimo && flag == '1')
        flag = '2';
        //vuol dire che ci sono più massimi, perciò non devo decrementare il massimo perché rimane quello

      if(cur->contatore == m->massimo && flag == '0')
        flag = '1';

     }
    }

  //vado a cercare la relazione con id_dest come destinatario e id_rel come nome relazione e la fisso su cur:
  //è la relazione da eliminare
  for(cur=first_rel2;  cur!=NULL  &&  (strcmp(cur->id_rel, id_rel) != 0 || strcmp(cur->id_radice, id_dest) != 0)  ; cur=cur->next);

  if(strcmp(id_ent, id_dest) == 0 && cur->flag_stesso == '1')
     cur->flag_stesso = '0';
     //lo faccio perché altrimenti, se prova a eliminare la stessa entità della radice, elimina la radice

  if(cur->contatore == m->massimo && flag == '1'){
     m->massimo = m->massimo - 1;
     flag_massimo = '1';
  }
     //nel caso in cui fosse solo una la relazione con id_rel quello cercato, diminuisco il massimo, perché era solo lei il massimo
     //in caso contrario ovviamente non devo, perché il massimo rimane quello

  //il contatore della relazione va aggiornato a prescindere, ho sempre eliminato una relazione
  cur->contatore = cur->contatore - 1;


  //se il contatore è maggiore di 0 devo apportare il cambiamento se e solo l'entità che non viene più stampata è la prima
  if(cur->contatore > 0){

    if((flag == '2' || flag_massimo == '1') && strcmp(cur->id_radice, m->id_dest) == 0)
      CambiaDestinatarioPrincipale(first_rel2, cur->id_rel, cur->id_radice);

    nodo_rb *x;
    x = malloc(sizeof(nodo_rb));
    x = RestituisciPerNome(cur->radice, id_ent);
    //così ho trovato il nodo da eliminare

    //non devo eliminare la relazione se il contatore non è 0, ma devo togliere il nodo che voglio togliere dall'albero!
    EliminaNodo(cur, x);
    //così lo elimino
  }


  //se il contatore è 0 devo eliminare il nodo che sto togliendo dall'albero corrispondente alla relazione indicata
  //ed eliminare la relazione
  if(cur->contatore == 0){

    //ciclo finché non esco. Quando esco sono nella condizione che volevo, ovvero mi sono posizionato sulla relazione da eliminare
    for(cur2=*first_rel, prev=NULL;         cur2!=NULL   &&   (strcmp(cur2->id_rel, id_rel) != 0   ||   strcmp(cur2->id_radice, id_dest) != 0   ||   RicercaSeEsistePerNome(cur2->radice, id_ent) != 1)       ;      prev=cur2, cur2=cur2->next);

    if(cur2 == NULL)
      return;                              //non trova nulla, cur scorre e arriva fino a NULL

    if(prev == NULL){

      if(flag == '2' && strcmp(cur2->id_radice, m->id_dest) == 0)
        CambiaDestinatarioPrincipale(first_rel2, cur2->id_rel, cur2->id_radice);

      nodo_rb *x;
      x = malloc(sizeof(nodo_rb));
      x = RestituisciPerNome(cur2->radice, id_ent);
      //così ho trovato il nodo da eliminare

      EliminaNodo(cur2, x);
      //così lo elimino

      *first_rel = (*first_rel)->next;
      //è da eliminare il primo nodo
      //aggiorno la lista delle relazioni
    }

    else{

      if(flag == '2' && strcmp(cur2->id_radice, m->id_dest) == 0)
        CambiaDestinatarioPrincipale(first_rel2, cur2->id_rel, cur2->id_radice);

      nodo_rb *x;
      x = malloc(sizeof(nodo_rb));
      x = RestituisciPerNome(cur2->radice, id_ent);
      //così ho trovato il nodo da eliminare

      EliminaNodo(cur2, x);
      //così lo elimino

      prev->next = cur2->next;
      //aggiorno la lista delle relazioni
    }

    free(cur2);
    return;
  }


}

//elimino tutte le relazioni di un'entità
void EliminoTutteLeSueRel(listanodi* first, nodolistarelazioni** first_rel, nodomassimi* first_max, char* id_ent){

    //faccio puntare a q all'entità da eliminare
    listanodi *q, *p;
    for(q=first; q!=NULL && strcmp(q->id_ent, id_ent) != 0; q=q->next);

    //m e p girano su tutte le relazioni e il resto delle entità rispettivamente
    nodomassimi* m;
    for(m=first_max; m!=NULL; m=m->next){

       for(p=first; p!=NULL; p=p->next){

          //elimino tutte le relazioni di cui id_ent è origine
          if(EsisteGia(*first_rel, q->id_ent, p->id_ent, m->id_rel) == 1)
            EliminaRelTraDueEnt(first_rel, *first_rel, first, first_max, q->id_ent, p->id_ent, m->id_rel);

          //elimino tutte le relazioni di cui id_ent è destinatario
          if(EsisteGia(*first_rel, p->id_ent, q->id_ent, m->id_rel) == 1)
            EliminaRelTraDueEnt(first_rel, *first_rel, first, first_max, p->id_ent, q->id_ent, m->id_rel);
    }
  }
}

//stampa l'elenco dei massimi per il comando report
void StampaMassimi(nodomassimi* first_max, nodomassimi* m){

  nodolistarelazioni* p;

    if(m->massimo > 0){
      fputs("\"", stdout);
      printf("%s", m->id_rel);
      fputs("\" ", stdout);
      fputs("\"", stdout);
      printf("%s", m->id_dest);
      fputs("\" ", stdout);

      for(p=first_rel; p!=NULL ; p=p->next){

        if(strcmp(p->id_radice, m->id_dest) != 0 && strcmp(p->id_rel, m->id_rel) == 0 && p->contatore == m->massimo){
          fputs("\"", stdout);
          printf("%s", p->id_radice); //stampo solo se è diverso, vuol dire che non l'ho già stampato
          fputs("\" ", stdout);
        }
      }
      printf("%d", m->massimo);
      fputs("; ", stdout);
    }
  }


//trovo la relazione in base al nome
nodolistarelazioni* TrovaRelazione(nodolistarelazioni* first_rel, char* id_rel, char* id_radice){


  nodolistarelazioni* p;

  for(p=first_rel;  p!=NULL  &&  (strcmp(p->id_rel, id_rel) != 0  ||  strcmp(p->id_radice, id_radice) != 0)  ; p=p->next);

  return p;
}

//cambia il destinatario principale di un massimo
void CambiaDestinatarioPrincipale(nodolistarelazioni* first_rel, char* id_rel, char* id_dest){

  nodomassimi* m;
  nodolistarelazioni* p;

  //se trovo un massimo con lo stesso id relazione di id_rel e stesso destinatario di id_dest
  for(m=first_max; m!=NULL && (strcmp(m->id_rel, id_rel) != 0 || strcmp(m->id_dest, id_dest) != 0); m=m->next);

  if(m!=NULL){
    //devo far diventare l'id_dest quello subito dopo
    for(p=first_rel; p!=NULL && (strcmp(p->id_rel, m->id_rel) != 0 || p->contatore != m->massimo); p=p->next);
    if(p!=NULL)
      strcpy(m->id_dest, p->id_radice);
  }
}
