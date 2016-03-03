float rayon_max = 120.0;

// This class brings together the information and methods about the cities and town we display 
class Place {
  int postalCode;        //postal code of the town
  String name;           //city name
  int x;                 //city real x position on the window
  int y;                 //city real y position on the window
  float population;      //city population
  float density;         //city density
  int rayon;             //radius of the population circle, proportional to the population 
  boolean isSelected;    //selection flag as you click on the city
  boolean isHovered;     //hovering flag as you hover on the city circle area

  Place(int postalCode, String name, int x, int y, float population, float density){
    this.postalCode = postalCode;
    this.name = name;
    this.x =x;
    this.y =y;
    this.population = population;
    this.density = density;
    this.isSelected = false;
    this.isHovered = false;
  }
    
  //set the radius, once we are aware of the maximum population among the list
  //also fill the neighbours table
  void set_rayon(){
    int xx,yy;
    this.rayon = (int)(rayon_max*(this.population/max_pop));
  }
  
  //draw the blue dot, which intensity is proportional to the city density
  void draw(){
    if(this.isConcerned()){
      float ratio = this.density/max_dens;
      int b = (int)(ratio*256*up(ratio))-1;
      color c = color(255-b,255-b,200);
      set(this.x, this.y, c);} 
  }
   
  //draw the city circle in a blue color 
  void drawCircles(){
    if(this.isConcerned()){
      noStroke();
      ellipseMode(CENTER);
      fill(0,39,74,100);
      ellipse(this.x,this.y,this.rayon,this.rayon);}
  }
   
  // draw the city circle in a red color
  void drawHighlighted(){
    if(this.isConcerned()){
      noStroke();
      ellipseMode(CENTER);
      fill(255,50,27,200);
      ellipse(this.x,this.y,this.rayon,this.rayon);
      if(mouseX-textWidth(this.name)-5>=0){
        fill(0,150);
        rect(mouseX-textWidth(this.name)-10, mouseY-25, textWidth(this.name)+10, 20+10, 5);
        fill(255);
        text(this.name,mouseX-textWidth(this.name)-5,mouseY-5);}
      else{
        fill(0,150);
        rect(mouseX-0, mouseY-25, textWidth(this.name)+10, 20+10, 5);
        fill(255);
        text(this.name,mouseX+5,mouseY-5);}
    }
  }
  
  //says if the given point is inside the city circle
  boolean contains(int xx, int yy){
    if(this.isConcerned() && abs(xx-this.x)*abs(xx-this.x)+abs(yy-this.y)*abs(yy-this.y)<=(this.rayon*this.rayon)/4)
      return true;
    else
      return false;
  } 
  
  //says if the city population is whithin the current displayed population range
  boolean isConcerned(){
    return this.population >= minPopulationToDisplay && this.population <= maxPopulationToDisplay;
  }
  
  //function to raise the density curve, hence favoring the low density city 
  float up(float x){
    float a = 1000000;
    return (-a*x*x+(1+a)*x);  
  }
}
