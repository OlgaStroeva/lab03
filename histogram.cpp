#include "histogram.h"
#include <iostream>
using namespace std;

const size_t SCREEN_WIDTH = 80;
const size_t MAX_ASTERISK = SCREEN_WIDTH - 3 - 1;

void find_minmax(const vector<double>& numbers, double &min, double &max) {
  min = numbers[0]; max = numbers[0];
      for (double number : numbers) {
          if (number < min) {
              min = number;
          }
          if (number > max) {
              max = number;
          }
      }
  }

const size_t HeightOfColumn = 25, Scale = 10; //scale = one char; border height = scale; space between columns = scale/2

//for first task
size_t ChooseYourColor(string color){

  if(color == "green") return 1;
  else if(color == "blue") return 0;
  else if(color == "pink") return 3;
  else if(color == "yellow") return 4;
  else if(color == "cyan") return 5;
  else return 6;
}

void Scale_under_image(const size_t max_count, size_t bin_count){ //Code for task 15.
  size_t scale;
  repeat:
  cin >> scale;
  if(scale<2 || scale>9) {
      cerr << "ERROR";
      goto repeat;
    }
  for(size_t i = 0; i <= MAX_ASTERISK && i <= max_count - max_count%scale + scale*(max_count%scale ? 1 : 0 ) ; ++i){
      if(!(i%scale) || i==MAX_ASTERISK) {
          cout << "<line x1='" << Scale*(5+i)  << "' y1='" << (HeightOfColumn+Scale/2)*bin_count + Scale << "' x2='" <<
                  Scale*(5+i) << "' y2='" << (HeightOfColumn+Scale/2)*bin_count + Scale + HeightOfColumn << "' stroke='black' stroke-width='2'/>";
          if(i==0 || i==scale || i==MAX_ASTERISK || (1+i > max_count - max_count%scale + scale*(max_count%scale ? 1 : 0 )))
          cout << "<text text-anchor='end' x='" << Scale*(5+i) << "' y='" << (HeightOfColumn+Scale/2)*bin_count + Scale + HeightOfColumn*1.5 << "'><tspan font-weight='bold' font-size='"
                 << Scale*2 << "'>" << i << "</tspan></text>\n";
        }
    }
  cout << "<line x1='" << Scale*5  << "' y1='" << (HeightOfColumn+Scale/2)*bin_count + HeightOfColumn*0.9 << "' x2='" <<
          Scale*(5 + min(MAX_ASTERISK, (max_count - max_count%scale + scale*(max_count%scale ? 1 : 0 )))) << "' y2='" <<
          (HeightOfColumn+Scale/2)*bin_count + HeightOfColumn << "' stroke='black' stroke-width='2'/>";
}


void draw_histogram_svg(const vector<size_t>& bins){
  size_t max_count = 0, i = 0;
  for (size_t count : bins) {
      if (count > max_count) {
          max_count = count;
      }
  }

  const bool scaling_needed = max_count > MAX_ASTERISK;
  const double scaling_factor = (double)MAX_ASTERISK / max_count;
  const size_t bin_count = bins.size();
  const char* colors[] = {"(0, 0, 255)", "(0, 255, 0)", "(255, 0, 0)", "(255, 0, 255)", "(255, 255, 0)", "(0, 255, 255)"};
  //print start of svg
  cout << "<?xml version='1.0' encoding='UTF-8' ?>\n<svg width='" << (SCREEN_WIDTH+5)*Scale << "' height='" //for example, if we don't heed "full screen" we can use (max_count+2)*Scale
       << bin_count * (HeightOfColumn+Scale/2) + Scale*5 /*1.5*/ << "' viewBox='0 0 " << (SCREEN_WIDTH+5)*Scale << ' '
       << bin_count * (HeightOfColumn+Scale/2) + Scale*5/*1.5*/ << "' xmlns='http://www.w3.org/2000/svg'>\n";

  //print all columns and text
  for(auto bin : bins){
      //Drawing text
      cout << "<text text-anchor='end' x='" << Scale*4 << "' y='" << (HeightOfColumn+Scale/2)*(i+1) << "'><tspan font-weight='bold' font-size='"
           << HeightOfColumn << "'>" << bin << "</tspan></text>\n";

      //Drawing rectangle. Separated function isn't needed cause it's single line where we need all paramet
    cout << "<rect x='" << Scale*5 << "' y='" << (HeightOfColumn+Scale/2)*i + Scale << "' width='" << (scaling_needed ?
            ((size_t)(bin * scaling_factor)) : bin) * Scale <<
            "' height='" << HeightOfColumn << "' style='fill:rgb" <<
            //here is a first task. There is some random colors in files.
            //colors[ChooseYourColor()] <<
            colors[i%6] <<
            "' />\n";
    ++i;
    }
  //Task for 15th variant
  Scale_under_image(max_count, bin_count);



  //here we print the second task
  //cout << "<text text-anchor='middle' x='"<< (SCREEN_WIDTH+5)*Scale/2 <<"' y='" << (bin_count * (HeightOfColumn+Scale/2) + Scale*5)/2 + Scale
  //     << "'><tspan font-weight='bold' font-size='" << HeightOfColumn << "'>Гистограмма</tspan></text>\n";



//here we print the end of SVG
  cout << "</svg>";
}
