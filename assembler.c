#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

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
    {"jr", "000000", 'R',"001000"},
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
void hextonum(char* ptr, char plag[]);
void bintonum (char* ptr, char plag[]);

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
    struct Sym *sym;

    //Read .data region
    address = 0x10000000;
    for (i=0; scanf("%s", temp) == 1;) {

       if (strcmp(temp, ".text") == 0) {
	   struct Sym* cur = Symbols;
	   while (cur->next!=NULL)
	   {
	    cur = cur->next;
	   }
	   cur->next = temp_sym;
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
	   scanf("%s", temp);	int joker;
	   if (temp[0] == '0' && temp[1] == 'x')
	   {
	      char *ptr1 = strtok(temp, "x");
	      ptr1 = strtok(NULL, "x");
	      char plag[0x1000];
	      hextonum(ptr1, plag);
	      joker = atoi(plag);
	   }
	   else
	   {
	     joker = atoi(temp);
	   }
	   temp_data->value = joker;
	   temp_data->next = NULL;

	   if (temp_sym->first==NULL)
	   {
	     temp_sym->first = temp_data;
	   }
	   else
	   {
	    temp_sym->last->next = temp_data;
	   }
	   temp_sym->last = temp_data;
	   temp_sym->size += 1;
           address = address + 4; 
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
		 if (strcmp(inst[i].name, "jr") == 0)
		 {
		    scanf("%s", temp);
		    strcpy(temp_text->s, temp);
		    strcpy(temp_text->t, "0");
		    strcpy(temp_text->d, "0");
		 } else {
		    scanf("%s", temp);
		    strcpy(temp_text->d, temp);
		    scanf("%s", temp);
		    strcpy(temp_text->s, temp); 
		    scanf("%s", temp);
		    strcpy(temp_text->t, temp);
		 }
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
		 if (strcmp(inst[i].name, "beq") == 0 || strcmp(inst[i].name, "bne") == 0)
		 {
		    strcpy(temp_text->s, temp);
		 }
		 else
		 {
		    strcpy(temp_text->t, temp);
		 }
		 if (strcmp(inst[i].name, "lui")!=0)
		 {
		    scanf("%s", temp);
		    if (strcmp(inst[i].name, "beq") == 0 || strcmp(inst[i].name, "bne") == 0)
		    {
			strcpy(temp_text->t, temp);
		    }
		    else if (strcmp(inst[i].name, "lw") == 0 || strcmp(inst[i].name, "sw") == 0)
		    {
			char *ptr = strtok(temp, "()");
			strcpy(temp_text->d, ptr);
			ptr = strtok(NULL, "()");
			strcpy(temp_text->s, ptr);
		    }
		    else
		    {	
			strcpy(temp_text->s, temp);
		    }
		 }
		 else
		 {
		    strcpy(temp_text->s, "0");
		 }
		 if (strcmp(inst[i].name, "lw") != 0 && strcmp(inst[i].name, "sw") != 0)
		 {
		    scanf("%s", temp);
		    strcpy(temp_text->d, temp);
		 }
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
	         temp_text->d = (char*)malloc(sizeof(char));
		 temp_text->idx = i;
		 temp_text->address = address;
		 scanf("%s", temp);
		 strcpy(temp_text->d, temp);
		 struct Text* cur = Texts;
		 while(cur->next!=NULL)
		 {
		    cur = cur->next;
		 }
		 cur->next = temp_text;
		}
		else if (inst[i].type == 'S')
		{
		    temp_text->t = (char*)malloc(sizeof(char));
		    temp_text->d = (char*)malloc(sizeof(char));
		    temp_text->s = (char*)malloc(sizeof(char));
		    temp_text->idx = i;
		    temp_text->address = address;
		    scanf("%s", temp);
		    strcpy(temp_text->d, temp);
		    scanf("%s", temp);
		    strcpy(temp_text->t, temp);
		    scanf("%s", temp);
		    strcpy(temp_text->s, temp);
		    struct Text* cur = Texts;
		    while (cur->next!=NULL)
		    {
			cur = cur->next;
		    }
		    cur->next = temp_text;
		}
	    }
	  }
	  if (strcmp("la", temp)==0)
	    {
		int ck = 1;
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
		address += 4;
		struct Text* cur = Texts;
		while (cur->next!=NULL)
                {
                    cur = cur->next;
                }
                cur->next = temp_text;
		for (sym = Symbols->next; sym != NULL; sym = sym->next)
		{
		    if (strcmp(sym->name, temp_text->d)==0)
		    {
			char* plag = NumToBits(sym->address, 32);
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
			    ck = -1;
			}
			break;
		    }		
		}

		if (ck == 1)
		{	    
	          temp_text = (struct Text*)malloc(sizeof(struct Text));
		  temp_text->s = (char*)malloc(sizeof(char));
		  temp_text->t = (char*)malloc(sizeof(char));
		  temp_text->d = (char*)malloc(sizeof(char));
		  temp_text->idx = 13;
	          strcpy(temp_text->t, abbos);
		  strcpy(temp_text->s, abbos);
		  strcpy(temp_text->d, austin);
	          temp_text->address = address;
		  address += 4;
		  cur = Texts;
		  while (cur->next!=NULL)
		  {
		      cur = cur->next;
                  }
	          cur->next = temp_text;
		}
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
       if (inst[text->idx].type == 'R' || inst[text->idx].type == 'S')
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
	if (text->t[0] == '$')
	{
	   char *ptr = strtok(text->t, "$,");
	   strcpy(text->t, ptr);
	}
        if (text->s[0] == '$')
        {
	   char *ptr = strtok(text->s, "$,");
	   strcpy(text->s, ptr);
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
			char* ptr = NumToBits(sym->address, 32);
			char* ptr1 = (char*)malloc(sizeof(char));
			char plag[0x1000];
			strncpy(ptr1, ptr, 16);
			bintonum(ptr1, plag);
			strcpy(text->d, plag);  			
		    }
		    else if (strcmp("ori", inst[text->idx].name)==0)
		    {
			char* ptr = NumToBits(sym->address, 32);
			char dom[17] = {0, };
			char plag[0x1000];
			for (int i = 16, j = 0; i < 32; i++, j++)
			{
			    dom[j] = ptr[i];
			}
			bintonum(dom, plag);
			strcpy(text->d, plag);    
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
		  char plag[0x1000];
		  hextonum(ptr1, plag);  	  
                  strcpy(text->d, plag);
	      }                                    
           }
	}	    
       }
      else if (inst[text->idx].type == 'J')
      {
	for (sym = Symbols->next; sym != NULL; sym = sym->next){
	    if (strcmp(sym->name, text->d)==0)
	    {
		int q = sym->address / 4;
		char limo[27];
		sprintf(limo, "%d", q);
		strcpy(text->d, limo);
	    }	
	}
      }	  
    } 
}

void hextonum (char* ptr, char plag[])
{
    int decimal = 0, position = 0;
    int len = strlen(ptr);
    for (int i = len - 1; i >= 0; i--)
    {
	char ch = ptr[i];
	if (ch >= 48 && ch <= 57)
	{
	    decimal += (ch - 48) * pow(16, position);
	}
	else if (ch >= 65 && ch <= 70)
	{
	    decimal += (ch - (65 - 10)) * pow(16, position);
	}
	else if (ch >= 97 && ch <= 102)
	{
	    decimal += (ch - (97 - 10)) * pow(16, position);
	}

	position++;
    }
    sprintf(plag, "%d", decimal);   
}

void bintonum (char* ptr, char plag[])
{
    int decimal = 0, position = 0;
    int len = strlen(ptr);
    for (int i = len - 1; i >= 0; i--)
    {
	char ch = ptr[i];
	decimal += (ch - '0') * pow(2, position);
	position++;
    }
    sprintf(plag, "%d", decimal);
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
	     printf("%s", NumToBits(atoi(text->d), 16));
        } else if(inst[text->idx].type == 'S') {
	    printf("00000");
	    printf("%s", NumToBits(atoi(text->t), 5));
	    printf("%s", NumToBits(atoi(text->d), 5));
	    printf("%s", NumToBits(atoi(text->s), 5));
	    printf("%s", inst[text->idx].funct);
        } else {
            printf("%s", NumToBits(atoi(text->d), 26));
        }
    }

    for (sym = Symbols->next; sym != NULL; sym = sym->next) {
        struct Data* cur = sym->first;
	if (cur != NULL)
	{
            printf("%s", NumToBits(cur->value, 32));
	    while (cur->next!=NULL)
	    {  
	       cur = cur->next;
	       printf("%s", NumToBits(cur->value, 32));
	    }
	}
    }
    printf("\n");
}

/*
 * min function
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
