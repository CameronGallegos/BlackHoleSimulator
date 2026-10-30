#include <SFML/Graphics.hpp>



int main(){


    sf::RenderWindow window; //so this is from the sfml. it is like an int or string but will open a window for us to mess wit
    //we could also have done below above but w/o using the .create because it would have used the constructor
    //for the 800x600. this is best for little projects, 1024x768 is a larger window, 1280x720 is for games, 1920x1080 is for full hd
    window.create(sf::VideoMode({800, 600}), "Black Hole Simulator");

    //window open now make a star
    sf::CircleShape star(30.f); //circle is already apart of sfml, 30 is radius, f is for a float
    //so by default, the shape will be top left in window, we want it to be in the center(400, 300)
    //the measure starts on the left of the shape, we need to shift 30 left and 30 up (370, 270) 
    star.setPosition({370.f, 270.f});


    while(window.isOpen()){

        //so optional either points to the event happening or nothing(no event going on)
        //must use while b/c multiple events can be happening, stored as FIFO in optional 
        while (const std::optional<sf::Event> event = window.pollEvent()){
            //so this is like "hey is the event the user closing the window? yes? okay close it"
            if (event->is<sf::Event::Closed>()) { //event handling
                window.close();
            }
        }

        //clear window and fill with black...b/c its in space 
        window.clear(sf::Color::Black);
        window.draw(star);        
        window.display(); //this will display waht we draw before


    }



    return 0;
}