#include <SFML/Graphics.hpp>



int main(){


    sf::RenderWindow window; //so this is from the sfml. it is like an int or string but will open a window for us to mess wit
    //we could also have done below above but w/o using the .create because it would have used the constructor
    //for the 800x600. this is best for little projects, 1024x768 is a larger window, 1280x720 is for games, 1920x1080 is for full hd
    window.create(sf::VideoMode({800, 600}), "Black Hole Simulator");
    while(window.isOpen()){

        //so optional either points to the event happening or nothing(no event going on)
        //must use while b/c multiple events can be happening, stored as FIFO in optional 
        while (const std::optional<sf::Event> event = window.pollEvent()){
            //so this is like "hey is the event the user closing the window? yes? okay close that mf"
            if (event->is<sf::Event::Closed>()) { //event handling
                window.close();
            }
            //clear window and fill with black...b/c its in space 
            window.clear(sf::Color::Black);
            //here we will draw the stuff later, math, stars, all the good stuff
        
            window.display(); //this will display waht we draw before


        }


    }



    return 0;
}