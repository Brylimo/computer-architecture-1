#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Inst{
    char *name;
    char *op;
    char type;
    char *funct;
};

struct Inst inst[22] = {
    {"addiu", "001001", 'I',""},
    {"addu", "000000", 'R',"100001"},
    {"and", "000000", 'R',"100100"},
    {"andi", "001100", 'I', ""},
    {"beq","000100", 'I', ""},
    {"bne", "000101", 'I', ""},
    {"j", "000010", 'J', ""},
    {"jal", "000011", 'J', ""},
    {"jr", "000000", 'J',"001000"},
    {"lui", "001111", 'I', ""},
    {"lw", "100011", 'I', ""},
    {"nor", "000000", 'R' , "100111"},
    {"or", "000000", 'R', "100101"},
    {"ori", "001101", 'I', ""},
    {"sltiu", "001011",'I' ,""},
    {"sltu", "000000", 'R', "101011"},
    {"sll", "000000", 'S' ,"000000"},
    {"srl", "000000", 'S', "000010"},
    {"sw", "101011", 'I', ""},
    {"subu", "000000", 'R', "100011"}
};

struct Data{
    int value;
    struct Data *next;
};

struct Text{
    int idx;
    char *d;
    char *s;
    char *t;
    unsigned int address;
    struct Text *next;
};

struct Sym{
    char *name;
    int size;
    unsigned int address;
    struct Data *first;
    struct Sym *next;
    struct Data *last;
};

struct Sym *Symbols;
struct Text* Texts;
int datasize, textsize;

/*
 * You may need the following function
 */
char* NumToBits (unsigned int num, int len) {
    char* bits = (char *) malloc(len+1);
    int idx = len-1, i;

    while (num > 0 && idx >= 0) {
        if (num % 2 == 1) {
            bits[idx--] = '1';
        } else { 
            bits[idx--] = '0';
        }
        num /= 2;
    }

    for (i = idx; i >= 0; i--){ 
        bits[i] = '0';
    }

    return bits;
}

/*
 * read the assmebly code
 */
void read_asm () {
    int tmp, i;
    unsigned int address;
    char temp[0x1000] = {0};
    struct Sym *temp_sym = NULL;
    struct Data *temp_data = NULL;
    struct Text* temp_text = NULL;

    //Read .data region
    address = 0x10000000;
    for (i=0; scanf("%s", temp) == 1;) {

       if (strcmp(temp, ".text") == 0) {
	    break;
       } else if(temp[strlen(temp)-1] == ':') {
	   struct Sym* cur = Symbols;
	   while (cur->next!=NULL)
	   {
	    cur = cur->next;
	   }	       
	   cur->next = temp_sym;
	   temp_sym = (struct Sym*)malloc(sizeof(struct Sym));
	   temp_sym->name = (char*)malloc(sizeof(char));
	   strncpy(temp_sym->name, temp, strlen(temp)-1);
	   temp_sym->address = address;
       } else if(strcmp(temp, ".word") == 0) {
	   temp_data = (struct Data*)malloc(sizeof(struct Data));
	   int input;	 scanf("%d", &input);
	   temp_data->value = input;
	   struct Data* cur = temp_sym->first;
	   if (cur == NULL)
	   {
	    cur = temp_data;
	    temp_sym->size += 1;
	    address = address + 4;
	   }else{
	   while (cur->next!=NULL)
	   {
	      cur = cur->next;	     
	   } 
	   cur = temp_data;
	   temp_sym->size += 1;
           address = address + 4; 
	   }
       }
    }

    datasize = address - 0x10000000;
   
    //Read .text region
    address = 0x400000;
    for (i=0; scanf("%s", temp) == 1;) {

       if (temp[strlen(temp)-1] == ':') {	   
	    temp_sym = (struct Sym*)malloc(sizeof(struct Sym));
	    temp_sym->name = (char*)malloc(sizeof(char));
	    strncpy(temp_sym->name, temp, strlen(temp)-1);
	    if (strcmp(temp_sym->name, "main")==0)
		address += 4;
	    temp_sym->address = address;
	    struct Sym* cur = Symbols;
	    while(cur->next!=NULL)
	    {
		cur = cur->next;	    
	    }	
	    cur->next = temp_sym;
       } else {
	  for (int i = 0; i < 20; i++)
	  {
	    if (strcmp(inst[i].name, temp)==0)
	    {
		address += 4;
		temp_text = (struct Text*)malloc(sizeof(struct Text));
		if (inst[i].type == 'R')
		{
		 temp_text->idx = i;
		 temp_text->d = (char*)malloc(sizeof(char));
		 temp_text->s = (char*)malloc(sizeof(char));
		 temp_text->t = (char*)malloc(sizeof(char));
		 scanf("%s", temp);
		 strcpy(temp_text->d, temp);
		 scanf("%s", temp);
		 strcpy(temp_text->s, temp); 
	         scanf("%s", temp);
		 strcpy(temp_text->t, temp);
		 temp_text->address = address;
		 struct Text* cur = Texts;
		 while (cur->next!=NULL)
		 { 
		    cur = cur->next;
		 }
		  cur->next = temp_text;
		 }	
		else if (inst[i].type == 'I')
		{
		 temp_text->s = (char*)malloc(sizeof(char));
	         temp_text->t = (char*)malloc(sizeof(char));
		 temp_text->d = (char*)malloc(sizeof(char));	 
		 temp_text->idx = i;
		 scanf("%s", temp);
		 strcpy(temp_text->s, temp);
		 scanf("%s", temp);
		 strcpy(temp_text->t, temp);
		 scanf("%s", temp);
		 strcpy(temp_text->d, temp);
		 temp_text->address = address;
		 struct Text* cur = Texts;
		 while (cur->next!=NULL)
		 {
		    cur = cur->next;
		 }
		 cur->next = temp_text;
		}
		else if(inst[i].type == 'J')
		{
		 temp_text->idx = i;
		 temp_text->address = address;
		 struct Text* cur = Texts;
		 while(cur->next!=NULL)
		 {
		    cur = cur->next;
		 }
		 cur->next = temp_text;
		}
	    }
	  }
	  if (strcmp("la", temp)==0)
	    {
		address += 4;
		temp_text = (struct Text*)malloc(sizeof(struct Text));
		temp_text->t = (char*)malloc(sizeof(char));
		temp_text->s = (char*)malloc(sizeof(char));
		temp_text->d = (char*)malloc(sizeof(char));
		temp_text->idx = 9;
		char abbos[0x1000], austin[0x1000];
	        scanf("%s", abbos);
		strcpy(temp_text->t, abbos);
		temp_text->s = "0";
		scanf("%s", austin);
		strcpy(temp_text->d, austin);
	        temp_text->address = address;
		struct Text* cur = Texts;
		while (cur->next!=NULL)
                {
                    cur = cur->next;
                }
                cur->next = temp_text;
		    
		address += 4;
	        temp_text = (struct Text*)malloc(sizeof(struct Text));
		temp_text->s = (char*)malloc(sizeof(char));
		temp_text->t = (char*)malloc(sizeof(char));
		temp_text->d = (char*)malloc(sizeof(char));
		temp_text->idx = 13;
	        strcpy(temp_text->t, abbos);
		strcpy(temp_text->s, abbos);
		strcpy(temp_text->d, austin);
	        temp_text->address = address;
		cur = Texts;
		while (cur->next!=NULL)
		{
		    cur = cur->next;
                }
	        cur->next = temp_text;
	  }  			
       }
    }
    textsize = address - 0x400000;
}

/*
 * convert the assembly code to num
 */
void subst_asm_to_num () {
    struct Text *text;
    struct Sym *sym;

    for (text = Texts->next; text != NULL; text = text->next) {
       if (inst[text->idx].type == 'R')
       {
	  if (text->d[0] == '$')
	  {
	    char *ptr = strtok(text->d, "$,");
	    strcpy(text->d, ptr);
	  }
          if (text->s[0] == '$')
	  {
	    char *ptr = strtok(text->s, "$,");
	    strcpy(text->s, ptr);
	  }
	  if (text->t[0] == '$')
          {
	    char *ptr = strtok(text->t, "$,");
	    strcpy(text->t, ptr);
	  }	      
       }
       else if(inst[text->idx].type == 'I')
       {
	if (text->s[0] == '$')
	{
	   char *ptr = strtok(text->s, "$,");
	   strcpy(text->s, ptr);
	}
        if (text->t[0] == '$')
        {
	   char *ptr = strtok(text->t, "$,");
	   strcpy(text->t, ptr);
	}
	if (atoi(text->d) == 0)
	{
	  int check = -1;  
	   for (sym = Symbols->next; sym != NULL; sym = sym->next) { 
		if(strcmp(sym->name, text->d)==0)
		{
		    check = 1;
		    if (strcmp("lui", inst[text->idx].name)==0)
		    {			
			char* plag = NumToBits(sym->address, 32);
			strncpy(text->d, plag, 16);
			int u = 0;
			for (int k = 16; k < 32; k++)
			{
			    if (plag[k]=='0')
			    {
				u++;
			    }
			}
			if (u == 16)
			{
			    text->next = text->next->next;
			    break;
			}			    
		    }
		    else if (strcmp("ori", inst[text->idx].name)==0)
		    {
			char* plag = NumToBits(sym->address, 32);
			char dom[17] = {0, };
			for (int i = 16, j = 0; i < 32; i++, j++)
			{
			    dom[j] = plag[i];
			}
			    strcpy(text->d, dom);
		    }
		    else
		    {
			int liv = sym->address - text->address;
		        liv /= 4;
			char buf[17];
			sprintf(buf, "%d", liv);
		        strcpy(text->d, buf);
		    }
		    break;
		}
	   }
	   if (check == -1) 
           {
              if (text->d[0] == '0' && text->d[1] == 'x')
	      {
                  char *ptr1 = strtok(text->d, "x");
		  ptr1 = strtok(NULL, "x");  	  
                  strcpy(text->d, ptr1);
	      }                                    
           }
	}	    
       }	   
    } 
}

/*
 * print the results of assemble
 */
void print_bits () {
    struct Text* text;
    struct Sym* sym;
    struct Data* data;

    // print the header
    printf("%s", NumToBits(textsize, 32));
    printf("%s", NumToBits(datasize, 32));
    
    // print the body
    for (text = Texts->next ; text != NULL ; text = text->next) {
        printf("%s", inst[text->idx].op);

        if (inst[text->idx].type == 'R') {
	    printf("%s", NumToBits(atoi(text->s), 5));
	    printf("%s", NumToBits(atoi(text->t), 5));
	    printf("%s", NumToBits(atoi(text->d), 5));
	    printf("00000");
            printf("%s", inst[text->idx].funct);

        } else if(inst[text->idx].type == 'I') {
             printf("%s", NumToBits(atoi(text->s), 5));
	     printf("%s", NumToBits(atoi(text->t), 5));
	     if (strcmp(inst[text->idx].name, "lui") == 0 || strcmp(inst[text->idx].name, "ori")==0)
	     {
		printf("%s", text->d);
	     }
	     else		 
		printf("%s", NumToBits(atoi(text->d), 16));

        } else if(inst[text->idx].type == 'S') {
            /* blank */

        } else {
            /* blank */
        }
    }

    for (sym = Symbols->next; sym != NULL; sym = sym->next) {
        /* blank */
    }
    printf("\n");
}

/*
 * main function
 */ 
int main (int argc, char* argv[]) {

    if (argc != 2) {
        
        printf("Usage: ./assembler <assembly file>\n");
        exit(0);

    } else {
        
        // reading the input file
        char *filename = (char *)malloc(strlen(argv[1]) + 3);
        strncpy(filename, argv[1], strlen(argv[1]));
        if (freopen(filename, "r", stdin) == 0) {
            printf("File open Error\n");
            exit(1);
        }

        Symbols = (struct Sym *)malloc(sizeof(struct Sym));
        Texts = (struct Text *)malloc(sizeof(struct Text));

        // creating the output file (*.o)
        filename[strlen(filename)-1] = 'o';
	freopen(filename, "w", stdout);

        // Let's complete the below functions!
        read_asm();
        subst_asm_to_num();
        print_bits();

        // freeing the memory
        free(filename);
        free(Symbols);
        free(Texts);
    }
    return 0;
}
