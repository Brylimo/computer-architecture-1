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
struct Text *Texts;
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
	    

       } else if(strcmp(temp, ".word") == 0) {
	
           address += 4; 
       }
    }

    datasize = address - 0x10000000;

    //Read .text region
    address = 0x400000;
    for (i=0; scanf("%s", temp) == 1;) {

       if (temp[strlen(temp)-1] == ':') {	
	   address += 4;   
       } else {
	  temp_text = (struct Text*)malloc(sizeof(struct Text));
	  for (int i = 0; i < 20; i++)
	  {
	    if (strcmp(inst[i].name, temp)==0)
	    {
		address += 4;
		if (inst[i].type == 'R')
		{
		 temp_text->idx = i;
		 scanf("%s", temp_text->d);
		 scanf("%s", temp_text->s); 
	         scanf("%s", temp_text->t);
		 temp_text->address = address;
		 struct Text* cur = Texts;
		 while (cur->next!=NULL)
		 {
		    cur = cur->next;
		 }
		 Texts->next = temp_text;
		}
		else if (inst[i].type == 'I')
		{
		 temp_text->idx = i;
		 scanf("%s", temp_text->s);
		 scanf("%s", temp_text->t);
		 temp_text->address = address;
		 struct Text* cur = Texts;
		 while (cur->next!=NULL)
		 {
		    cur = cur->next;
		 }
		 Texts->next = temp_text;
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
		 Texts->next = temp_text;
		}
	    }
	  }  
	  /* char mssg[33];
	   int son = 0;
	   for (int i = 0; i < 20; i++)
	   {
		if (strcmp(inst[i].name, temp) == 0)
		{
		    if (inst[i].type == 'R')
		    {
			for (son; son < 6; son++)
			{
			   mssg[son] =  inst[i].op[son];
			}
			for (son; son < 11; son++)
			{
			    char k[4];  scanf("%s", &k);

			}
					
		    }
		    else if (inst[i].type == 'I')
		    {
		    
		    }
		    else if (inst[i].type == 'J')
		    {
		    
		    }
		}
	   }*/
       }
    }

    textsize = address - 0x400000;
    printf("%s\n", NumToBits(textsize, 32));
    printf("%s\n", NumToBits(datasize, 32));
}

/*
 * convert the assembly code to num
 */
void subst_asm_to_num () {
    struct Text *text;
    struct Sym *sym;

    for (text = Texts->next; text != NULL; text = text->next) {
        /* blank */
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
            /* blank */

        } else if(inst[text->idx].type == 'I') {
            /* blank */

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
