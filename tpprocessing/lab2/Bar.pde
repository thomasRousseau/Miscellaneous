//This class brings together the information and display functions of the population range bar
class Bar{
  PImage bar_img;          //bar image
  PImage lbar_img;         //left slider image
  PImage rbar_img;         //right slider image
  int xpos;                //initial x position
  int ypos;                //initial y position
  int minxpos = xpos;      //initial left slider position
  int maxxpos = ypos;      //initial right slider position
  int typos = ypos;        //real displayed y position
  int currentminpos;       //currently displayed left slider position
  int currentmaxpos;       //currently displayed right slider position
  boolean isLeftDragged;   //says if the left slider is being dragged
  boolean isRightDragged;  //says if the right slider is being dragged
  float ratiomin;          //minimum sliding ratio
  float ratiomax;          //maximum sliding ratio
  int[] Hist;              //table of the values for the histogram 
    
  Bar(){
    this.bar_img = loadImage("bar.png");
    this.lbar_img = loadImage("left_bar.gif");
    this.rbar_img = loadImage("right_bar.gif"); 
    this.xpos = 50;
    this.ypos = 770;
    this.minxpos = xpos - 5;
    this.maxxpos = 750 - 7;
    this.typos = ypos - 5;
    this.currentminpos = minxpos;
    this.currentmaxpos = maxxpos;
    this.isLeftDragged = false;
    this.isRightDragged = false;
    this.ratiomin = 0;
    this.ratiomax = 1;
    this.Hist = new int[(maxxpos-minxpos)/2-1];
    int count;
    //fill the histogram table
    for(int i = 0; i < (maxxpos-minxpos)/2-1;i++){
      int inf = fpop((2*i)/float(maxxpos-minxpos));
      int sup = fpop((2*i+2)/float(maxxpos-minxpos)); 
      count = 1;
      for (int ii = 0; ii < numPoints-2; ++ii) {
         if(places[ii].population >= inf && places[ii].population < sup)
           count++; 
      }
      this.Hist[i] = count;
    }
  }
  
  //update the is*Dragged value if the click is done on a slider
  void goodclick(int x, int y){
    if(x >= this.currentminpos && x <= this.currentminpos+12 && y >= typos && y <= typos+25)
      this.isLeftDragged = true;
    else if(x >= this.currentmaxpos && x <= this.currentmaxpos+12 && y >= typos && y <= typos+25)
      this.isRightDragged = true;
  }  
  
  //set the current minimum population ratio, on the log scale 
  void setmin(int x){
    this.currentminpos = max(x,this.minxpos);
    this.ratiomin = float(currentminpos - minxpos)/float(maxxpos - minxpos);
    minPopulationToDisplay = fpop(ratiomin);}
  
  //set the current maximum popualtion ratio, on the log scale
  void setmax(int x){
    this.currentmaxpos = min(x,this.maxxpos);
    this.ratiomax = float(currentmaxpos - minxpos)/float(maxxpos - minxpos);
    maxPopulationToDisplay = fpop(ratiomax);}
    
  //function to raise the ratio on the log scale
  int fpop(float ratio){
    float b = 10;
    float cons = 1.0/(exp(b)-1);
    return (int)(max_pop * (exp(b*ratio)-1) * cons); 
  }
  
  //inverse function of the previous function (to take care of the keyboard inputs)
  int fpopinv(int pop){
    float b = 10.0;
    float p = pop/max_pop;
    float cons = p*(exp(b)-1);
    float x = log(cons+1)/b;
    return (int)((x*float(maxxpos - minxpos))+minxpos);
  }

  //draw function of the bar
  void draw(int pop){
    drawHist(pop);
    image(bar_img,xpos,ypos);
    image(lbar_img,currentminpos,typos);
    image(rbar_img,currentmaxpos,typos);
    fill(0,39,74,200);
    rect(currentminpos+12,ypos,currentmaxpos-currentminpos-12,14);
  }
  
  //histogram drawing function
  void drawHist(int pop){
    int max_count = 1727;
    int max_h = 35;
    int th;
    int min_y = typos+4;
    for(int i = 0; i < (maxxpos-minxpos)/2-1;i++){
       fill(0,39,74,100);
       int inf = fpop((2*i)/float(maxxpos-minxpos));
       int sup = fpop((2*i+2)/float(maxxpos-minxpos)); 
       if(pop >= inf && pop < sup)
         fill(255,50,27,200);
       th = (max_h * Hist[i])/max_count;
       th*=(2*(i/float(maxxpos-minxpos))+1);
       rect(xpos+3+2*i, min_y-th,2,th+7);
    }
  }
}
