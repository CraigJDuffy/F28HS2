#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define AUTHOR "CRAIG J DUFFY"

//Linked List Implementation
typedef struct list {char * data; struct list * next;} lNode;

//One Pixel of a PPM Image
typedef struct{
  unsigned char red, green, blue;
} PPMPixel;

//PPM Image
typedef struct{
  int width, height, max;
  lNode *pHead, *cHead;
}PPMImage;


//Returns a PPMImage from a File fd
PPMImage getPPM(FILE * fd){
    PPMImage  * currImage; // Create a new pointer to a PPMImage
    currImage = (PPMImage *) malloc(sizeof(PPMImage)); //allocate space for it

    lNode * curNode; //Create a new pointer to a lNode
    char * curChar; // Create a pointer to a char
    curNode = (lNode *) malloc(sizeof(lNode)); //Create space in memory for the node
    currImage->cHead = curNode;
    printf("\n %p", curNode);

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
        curNode->data = strcpy(curChar, line); //store it
        curNode->next = (lNode *) malloc(sizeof(lNode));//create a new node
        curNode = curNode->next; //set the pointer to the next node
        fgets(line, 70 , fd);//read the next line
    }
    //End Reading Comments -- HEAD OF LLIST APPEARS TO BE STORED

    //Read Width and height -- WORKING
    int imageWidth, imageHeight; //Create temp varibles to store values
    sscanf(line, "%d %d", &imageWidth, &imageHeight); //read formatted input
    currImage->width =  imageWidth; // store value of width
    currImage->height = imageHeight; // store value of height
    //End reading Width and Height

    //Read IntMaxColour Val -- WORKING
    int intMax;
    fgets(line, 70, fd);
    sscanf(line, "%d", &intMax); //read formatted input
    currImage->max = intMax;
    printf("\n");

    //Begin Reading Pixels -- Currently does not store
    int r, g, b, count;
    PPMPixel * currPixel;
    currPixel = (PPMPixel *) malloc(sizeof(PPMPixel)); //allocate space for it
    while(line[0] != EOF){
      if(fgets(line,70,fd) == NULL){
       printf("\n Finished reading file: ");
       curNode = currImage->cHead;
       printf("\n %p", curNode);
       printf("\n Width: %d ", currImage->width);
       printf("\n Height: %d ", currImage->height);
       printf("\n MaxColVal: %d", currImage->max);
       printf("\n PixelData: %p" , currImage->pHead);
       printf("\n");
       exit(1);
      }
      else{
      sscanf(line, "%d %d %d", &r, &g, &b);
      currPixel->red = r;
      currPixel->green = g;
      currPixel->blue =b;
     // curNode->data = currPixel;
      curNode->next = (lNode *) malloc(sizeof(lNode));//create a new node
      curNode = curNode->next;
      }
    }
return * currImage;
}

void showPPM(PPMImage i){
}

int main(int argc, char ** argv){
  getPPM(fopen(argv[1], "r"));
}
