#include <SFML/Graphics.hpp>
#include <algorithm>


int main(){
    const unsigned int windowWidth = 800;
    const unsigned int windowHeight = 600;
    const float centerX = windowWidth / 2.0;
    const float centerY = windowHeight / 2.0;
    const float starRadOG = 30.0;
    sf::RenderWindow window; //so this is from the sfml. it is like an int or string but will open a window for us to mess wit
    //we could also have done below above but w/o using the .create because it would have used the constructor
    //for the 800x600. this is best for little projects, 1024x768 is a larger window, 1280x720 is for games, 1920x1080 is for full hd
    window.create(sf::VideoMode({windowWidth, windowHeight}), "Black Hole Simulator");

    //window open now make a star
    sf::CircleShape star(starRadOG); //circle is already apart of sfml, 30 is radius, f is for a float
    //now that we have variables for changeablility we need to change our logic
    star.setPosition({centerX - star.getRadius(), centerY - star.getRadius()});
    sf::Clock clock; //built in object from sfml, will help with the star disappearing almost instantly


    while(window.isOpen()){

        //we want to measure time so we can make it change more to how we want.
        //so we reset it and turn it into seconds so we can manipulate it
        //so line below is incorrect, reset jsut resets clock to 0
        //float timeChange = clock.reset().asSeconds();
        float timeChange = clock.restart().asSeconds();//this one is correct


        //so optional either points to the event happening or nothing(no event going on)
        //must use while b/c multiple events can be happening, stored as FIFO in optional 
        while (const std::optional<sf::Event> event = window.pollEvent()){
            //so this is like "hey is the event the user closing the window? yes? okay close it"
            if (event->is<sf::Event::Closed>()) { //event handling
                window.close();
            }
        }

        //so here we want the star to actually collapse, so we shrink it be .1 until we hit 0
        if(star.getRadius() >= 0){
            //star shrinks 5 pixels per second now
            //our max function prevents it from being a negative value
            star.setRadius(std::max(0.0f, star.getRadius() - 5.0f * timeChange));
        }






        //this will be at the bottom b/c we want to do all the math and fancy stuff and then draw what we calculated
        //clear window and fill with black...b/c its in space 
        window.clear(sf::Color::Black);
        window.draw(star);        
        window.display(); //this will display waht we draw before


    }



    return 0;
}