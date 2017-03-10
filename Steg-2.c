#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_MSG_LENGTH 100

//Defining One Pixel of PPM Image
typedef struct pixel  {
  int r, g, b;
} pixel;

//Creating a LinkedList
typedef struct lNode {
  char * data;
  struct lNode * next;
} lNode;

/*The PPM Image itself.
 * Consisting of:
 * an LNode pointer for iterating
 * over linked lists.
 * int values to store,
 * width, height, and the max value
 * and an array of pointers to pointers
 * for iterating over the matrix
 */
struct PPM{
  struct lNode * currNode;
  int width, height, max;
  struct pixel ** pixel;
} PPM;

struct PPM * getPPM(FILE * fd){
  char line[70]; //MAX a line can be in Valid PPM
  struct PPM * img = malloc(sizeof(struct PPM));
  fgets(line, 70, fd);

  //Begin checking format
  if(strcmp(line, "P3\n") != 0){
    fprintf(stderr, "\n Error: File Not in PPM, P3, Format \n");//throw an error
    exit(0);
  }
  //Format OK.
  fgets(line, 70, fd);

  //Begin reading comments - WON'T STORE?!
  while(line[0] == '#'){
    fgets(line, 70, fd);
  }
  //End reading Comments

  //Read Width and height
  int imageWidth, imageHeight, imageSize; //Create temp varibles to store values
  sscanf(line, "%d %d", &imageWidth, &imageHeight); //read formatted input
  img->width =  imageWidth; // store value of width
  img->height = imageHeight; // store value of height
  imageSize = imageHeight * imageWidth;
  //End reading Width and Height

  //Read IntMaxColour Val
  int intMax;
  fgets(line, 70, fd);
  sscanf(line, "%d", &intMax);
  img->max = intMax;
  //End Reading MaxColVal

  //Allocate Space for Rows
  img->pixel = (struct pixel ** ) malloc(sizeof(struct pixel *) * img->height);
  int j,k; //tmp variables
  for(j=0;j<img->height;j++){
  //Allocate space for columns
  img->pixel[j] = (struct pixel * ) malloc(sizeof(struct pixel) * img->width);
    for (k=0;k<img->width;k++){
      fscanf (fd, "%d", &(img->pixel[j][k].r));
      fscanf (fd, "%d", &(img->pixel[j][k].g));
      fscanf (fd, "%d", &(img->pixel[j][k].b));
    }}
  return img;
}

void showPPM(struct PPM * i){
  printf("P3\n");
  printf("%d\n", i->width);
  printf("%d\n", i->height);

  int k,l;
  for(k=0; k<i->height; k++){
    for(l=0; l<i->width; l++){
      struct pixel p = i->pixel[k][l];
      printf("%i %i %i\n", p.r, p.g, p.b);
  }}}

struct PPM * encode (char * text, struct PPM * i){
  srand(time(NULL));
  int j, acc, random;
  acc = 0;

  if(strlen(text) > MAX_MSG_LENGTH){
    printf("\n Message exceeds max message length.\n");
    exit(0);
  }

  for (j = 0; j< strlen(text); j++){
    random = (rand() % 100);
    acc = acc + random;

    int row, column;
    row = acc / i->width;
    column = acc - (row * i->width);

    struct pixel * p = &(i->pixel[row][column]);
    p->r = text[j];
  }
  return i;
}

char * decode(struct PPM * i1, struct PPM * i2){
	char * buffer = malloc(sizeof(char) * MAX_MSG_LENGTH);

	if(i1->height != i2->height || i1->width != i2->width){
		printf("Files differ in Height or Width\n");
		exit(0);
	}

	int c, r, l, m, n;
	l = 0;

	for(c = 0; c < i2->height ; c++){
		for(r = 0; r < i2->width ; r++){
			if(i2->pixel[c][r].r != i1->pixel[c][r].r)
				buffer[l] = i2->pixel[c][r].r;
				printf("%c", buffer[l]);
				l = l+1;
  }}

	m = strlen(buffer) - 1;

  if (buffer[m] == '\n'){
  buffer[m] = '\0';
  }

	char * str = malloc(sizeof(char) * (m+1));

	for(n=0; n<(m+1); n++)
		str[n] = buffer[n];

	free(buffer);
	return str;

}

void writePPM(FILE * fin, struct PPM * i, const char *fileName){
	FILE *pfile = NULL;
	int x,y;

	pfile = fopen(fileName, "w");
	fprintf(pfile, "P3\n%d %d\n255\n", i->width, i->height);

	for(x=0; x<i->height; x++) {
		for(y=0; y<i->width;y++){
			struct pixel p = i->pixel[x][y];
			fprintf(pfile, "%i %i %i \n", p.r, p.g, p.b);		}
	}
	fclose(pfile);
}

void encodeFile(int argc, char const ** argv){
	char txt[MAX_MSG_LENGTH];
	int j;

	FILE * fin = fopen(argv[2], "r");
	if(fin == NULL){
		fprintf(stderr," steg: Error - Unable to open the file '%s' .\n", argv[2]);
		exit(0);
	}

	struct PPM * i = getPPM(fin);

	fprintf(stderr, "Message to Encode-> ");
	fgets(txt, MAX_MSG_LENGTH, stdin);

	j = strlen(txt) - 1;
	if(txt[j] == '\n')
		txt[j] = '\0';

	encode(txt, i);
	writePPM(fin, i, "modified.ppm");
	showPPM(i);
}
void decodeFile(int argc, char const ** argv){

	FILE * i1 = fopen(argv[2], "r");
	FILE * i2 = fopen(argv[3], "r");

	if(i1 == NULL){
		printf("steg: Error - File '%s' could not be opened. \n", argv[2]);
		exit(0);
	}

	if(i2 == NULL){
		printf("steg: Error - File '%s' could not be opened. \n", argv[3]);
		exit(0);
	}

	//get files
	struct PPM * oPPM = getPPM(i1);
	struct PPM * ePPM = getPPM(i2);

	//get encoded txt
	char * str = decode(oPPM, ePPM);

	//print txt
	printf("%s\n", str);
}



int main(int argc, char const * argv[]){
  	if(argc < 3){
      printf("Please use e for Encode or d for Decode. Decode requires Two files");
  		exit(0);
  	}

  	if(argv[1][0] == 'e'){
  		encodeFile(argc, argv);
  	}
  	if(argv[1][0] == 'd'){
  		if(argc != 4){
  		 printf("Decode requires two files as arguments.\n");
       exit(0);
  		}
  		decodeFile(argc, argv);
  	}
}
