#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define AUTHOR "CRAIG J DUFFY"

//Linked List Implementation
typedef struct list {char * data; struct list * next;} lNode;
//PPM Image
typedef struct{
  int width, height, max, **pHead;
  lNode *cHead;
}PPMImage;


//Returns a PPMImage from a File fd
PPMImage getPPM(FILE * fd){
    PPMImage  * currImage; // Create a new pointer to a PPMImage
    currImage = (PPMImage *) malloc(sizeof(PPMImage)); //allocate space for it

    lNode * currNode; //Create a new pointer to a lNode
    char * curChar; // Create a pointer to a char
    currNode = (lNode *) malloc(sizeof(lNode)); //Create space in memory for the node
    currImage->cHead = currNode;

    char line[70]; //MAX a line can be in valid PPM
    fgets(line, 70 , fd); //Get PPM File Code

    //Confirm file is in P3 format
    if(strcmp(line, "P3\n") != 0){ //if the first string is equal to anything but P3 new line
        fprintf(stderr, "\n Error: File Not in PPM, P3, Format \n");//throw an error
        exit(0);//exit fail state
    }

    //Begin Reading Comments
    fgets(line, 70 , fd); //Get the next line from the file
    while(line[0] == '#'){ //Line is A Comment
        curChar = (char *) malloc(strlen(line) * sizeof(char)); //allocate space to store it
        currNode->data = strcpy(curChar, line); //store it
        currNode->next = (lNode *) malloc(sizeof(lNode));//create a new node
        currNode = currNode->next; //set the pointer to the next node
        currNode->next = NULL;
        fgets(line, 70 , fd);//read the next line
    }
    //End Reading Comments

    //Read Width and height
    int imageWidth, imageHeight, imageSize; //Create temp varibles to store values
    sscanf(line, "%d %d", &imageWidth, &imageHeight); //read formatted input
    currImage->width =  imageWidth; // store value of width
    currImage->height = imageHeight; // store value of height
    imageSize = imageHeight * imageWidth;
    //End reading Width and Height

    //Read IntMaxColour Val
    int intMax;
    fgets(line, 70, fd);
    sscanf(line, "%d", &intMax); //read formatted input
    currImage->max = intMax;

    //Begin Reading Pixels
    int r, g, b, column, row;
    int ** pixels = (int **) malloc(sizeof(int) * imageSize);

    for ( column = 0; column < (imageSize); column++){
      pixels [column] = malloc(3 * sizeof(int));
      fgets(line, 70, fd);
      sscanf(line, "%d %d %d", &r, &g, &b );
      pixels [column] [row] = r;
      pixels [column] [row + 1] = g;
      pixels [column] [row + 2] = b;

      printf("\n %d    %d    %d   ", pixels [column] [row], pixels [column] [row +1], pixels [column] [row +2]);
    }
return * currImage;
}

void showPPM(PPMImage i){
  printf("\n P3");
  lNode * currNode;
  currNode = i.cHead;
  while (currNode->next != NULL){
    printf("\n %s", currNode->data);
    currNode = currNode->next;
  }
  printf("\n %d %d", i.width, i.height);
  printf("\n %d \n", i.max);
  printf("\n Pixels: %p \n", i.pHead);
  int counter = 0;

 /*while(counter < i.width * i.height){
    printf("%d", i.pHead[counter][counter]);
    counter ++;
  }*/
}


int main(int argc, char ** argv){
  showPPM(getPPM(fopen(argv[1], "r")));
}
